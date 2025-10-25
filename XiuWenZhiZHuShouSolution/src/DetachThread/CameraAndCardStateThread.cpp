#include "CameraAndCardStateThread.h"

#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "rqw_CameraObjectZMotion.hpp"

size_t CameraAndCardStateThreadHandleScanner::runtimeCounts=0;

CameraAndCardStateThreadHandleScanner::CameraAndCardStateThreadHandleScanner(QObject* parent)
	: QThread(parent), running(false), _dlgProductSet(Modules::getInstance().configManagerModule.setConfig){
}

CameraAndCardStateThreadHandleScanner::~CameraAndCardStateThreadHandleScanner()
{
	stopThread();
	wait();
}

void CameraAndCardStateThreadHandleScanner::startThread()
{
	running = true;
	if (!isRunning()) {
		start();
	}
}

void CameraAndCardStateThreadHandleScanner::stopThread()
{
	running = false;
}

void CameraAndCardStateThreadHandleScanner::run()
{
	while (running) {
		QThread::msleep(2000);
		if (_dlgProductSet.yundongkongzhiqichonglian)
		{
			check_cardState();
		}

		check_cameraState();

		runtimeCounts++;
		if (runtimeCounts == 2) {
			runtimeCounts = 0;
		}
	}
}

void CameraAndCardStateThreadHandleScanner::check_cameraState()
{
	check_cameraState1();
	if (_dlgProductSet.qiyongerxiangji)
	{
	}
}

void CameraAndCardStateThreadHandleScanner::check_cameraState1()
{
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	static bool isUpdateState = false;


	if (runtimeCounts != 0) {
		return;
	}
	if (camera1) {
		if (camera1->getConnectState()) {
			if (!isUpdateState) {
				emit updateCameraLabelState(1, true);
				isUpdateState = true;
			}
		}
		else {
			emit destroyCamera(1);
			emit updateCameraLabelState(1, false);
		}
	}
	else {
		emit buildCamera(1);
		emit startMonitor(1);
		emit updateCameraLabelState(1, false);
		isUpdateState = false;
	}
}

void CameraAndCardStateThreadHandleScanner::check_cardState()
{
	
}
