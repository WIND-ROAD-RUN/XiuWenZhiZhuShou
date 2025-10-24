#pragma once

#include"IModule.hpp"
#include<QObject>
#include "oso_StorageContext.hpp"
#include "ScoreConfig.hpp"
#include "SetConfig.hpp"
#include "HandleScanner.hpp"

class ConfigManagerModule
	: public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
public:
	std::unique_ptr<rw::oso::StorageContext> storeContext{ nullptr };
public:
	cdm::HandleScannerConfig handleScannerConfig{};
	cdm::ScoreConfig scoreConfig{};
	cdm::SetConfig setConfig{};
};
