#include "ImgProModule.hpp"

#include "Modules.hpp"
#include "osoFIleUtiltyFunc.hpp"
#include "Utilty.hpp"

bool ImgProModule::build()
{
	buildImgProContext();
	buildImageProcessingModule(imgProSignalWorkThreadNum);
	for (int i = 0; i < 4; i++)
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
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;

#pragma region  build base
	imageProcessContext_Main.imageProcessPrepare = [this, &runningState, &handleScannerConfig](rw::imgPro::ImageProcessContext& context)
		{

			imageCenterX = 0;
			bodyCount = 0;

			auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
			auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;

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
					currentPixToWorld = setConfig.xiangSuDangLiang1;
				}
				else if (2 == ImgProcessIndex)
				{
					currentPixToWorld = setConfig.xiangSuDangLiang2;
				}
				context.customFields["CurrentPixToWorld"] = static_cast<double>(currentPixToWorld);
			}

			//update Config
			{
				if (imgProIsUpdate[stationIdx])
				{
					context.eliminationCfg = cdm::ScoreConfigConvert::toClassIdWithEliConfigMap(scoreConfig, currentPixToWorld * currentPixToWorld, 100);
					context.defectCfg = cdm::ScoreConfigConvert::toClassIdWithDefConfigMap(scoreConfig);
					imgProIsUpdate[stationIdx] = false;
				}
			}


			// update limite
			{
				int limitLeft{ 0 };
				int limitRight{ 0 };
				int limitTop{ 0 };
				int limitBottom{ 0 };
				int centralLimit{0};

				if (1 == ImgProcessIndex)
				{
					limitLeft = static_cast<int>(setConfig.zuoXianWei1);
					limitRight = static_cast<int>(setConfig.youXianWei1);
					limitTop = static_cast<int>(setConfig.shangXianWei1);
					limitBottom = static_cast<int>(setConfig.xiaXianWei1);
					centralLimit = static_cast<int>(setConfig.zhongyangxianwei1);
				}
				else if (2 == ImgProcessIndex)
				{
					limitLeft = static_cast<int>(setConfig.zuoXianWei2);
					limitRight = static_cast<int>(setConfig.youXianWei2);
					limitTop = static_cast<int>(setConfig.shangXianWei2);
					limitBottom = static_cast<int>(setConfig.xiaXianWei2);
					centralLimit = static_cast<int>(setConfig.zhongyangxianwei2);
				}

				context.customFields["LimitLeft"] = static_cast<int>(limitLeft);
				context.customFields["LimitRight"] = static_cast<int>(limitRight);
				context.customFields["LimitTop"] = static_cast<int>(limitTop);
				context.customFields["LimitBottom"] = static_cast<int>(limitBottom);
				context.customFields["CentralLimit"] = static_cast<int>(centralLimit);
			}

			// update drawConfig
			{
				if (RunningState::Debug == runningState)
				{
					context.defectDrawCfg.textLocate = rw::imgPro::ConfigDrawRect::TextLocate::LeftTopIn;

					if (handleScannerConfig.isshibiekuang)
					{
						context.defectDrawCfg.isDrawDefects = true;
						context.defectDrawCfg.isDrawDisableDefects = true;
						context.defectDrawCfg.isDisAreaText = true;
						context.defectDrawCfg.isDisScoreText = true;
					}
					else
					{
						context.defectDrawCfg.isDrawDefects = false;
						context.defectDrawCfg.isDrawDisableDefects = false;
						context.defectDrawCfg.isDisAreaText = false;
						context.defectDrawCfg.isDisScoreText = false;
					}

					if (handleScannerConfig.iswenzi)
					{
						context.runTextCfg.isDrawExtraText = true;
					}
					else
					{
						context.runTextCfg.isDrawExtraText = false;
					}
				}
				else if (RunningState::OpenRemoveFunc == runningState)
				{
					context.defectDrawCfg.isDrawDefects = true;
					context.defectDrawCfg.isDrawDisableDefects = true;
					context.defectDrawCfg.isDisAreaText = true;
					context.defectDrawCfg.isDisScoreText = true;

					context.runTextCfg.isDrawExtraText = false;
				}
			}

			context.customFields["BodyIndexWithDefectsIndexMap"] = BodyIndexWithDefectsIndexMap();
			context.customFields["DefectsIndexWithBodyIndex"] = DefectsIndexWithBodyIndex();
			context.customFields["DefectBoxs"] = DefectBoxs();
		};
