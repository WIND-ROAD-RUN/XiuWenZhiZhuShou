#include "DetachDefectThread.h"
#include <chrono>

#include "Modules.hpp"

DetachDefectThreadHandleScanner::DetachDefectThreadHandleScanner(QObject* parent)
{

}

DetachDefectThreadHandleScanner::~DetachDefectThreadHandleScanner()
{
	stopThread();
	wait(); // 等待线程安全退出
}

void DetachDefectThreadHandleScanner::startThread()
{
	running = true;
	if (!isRunning()) {
		start(); // 启动线程
	}
}

void DetachDefectThreadHandleScanner::stopThread()
{
	running = false; // 停止线程
}

void DetachDefectThreadHandleScanner::processQueue1(std::unique_ptr<rw::dsl::ThreadSafeHeap<DefectInfoForPriorityQueue>>& queue, float loc)
{
	
}

void DetachDefectThreadHandleScanner::processQueue2(std::unique_ptr<rw::dsl::ThreadSafeHeap<DefectInfoForPriorityQueue>>& queue, float loc)
{
	
}

void DetachDefectThreadHandleScanner::run()
{
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;
	auto& priorityQueue2 = Modules::getInstance().eliminateModule.productPriorityQueue2;

	while (running) {
		QThread::msleep(1);
		auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
		bool isget = false;
		auto nowLocation = static_cast<int>(abs(zmotion->getAxisLocation(Modbus::LocationGet, isget)));
		if (isget) {
			processQueue1(priorityQueue1, nowLocation);
			processQueue2(priorityQueue2, nowLocation);
		}
	}
}
