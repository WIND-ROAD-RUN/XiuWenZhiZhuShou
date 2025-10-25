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
