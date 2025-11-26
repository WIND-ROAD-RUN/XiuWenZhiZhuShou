#pragma once

#include"ime_ModelEngineFactory.h"

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>
#include <vector>
#include <QThread>
#include <QPixmap>
#include <imgPro_ImageProcess.hpp>
#include "rqw_ImageSaveEngine.h"
#include "Utilty.hpp"

struct MatInfo {
	cv::Mat image;
	float location;	
	size_t index;
};

class ImageProcessorHandleScanner : public QThread
{
	Q_OBJECT

public:
	ImageProcessorHandleScanner(QQueue<MatInfo>& queue,
		QMutex& mutex,
		QWaitCondition& condition,
		int workIndex,
		QObject* parent = nullptr);

protected:
	void run() override;

private:
	void run_debug(MatInfo& frame);			
private:
	void run_OpenRemoveFunc(MatInfo& frame);

private:
	// 存图
	void save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image);
	void save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image);

	void drawCenterPointAndAngle(QImage& maskImg, double angle, int centerX, int centerY);
signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);

	void updateMainWindowShowTXT(const QString message);
private:
	std::unique_ptr<rw::imgPro::ImageProcess> _imgProcess;
public:
	void buildDetModelEngine(const QString& enginePath);	

private:
	// 判断是否有缺陷
	bool _isbad{ false };

	QString lastSaveImgRoot{};

private:
	QQueue<MatInfo>& _queue;
	QMutex& _mutex;
	QWaitCondition& _condition;
	int _workIndex;
public:
	int imageProcessingModuleIndex;
};


class ImageProcessingModuleHandleScanner : public QObject {
	Q_OBJECT
public:
	QString modelEnginePath;
public:
	void BuildModule();
public:
	ImageProcessingModuleHandleScanner(int numConsumers, QObject* parent = nullptr);

	~ImageProcessingModuleHandleScanner();

public slots:
	void onFrameCaptured(cv::Mat frame, size_t index);

signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);

	void updateMainWindowShowTXT(const QString message);
public:
	std::vector<ImageProcessorHandleScanner*> getProcessors() const {
		return _processors;
	}

private:
	QQueue<MatInfo> _queue;
	QMutex _mutex;
	QWaitCondition _condition;
	std::vector<ImageProcessorHandleScanner*> _processors;
	int _numConsumers;
public:
	size_t index;
};