#pragma endregion

#pragma region build index get
	imageProcessContext_Main.indexGetContext.removeIndicesIfByInfo = [this](const rw::DetectionRectangleInfo& info
		, rw::imgPro::ImageProcessContext& context)
		{
			bool isInShieldWires = false;
			int limitLeft{ -1 };
			int limitRight{ -1 };
			int limitTop{ -1 };
			int limitBottom{ -1 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}
			if (context.customFields.find("LimitTop") != context.customFields.end()) {
				limitTop = std::any_cast<int>(context.customFields["LimitTop"]);
			}
			if (context.customFields.find("LimitBottom") != context.customFields.end()) {
				limitBottom = std::any_cast<int>(context.customFields["LimitBottom"]);
			}



			if (-1 == limitLeft || -1 == limitRight || -1 == limitTop || -1 == limitBottom)
			{
				return false;
			}
			// 判断缺陷框中心点是否在屏蔽线区域内并且只对与把手进行判断
			if (info.classId == ClassId::body)
			{
				if (info.center_x < limitRight && info.center_x > limitLeft)
				{
					if (info.center_y > limitTop && info.center_y < limitBottom)
					{
						isInShieldWires = true;
					}
				}
			}
			else
			{
				isInShieldWires = true;
			}


			return !isInShieldWires;
		};

	imageProcessContext_Main.indexGetContext.removeIndicesPost =
		[this](rw::imgPro::ProcessResultIndexMap& indexMap,
			const rw::imgPro::ProcessResult& processResult,
			rw::imgPro::ImageProcessContext& context) {

				bodyCount = indexMap[ClassId::body].size();

				if (0 == bodyCount)
				{
					return;
				}

				auto bodyIndexWithDefectsIndexMap = std::any_cast<BodyIndexWithDefectsIndexMap>(context.customFields["BodyIndexWithDefectsIndexMap"]);

				// 初始化
				for (const auto& indexMapItem : indexMap[ClassId::body])
				{
					bodyIndexWithDefectsIndexMap[indexMapItem];
				}

				auto defectsIndexWithBodyIndex = std::any_cast<DefectsIndexWithBodyIndex>(context.customFields["DefectsIndexWithBodyIndex"]);
				// 遍历所有缺陷
				for (const auto& indexMapItem : indexMap)
				{
					// 获得当前缺陷类型
					auto& currentClassId = indexMapItem.first;
					if (currentClassId == ClassId::body)
					{
						continue;
					}
					// 找出不是把手的缺陷
					for (const auto& defectIndex : indexMapItem.second)
					{
						// 获得当前缺陷识别结果
						auto& currentDefectResult = processResult[defectIndex];
						// 遍历所有把手
						for (const auto& handleIndex : indexMap[ClassId::body])
						{
							auto& bodyProResult = processResult[handleIndex];
							auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
							auto& pixToWorld = context.customFields.at("CurrentPixToWorld");
							auto deviation = setConfig.xunzhaoquexiansuoshubashoudepianchazhi / std::any_cast<double>(pixToWorld);

							auto isInHandle = rw::DetectionRectangleInfo::findPointIsInOtherDet({ currentDefectResult.center_x ,currentDefectResult.center_y }, bodyProResult, deviation);
							if (isInHandle)
							{
								defectsIndexWithBodyIndex[defectIndex] = handleIndex;
							}
						}
					}

					context.customFields["BodyIndexWithDefectsIndexMap"] = bodyIndexWithDefectsIndexMap;
					context.customFields["DefectsIndexWithBodyIndex"] = defectsIndexWithBodyIndex;

				}
		};
#pragma endregion

