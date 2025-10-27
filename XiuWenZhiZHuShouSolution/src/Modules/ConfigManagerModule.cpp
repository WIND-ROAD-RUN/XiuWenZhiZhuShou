#include "ConfigManagerModule.hpp"

#include "Utilty.hpp"
#include "Modules.hpp"

bool ConfigManagerModule::build()
{
    storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);

#pragma region readHandleScannerCfg
	auto loadMainWindowConfig = storeContext->loadSafe(globalPath.MainWindowConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		mainWindowConfig = *loadMainWindowConfig;
	}
#pragma endregion

	return true;
}

void ConfigManagerModule::destroy()
{
	storeContext->saveSafe(mainWindowConfig, globalPath.MainWindowConfigPath.toStdString());
	storeContext.reset();
}

void ConfigManagerModule::start()
{
	mainWindowConfig.isDebug = false;
	mainWindowConfig.isDefect = true;
}

void ConfigManagerModule::stop()
{

}
