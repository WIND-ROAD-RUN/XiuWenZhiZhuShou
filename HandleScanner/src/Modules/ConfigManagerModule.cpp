#include "ConfigManagerModule.hpp"

#include "Utilty.hpp"
#include "Modules.hpp"

bool ConfigManagerModule::build()
{
    storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);
	auto& runtimeModule = Modules::getInstance().runtimeInfoModule;

#pragma region readHandleScannerCfg
	auto loadMainWindowConfig = storeContext->loadSafe(globalPath.HandleScannerConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		handleScannerConfig = *loadMainWindowConfig;
		runtimeModule.isTakePictures = handleScannerConfig.isSaveImg;
		runtimeModule.statisticalInfo.produceCount = handleScannerConfig.totalProductionVolume;
		runtimeModule.statisticalInfo.wasteCount = handleScannerConfig.totalDefectiveVolume;
		runtimeModule.statisticalInfo.productionYield = handleScannerConfig.productionYield;
		runtimeModule.statisticalInfo.handleCountForStop = handleScannerConfig.tingjigeshu;
	}
#pragma endregion

#pragma region readscoreCfg
	loadMainWindowConfig = storeContext->loadSafe(globalPath.DlgProductScoreConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		scoreConfig = *loadMainWindowConfig;
	}
#pragma endregion

#pragma region readsetCfg
	loadMainWindowConfig = storeContext->loadSafe(globalPath.DlgProductSetConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		setConfig = *loadMainWindowConfig;
	}
#pragma endregion

	return true;
}

void ConfigManagerModule::destroy()
{
	storeContext->saveSafe(handleScannerConfig, globalPath.HandleScannerConfigPath.toStdString());
	storeContext->saveSafe(scoreConfig, globalPath.DlgProductScoreConfigPath.toStdString());
	storeContext->saveSafe(setConfig, globalPath.DlgProductSetConfigPath.toStdString());
	storeContext.reset();
}

void ConfigManagerModule::start()
{
	handleScannerConfig.isDebug = false;
	handleScannerConfig.isDefect = true;
}

void ConfigManagerModule::stop()
{

}
