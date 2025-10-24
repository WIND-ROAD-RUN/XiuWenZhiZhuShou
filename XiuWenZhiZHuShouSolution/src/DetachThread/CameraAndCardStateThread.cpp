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
		check_cameraState2();
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

void CameraAndCardStateThreadHandleScanner::check_cameraState2()
{
	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	static bool isUpdateSate = false;

	if (runtimeCounts != 1) {
		return;
	}

	if (camera2) {
		if (camera2->getConnectState()) {
			if (!isUpdateSate) {
				emit updateCameraLabelState(2, true);
				isUpdateSate = true;
			}
		}
		else {
			emit destroyCamera(2);
			emit updateCameraLabelState(2, false);
		}
	}
	else {
		emit buildCamera(2);
		emit startMonitor(2);
		emit updateCameraLabelState(2, false);
		isUpdateSate = false;
	}
}

void CameraAndCardStateThreadHandleScanner::check_cardState()
{
	
}
