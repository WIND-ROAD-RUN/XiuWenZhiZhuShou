#include "TestImgGenerated.hpp"

#include "GlobalStruct.hpp"

DetachTestImgThread::DetachTestImgThread(QObject* parent)
	: QThread(parent), _running(false) {
	mat1 = cv::imread(R"(C:\Users\zfkj4090\Desktop\TestImg\shijin\Image_20250730144219347_30.jpg)");
}

DetachTestImgThread::~DetachTestImgThread()
{
	stopThread();
	wait();
}

void DetachTestImgThread::startThread()
{
	_running = true;
	if (!isRunning()) {
		start(); // 启动线程
	}
}

void DetachTestImgThread::stopThread()
{
	_running = false; // 停止线程
}

void DetachTestImgThread::run()
{
	while (_running) {
		QThread::msleep(500);
		emit frameCaptured(mat1, 1);
	}
}