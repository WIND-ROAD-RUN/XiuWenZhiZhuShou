#pragma once

#include <atomic>
#include<QThread>

#include"rqw_LabelWarning.h"
#include"dsl_Heap.hpp"

#include"Utilty.hpp"


class DetachDefectThreadHandleScanner : public QThread
{
	Q_OBJECT
public:
	std::atomic_bool isProcessing{ false };
	std::atomic_bool isProcessFinish{ false };
public:
	explicit DetachDefectThreadHandleScanner(QObject* parent = nullptr);

	~DetachDefectThreadHandleScanner() override;

	void startThread();

	void stopThread();

	void processQueue1(std::unique_ptr<rw::dsl::ThreadSafeHeap<DefectInfoForPriorityQueue>>& queue,float loc);
	void processQueue2(std::unique_ptr<rw::dsl::ThreadSafeHeap<DefectInfoForPriorityQueue>>& queue,float loc);
protected:
	void run() override;
private:
	std::atomic<bool> running; 
private:
	float lastDefectLoc1{ 0 };
	float lastDefectLoc2{ 2 };
};
