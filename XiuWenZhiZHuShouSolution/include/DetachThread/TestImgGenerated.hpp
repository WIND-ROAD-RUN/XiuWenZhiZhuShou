#pragma once
#include <qtmetamacros.h>
#include "rqw_CameraObject.hpp"
#include <QThread>
#include <QObject>

class DetachTestImgThread
	: public QThread
{
	Q_OBJECT
public:
	std::atomic_bool isProcessing{ false };
	std::atomic_bool isProcessFinish{ false };
private:
	cv::Mat mat1;
	cv::Mat mat2;
	cv::Mat mat3;
	cv::Mat mat4;
public:
	explicit DetachTestImgThread(QObject* parent = nullptr);

	~DetachTestImgThread() override;

	void startThread();

	void stopThread();
private:
	std::atomic<bool> _running;
signals:
	void frameCaptured(cv::Mat frame, size_t index);
protected:
	void run() override;
};