#pragma region build elimination config

#pragma endregion

#pragma region build defect config
	imageProcessContext_Main.defectResultGetContext.getDefectResultExtraPostOperate = [this](
		const rw::imgPro::ProcessResult& processResult,
		const rw::imgPro::ClassIdWithEliminationInfoConfigMap& classIdWithEliminationInfoConfigMap,
		const rw::imgPro::EliminationInfo& eliminationInfo,
		const rw::imgPro::ClassIdWithDefectResultInfoFuncConfigMap& classIdWithDefectResultInfoFuncConfigMap,
		rw::imgPro::DefectResultInfo& defectResultInfo,
		rw::imgPro::ImageProcessContext& context) {

			auto defectsIndexWithBodyIndex = std::any_cast<DefectsIndexWithBodyIndex>(context.customFields["DefectsIndexWithBodyIndex"]);
			auto bodyIndexWithDefectsIndexMap = std::any_cast<BodyIndexWithDefectsIndexMap>(context.customFields["BodyIndexWithDefectsIndexMap"]);
			// 获得坏的缺陷(满足剔废条件)
			for (const auto& infoItem : defectResultInfo.defects)
			{
				// 去除把手
				if (infoItem.first != ClassId::body && infoItem.first != ClassId::zhanlian&& infoItem.first != ClassId::feiliao && infoItem.first != ClassId::mojucuowu)
				{
					// 获得所有坏的缺陷
					for (const auto& infoItemSecond : infoItem.second)
					{
						// 根据缺陷索引找到所属的把手索引
						if (defectsIndexWithBodyIndex.find(infoItemSecond.index) != defectsIndexWithBodyIndex.end())
						{
							// 将找到的缺陷索引放入对应把手索引的map中
							bodyIndexWithDefectsIndexMap[defectsIndexWithBodyIndex.at(infoItemSecond.index)][infoItemSecond.classId].push_back(infoItemSecond.index);
						}
					}
				}

			}

			context.customFields["defectsIndexWithBodyIndex"] = defectsIndexWithBodyIndex;
			context.customFields["BodyIndexWithDefectsIndexMap"] = bodyIndexWithDefectsIndexMap;
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
			int limitTop{ 0 };
			int limitBottom{ 0 };

			int centralLimit{ 0 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}
			if (context.customFields.find("LimitTop") != context.customFields.end()) {
				limitTop = std::any_cast<int>(context.customFields["LimitTop"]);
			}
			if (context.customFields.find("LimitBottom") != context.customFields.end()) {
				limitBottom = std::any_cast<int>(context.customFields["LimitBottom"]);
			}
			if (context.customFields.find("CentralLimit") != context.customFields.end()) {
				centralLimit = std::any_cast<int>(context.customFields.at("CentralLimit"));
			}

			rw::imgPro::ConfigDrawLine configDrawLine;
			configDrawLine.color = rw::imgPro::Color::Orange;
			configDrawLine.thickness = 20;

			configDrawLine.position = limitTop;
			rw::imgPro::ImagePainter::drawHorizontalLine(img, configDrawLine);
			configDrawLine.position = limitBottom;
			rw::imgPro::ImagePainter::drawHorizontalLine(img, configDrawLine);
			configDrawLine.position = limitLeft;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);
			configDrawLine.position = limitRight;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);

			configDrawLine.color = rw::imgPro::Color::Yellow;
			configDrawLine.isDashed = true;
			configDrawLine.thickness = 10;
			configDrawLine.position = centralLimit;
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

	imageProcessingModule2 = std::make_unique<ImageProcessingModuleHandleScanner>(num, this);
	imageProcessingModule2->modelEnginePath = globalPath.modelPath;
	imageProcessingModule2->index = 2;
	imageProcessingModule2->BuildModule();
}

void ImgProModule::destroyImageProcessingModule()
{
	imageProcessingModule1.reset();
	imageProcessingModule2.reset();
}

void ImgProModule::onUpdateImgProContext()
{
	buildImgProContext();
	resetImgProIsUpdate(true);
}
