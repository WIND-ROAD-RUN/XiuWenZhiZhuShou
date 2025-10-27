#pragma once

#include"IModule.hpp"

#include<QObject>

enum class RunningState
{
	Debug,
	OpenRemoveFunc,
	Stop
};

class RuntimeInfoModule : public QObject, public IModule<bool>
{
	Q_OBJECT
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;

public:
	std::atomic<RunningState> runningState{ RunningState::Stop };
};

