#include "ReconnectModule.hpp"

void ReconnectModule::build()
{
	monitorCameraAndCardStateThread = std::make_unique<CameraAndCardStateThreadHandleScanner>();
}

void ReconnectModule::destroy()
{
	monitorCameraAndCardStateThread.reset();
}

void ReconnectModule::start()
{
	if (monitorCameraAndCardStateThread)
	{
		monitorCameraAndCardStateThread->startThread();
	}
}

void ReconnectModule::stop()
{
	if (monitorCameraAndCardStateThread)
	{
		monitorCameraAndCardStateThread->stopThread();
	}
}
