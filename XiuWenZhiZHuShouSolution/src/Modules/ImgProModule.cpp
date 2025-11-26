#include "ImgProModule.hpp"
#include "Modules.hpp"
#include "osoFIleUtiltyFunc.hpp"
#include "Utilty.hpp"

bool ImgProModule::build()
{
	buildImgProContext();
	buildImageProcessingModule(imgProSignalWorkThreadNum);
	for (int i = 0; i < 2; i++)
	{
		imgProIsUpdate[i] = true;
	}
	return true;
}

void ImgProModule::destroy()
{
	destroyImageProcessingModule();
}

void ImgProModule::start()
{

}

void ImgProModule::stop()
{

}

void ImgProModule::buildImgProContext()
{
	buildImgProContextMain();
}

void ImgProModule::buildImgProContextMain()
{
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

#pragma region  build base
	imageProcessContext_Main.imageProcessPrepare = [this, &runningState, &mainWindowConfig](rw::imgPro::ImageProcessContext& context)
		{
			if (context.customFields.find("ImgProcessIndex") == context.customFields.end()) {
				return;
			}
			auto ImgProcessIndex = std::any_cast<int>(context.customFields["ImgProcessIndex"]);

			if (context.customFields.find("stationIdx") == context.customFields.end()) {
				return;
			}
			auto stationIdx = std::any_cast<int>(context.customFields["stationIdx"]);

			double currentPixToWorld{ 0 };

			//update pixToWorld
			{
				if (1 == ImgProcessIndex)
				{
					currentPixToWorld = mainWindowConfig.xiangsudangliang;
				}
				context.customFields["CurrentPixToWorld"] = static_cast<double>(currentPixToWorld);
			}

			//update Config
			{
				if (imgProIsUpdate[stationIdx])
				{
					context.eliminationCfg = cdm::ScoreConfigConvert::toClassIdWithEliConfigMap(currentPixToWorld * currentPixToWorld, 100);
					context.defectCfg = cdm::ScoreConfigConvert::toClassIdWithDefConfigMap();
					imgProIsUpdate[stationIdx] = false;
				}
			}

			// update limite
			{
				int limitLeft{ 0 };
				int limitRight{ 0 };

				if (1 == ImgProcessIndex)
				{
					limitLeft = static_cast<int>(mainWindowConfig.zuoxianwei);
					limitRight = static_cast<int>(mainWindowConfig.youxianwei);
				}

				context.customFields["LimitLeft"] = static_cast<int>(limitLeft);
				context.customFields["LimitRight"] = static_cast<int>(limitRight);
			}

			// update drawConfig
			{
				if (RunningState::Debug == runningState)
				{
					context.defectDrawCfg.textLocate = rw::imgPro::ConfigDrawRect::TextLocate::LeftTopIn;

					context.defectDrawCfg.isDrawDefects = true;
					context.defectDrawCfg.isDrawDisableDefects = true;
					context.defectDrawCfg.isDisAreaText = false;
					context.defectDrawCfg.isDisScoreText = false;
					context.runTextCfg.isDrawExtraText = false;
				}
				else if (RunningState::OpenRemoveFunc == runningState)
				{
					context.defectDrawCfg.isDrawDefects = true;
					context.defectDrawCfg.isDrawDisableDefects = true;
					context.defectDrawCfg.isDisAreaText = false;
					context.defectDrawCfg.isDisScoreText = false;
					context.runTextCfg.isDrawExtraText = false;
				}
			}
		};
#pragma endregion

#pragma region build index get
	imageProcessContext_Main.indexGetContext.removeIndicesIfByInfo = [this](const rw::DetectionRectangleInfo& info
		, rw::imgPro::ImageProcessContext& context)
		{
			bool isInShieldWires = false;
			int limitLeft{ -1 };
			int limitRight{ -1 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}

			if (-1 == limitLeft || -1 == limitRight)
			{
				return false;
			}
			// 判断缺陷框中心点是否在屏蔽线区域内
			if (info.center_x < limitRight && info.center_x > limitLeft)
			{
				isInShieldWires = true;
			}

			return !isInShieldWires;
		};
#pragma endregion

#pragma region build defect draw
	imageProcessContext_Main.defectDrawCfg.classIdNameMap = ClassId::classIdNameMap;

	rw::imgPro::DefectDrawConfigItem drawItemConfig;

	drawItemConfig.fontSize = 50;
	drawItemConfig.textLocate = rw::imgPro::ConfigDrawRect::TextLocate::LeftTopIn;

	for (size_t i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[i] = drawItemConfig;
	}

	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisAreaText = false;
	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisScoreText = false;
	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisName = false;

	imageProcessContext_Main.defectDrawFuncContext.postOperateFunc = [](
		QImage& img,
		rw::imgPro::ImageProcessContext& context) {

			int limitLeft{ 0 };
			int limitRight{ 0 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}

			rw::imgPro::ConfigDrawLine configDrawLine;
			configDrawLine.color = rw::imgPro::Color::Orange;
			configDrawLine.thickness = 10;

			configDrawLine.position = limitLeft;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);
			configDrawLine.position = limitRight;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);
		};

#pragma endregion

#pragma region build running time text
	imageProcessContext_Main.runTextCfg.isDisProcessImgTime = false;
	imageProcessContext_Main.runTextCfg.isDrawExtraText = false;
#pragma endregion
}

void ImgProModule::resetImgProIsUpdate(bool state)
{
	for (auto& flag : imgProIsUpdate) {
		flag.store(state);
	}
}

void ImgProModule::buildImageProcessingModule(size_t num)
{
	imageProcessingModule1 = std::make_unique<ImageProcessingModuleHandleScanner>(num, this);
	imageProcessingModule1->modelEnginePath = globalPath.modelPath;
	imageProcessingModule1->index = 1;
	imageProcessingModule1->BuildModule();
}

void ImgProModule::destroyImageProcessingModule()
{
	imageProcessingModule1.reset();
}

void ImgProModule::onUpdateImgProContext()
{
	buildImgProContext();
	resetImgProIsUpdate(true);
}
