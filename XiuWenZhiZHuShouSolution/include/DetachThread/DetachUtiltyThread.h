#pragma once

#include <QThread>
#include <atomic>
#include"rqw_LabelWarning.h"

class DetachUtiltyThread : public QThread
{
	Q_OBJECT
public:
	explicit DetachUtiltyThread(QObject* parent = nullptr);

	~DetachUtiltyThread() override;

	void startThread();

	void stopThread();


protected:
	void run() override;
private:
	void CalculateRealtimeInformation(size_t s);
	void CalculateIsProduceCountReachingTheSetValue(size_t s);
signals:
	void updateStatisticalInfo();
	void produceCountReachingTheSetValue();
private:
	int lastProduceCount{ 0 };
private:
	std::atomic<bool> running; // 使用原子变量保证线程安全
};
