#include"DetachUtiltyThread.h"

#include"GlobalStruct.hpp"
#include "Modules.hpp"

DetachUtiltyThread::DetachUtiltyThread(QObject* parent)
	: QThread(parent), running(false) {
}

DetachUtiltyThread::~DetachUtiltyThread()
{
	stopThread();
	wait(); // 等待线程安全退出
}

void DetachUtiltyThread::startThread()
{
	running = true;
	if (!isRunning()) {
		start(); // 启动线程
	}
}

void DetachUtiltyThread::stopThread()
{
	running = false; // 停止线程
}

void DetachUtiltyThread::run()
{
	static size_t s = 0;
	while (running) {
		QThread::sleep(1);
		CalculateRealtimeInformation(s);
		CalculateIsProduceCountReachingTheSetValue(s);
		++s;
		if (s == 300)
		{
			s = 0;
		}
	}
}

void DetachUtiltyThread::CalculateRealtimeInformation(size_t s)
{
	auto& runtimeModule = Modules::getInstance().runtimeInfoModule;
	auto& statisticalInfo = runtimeModule.statisticalInfo;

	// 计算生产良率
	auto totalCount = statisticalInfo.produceCount.load();
	auto wasteCount = statisticalInfo.wasteCount.load();
	if (totalCount != 0)
	{
		if (totalCount > wasteCount)
		{
			statisticalInfo.productionYield = (static_cast<double>(totalCount - wasteCount) / totalCount) * 100;
		}
	}

	emit updateStatisticalInfo();
}

void DetachUtiltyThread::CalculateIsProduceCountReachingTheSetValue(size_t s)
{
	auto& nowProduceCount = Modules::getInstance().runtimeInfoModule.statisticalInfo.produceCount;
	auto& tingjigeshu = Modules::getInstance().configManagerModule.handleScannerConfig.tingjigeshu;

	if (nowProduceCount >= lastProduceCount)
	{
		if (nowProduceCount >= tingjigeshu && lastProduceCount < tingjigeshu)
		{
			emit produceCountReachingTheSetValue();
		}
	}

	lastProduceCount = nowProduceCount.load();
}
