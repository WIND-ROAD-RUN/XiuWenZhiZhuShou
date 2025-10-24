#include "DetachDefectThread.h"
#include "DetachDefectThread.h"
#include "DetachDefectThread.h"
#include "GlobalStruct.hpp"
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
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;

	DefectInfoForPriorityQueue info{};

	int minlocation = 0;

	auto isGet = queue->tryGetTop(info);
	minlocation = info.centerYLoc;

	if (!isGet)
	{
		return;
	}

	if (std::abs(lastDefectLoc1 - minlocation) < setConfig.tifeirongyufangdou1) {
		DefectInfoForPriorityQueue temp{};
		queue->tryPopTop(temp);
		return;
	}

	float diffValue{ 0 };

	diffValue = loc - std::abs(minlocation);

	if (diffValue >= 0) {
		queue->tryPopTop(info);
		lastDefectLoc1 = info.centerYLoc;
		std::cout << "----emit defect signal1 ,loc:-----" << static_cast<int>(lastDefectLoc1) << std::endl;
		// ++wasteCount when defect
		++Modules::getInstance().runtimeInfoModule.statisticalInfo.wasteCount;
		if (info.isInner)
		{
			zmotion->setIOOut(ControlLines::DOxiachaqigangnei1, true);
			QThread::msleep(setConfig.yanshixiachanei1);
			zmotion->setIOOut(ControlLines::DOboliaoqigangnei1, true);
			QThread::msleep(setConfig.boliaoshijiannei1);
			zmotion->setIOOut(ControlLines::DOxiachaqigangnei1, false);
			QThread::msleep(setConfig.yanshixiachanei1);
			zmotion->setIOOut(ControlLines::DOboliaoqigangnei1, false);
			QThread::msleep(setConfig.boliaoshijiannei1);
		}
		else
		{
			zmotion->setIOOut(ControlLines::DOxiachaqigangwai1, true);
			QThread::msleep(setConfig.yanshixiachawai1);
			zmotion->setIOOut(ControlLines::DOboliaoqigangwai1, true);
			QThread::msleep(setConfig.boliaoshijianwai1);
			zmotion->setIOOut(ControlLines::DOxiachaqigangwai1, false);
			QThread::msleep(setConfig.yanshixiachawai1);
			zmotion->setIOOut(ControlLines::DOboliaoqigangwai1, false);
			QThread::msleep(setConfig.boliaoshijianwai1);
		}
	}
}

void DetachDefectThreadHandleScanner::processQueue2(std::unique_ptr<rw::dsl::ThreadSafeHeap<DefectInfoForPriorityQueue>>& queue, float loc)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	DefectInfoForPriorityQueue info{};

	int minlocation = 0;

	auto isGet = queue->tryGetTop(info);
	minlocation = info.centerYLoc;

	if (!isGet)
	{
		return;
	}

	if (std::abs(lastDefectLoc2 - minlocation) < setConfig.tifeirongyufangdou2) {
		DefectInfoForPriorityQueue temp{};
		queue->tryPopTop(temp);
		//std::cout << "loc" << static_cast<int>(temp.centerYLoc) << std::endl;
		return;
	}

	float diffValue{0};

	if (minlocation<0) {
		diffValue = loc + minlocation;
	}
	else {
		diffValue = loc - minlocation;
	}

	if (diffValue >= 0) {
		queue->tryPopTop(info);
		lastDefectLoc2 = info.centerYLoc;
		// ++wasteCount when defect
		
		std::cout << "----emit defect signal2 ,loc:-----" << static_cast<int>(lastDefectLoc2) << std::endl;
		++Modules::getInstance().runtimeInfoModule.statisticalInfo.wasteCount;
		if (info.isInner)
		{
			zmotion->setIOOut(ControlLines::DOxiachaqigangnei2, true);
			QThread::msleep(setConfig.yanshixiachanei2);
			zmotion->setIOOut(ControlLines::DOboliaoqigangnei2, true);
			QThread::msleep(setConfig.boliaoshijiannei2);
			zmotion->setIOOut(ControlLines::DOxiachaqigangnei2, false);
			QThread::msleep(setConfig.yanshixiachanei2);
			zmotion->setIOOut(ControlLines::DOboliaoqigangnei2, false);
			QThread::msleep(setConfig.boliaoshijiannei2);
		}
		else
		{
			zmotion->setIOOut(ControlLines::DOxiachaqigangwai2, true);
			QThread::msleep(setConfig.yanshixiachawai2);
			zmotion->setIOOut(ControlLines::DOboliaoqigangwai2, true);
			QThread::msleep(setConfig.boliaoshijianwai2);
			zmotion->setIOOut(ControlLines::DOxiachaqigangwai2, false);
			QThread::msleep(setConfig.yanshixiachawai2);
			zmotion->setIOOut(ControlLines::DOboliaoqigangwai2, false);
			QThread::msleep(setConfig.boliaoshijianwai2);
		}
	}
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
