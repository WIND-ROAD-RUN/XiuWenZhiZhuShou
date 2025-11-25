#include "ImageProcessorModule.hpp"
#include "Modules.hpp"
#include "Utilty.hpp"
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

bool is_inner(const rw::DetectionRectangleInfo& info, int imgCentralX, int imgIndex)
{
	if (1 == imgIndex)
	{
		if (info.center_x < imgCentralX)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		if (info.center_x > imgCentralX)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

ImageProcessorHandleScanner::ImageProcessorHandleScanner(QQueue<MatInfo>& queue, QMutex& mutex, QWaitCondition& condition, int workIndex, QObject* parent)
	: QThread(parent), _queue(queue), _mutex(mutex), _condition(condition), _workIndex(workIndex)
{

}

void ImageProcessorHandleScanner::run()
{
	while (!QThread::currentThread()->isInterruptionRequested()) {
		MatInfo frame;
		{
			QMutexLocker locker(&_mutex);
			if (_queue.isEmpty()) {
				_condition.wait(&_mutex);
				if (QThread::currentThread()->isInterruptionRequested()) {
					break;
				}
			}
			if (!_queue.isEmpty()) {
				frame = _queue.dequeue();
			}
			else {
				continue; // 如果队列仍为空，跳过本次循环
			}
		}

		// 检查 frame 是否有效
		if (frame.image.empty()) {
			continue; // 跳过空帧
		}

		auto currentRunningState = Modules::getInstance().runtimeInfoModule.runningState.load();
		switch (currentRunningState)
		{
		case RunningState::Debug:
			run_debug(frame);
			break;
		case RunningState::OpenRemoveFunc:
			run_OpenRemoveFunc(frame);
			break;
		default:
			break;
		}
	}
}

void ImageProcessorHandleScanner::run_debug(MatInfo& frame)
{
	auto& imgPro = *_imgProcess;

	// 构建临时调试图片存储路径
	QString tempDir = "D:\\zfkjData\\XiuWenZhiZHuShouSolution\\TempDebugImages";
	QDir dir(tempDir);
	if (!dir.exists())
	{
		dir.mkpath(tempDir);  // 创建目录(如果不存在)
	}

	// 使用固定文件名，每次覆盖，避免文件积累
	QString tempImagePath = QString("%1\\Debug_Worker_%2.jpg")
		.arg(tempDir)
		.arg(_workIndex);

	// 保存原始图片
	bool saveSuccess = cv::imwrite(tempImagePath.toStdString(), frame.image);
	if (saveSuccess)
	{
		//qDebug() << "Debug image saved to:" << tempImagePath;

		// 立即读取验证
		cv::Mat reloadedImage = cv::imread(tempImagePath.toStdString(), cv::IMREAD_COLOR);
		if (!reloadedImage.empty())
		{
			//qDebug() << "Successfully reloaded image, size:" << reloadedImage.rows << "x" << reloadedImage.cols;
			frame.image = reloadedImage.clone();
		}
		else
		{
			//qWarning() << "Failed to reload image from:" << tempImagePath;
		}
	}
	else
	{
		//qWarning() << "Failed to save debug image to:" << tempImagePath;
	}

	imgPro(frame.image);

	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto proResult = imgPro.getProcessResult();

	if (proResult.size() != 0)
	{
		auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

		QString payload;
		bool hasValidData = false;

		for (size_t i = 0; i < proResult.size(); ++i) {
			auto body = proResult[i];
			auto& xiangsudangliang = mainWindowConfig.xiangsudangliang;
			body.center_x = body.center_x * xiangsudangliang;
			body.center_y = body.center_y * xiangsudangliang;

			auto area = body.area * xiangsudangliang * xiangsudangliang;

			if (area < mainWindowConfig.xiandingtiji * 100)
			{
				break;
			}

			if (!hasValidData)
			{
				payload += QString("Image\n");
				hasValidData = true;
			}

			payload += QString("[X:%1;").arg(body.center_x);
			payload += QString("Y:%1;").arg(body.center_y);
			payload += QString("A:%1;").arg(body.angle * 360);
			payload += QString("ATTR:0;");
			payload += QString("ID:0]\n");
		}

		if (hasValidData)
		{
			payload += QString("Done\n");

			Modules::getInstance().communicationModule.broadcastMessage(payload);

			const auto timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
			const QString uiMessage = QString("[%1] ").arg(timestamp) + payload;
			emit updateMainWindowShowTXT(uiMessage);
		}
	}
	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);

	/*rw::rqw::ImageInfo imageInfo(rw::rqw::cvMatToQImage(frame.image));

	save_image(imageInfo, rw::rqw::cvMatToQImage(frame.image));*/
}

void ImageProcessorHandleScanner::run_OpenRemoveFunc(MatInfo& frame)
{
	auto& imgPro = *_imgProcess;

	// 构建临时调试图片存储路径
	QString tempDir = "D:\\zfkjData\\XiuWenZhiZHuShouSolution\\TempDebugImages";
	QDir dir(tempDir);
	if (!dir.exists())
	{
		dir.mkpath(tempDir);  // 创建目录(如果不存在)
	}

	// 使用固定文件名，每次覆盖，避免文件积累
	QString tempImagePath = QString("%1\\Debug_Worker_%2.jpg")
		.arg(tempDir)
		.arg(_workIndex);

	// 保存原始图片
	bool saveSuccess = cv::imwrite(tempImagePath.toStdString(), frame.image);
	if (saveSuccess)
	{
		//qDebug() << "Debug image saved to:" << tempImagePath;

		// 立即读取验证
		cv::Mat reloadedImage = cv::imread(tempImagePath.toStdString(), cv::IMREAD_COLOR);
		if (!reloadedImage.empty())
		{
			//qDebug() << "Successfully reloaded image, size:" << reloadedImage.rows << "x" << reloadedImage.cols;
			frame.image = reloadedImage.clone();
		}
		else
		{
			//qWarning() << "Failed to reload image from:" << tempImagePath;
		}
	}
	else
	{
		//qWarning() << "Failed to save debug image to:" << tempImagePath;
	}

	imgPro(frame.image);

	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto proResult = imgPro.getProcessResult();

	if (proResult.size() != 0)
	{
		auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

		QString payload;
		bool hasValidData = false;

		for (size_t i = 0; i < proResult.size(); ++i) {
			auto body = proResult[i];
			auto& xiangsudangliang = mainWindowConfig.xiangsudangliang;
			body.center_x = body.center_x * xiangsudangliang;
			body.center_y = body.center_y * xiangsudangliang;

			auto area = body.area * xiangsudangliang * xiangsudangliang;

			if (area < mainWindowConfig.xiandingtiji * 100)
			{
				break;
			}

			if (!hasValidData)
			{
				payload += QString("Image\n");
				hasValidData = true;
			}

			payload += QString("[X:%1;").arg(body.center_x);
			payload += QString("Y:%1;").arg(body.center_y);
			payload += QString("A:%1;").arg(body.angle * 360);
			payload += QString("ATTR:0;");
			payload += QString("ID:0]\n");
		}

		if (hasValidData)
		{
			payload += QString("Done\n");

			Modules::getInstance().communicationModule.broadcastMessage(payload);

			const auto timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
			const QString uiMessage = QString("[%1] ").arg(timestamp) + payload;
			emit updateMainWindowShowTXT(uiMessage);
		}
	}

	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);

	/*rw::rqw::ImageInfo imageInfo(maskImg);

	save_image(imageInfo, rw::rqw::cvMatToQImage(frame.image));*/
}

void ImageProcessorHandleScanner::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;
	if (mainWindowConfig.isSaveImg)
	{
		save_image_work(imageInfo, image);
	}
}

void ImageProcessorHandleScanner::save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;

	rw::rqw::ImageInfo Ok(image);
	Ok.classify = "OK";
	imageSaveEngine->pushImage(Ok);
}

void ImageProcessorHandleScanner::buildDetModelEngine(const QString& enginePath)
{
	rw::ModelEngineConfig modelEngineConfig;
	modelEngineConfig.conf_threshold = 0.1f;
	modelEngineConfig.nms_threshold = 0.1f;
	modelEngineConfig.imagePretreatmentPolicy = rw::ImagePretreatmentPolicy::LetterBox;
	modelEngineConfig.letterBoxColor = cv::Scalar(114, 114, 114);
	modelEngineConfig.modelPath = enginePath.toStdString();
	auto engine = rw::ModelEngineFactory::createModelEngine(modelEngineConfig, rw::ModelType::Yolov11_Obb, rw::ModelEngineDeployType::TensorRT);
	_imgProcess = std::make_unique<rw::imgPro::ImageProcess>(engine);
	_imgProcess->context() = Modules::getInstance().imgProModule.imageProcessContext_Main;
	_imgProcess->context().customFields["ImgProcessIndex"] = static_cast<int>(imageProcessingModuleIndex);
	_imgProcess->context().customFields["stationIdx"] = static_cast<int>(_workIndex);
}

void ImageProcessingModuleHandleScanner::BuildModule()
{
	for (int i = 0; i < _numConsumers; ++i) {
		static size_t workIndexCount = 0;
		ImageProcessorHandleScanner* processor = new ImageProcessorHandleScanner(_queue, _mutex, _condition, workIndexCount, this);
		workIndexCount++;
		processor->imageProcessingModuleIndex = index;
		processor->buildDetModelEngine(modelEnginePath);
		connect(processor, &ImageProcessorHandleScanner::imageNGReady, this, &ImageProcessingModuleHandleScanner::imageNGReady, Qt::QueuedConnection);
		connect(processor, &ImageProcessorHandleScanner::updateMainWindowShowTXT, this, &ImageProcessingModuleHandleScanner::updateMainWindowShowTXT, Qt::QueuedConnection);
		_processors.push_back(processor);
		processor->start();
	}
}

ImageProcessingModuleHandleScanner::ImageProcessingModuleHandleScanner(int numConsumers, QObject* parent)
	: QObject(parent), _numConsumers(numConsumers)
{

}

ImageProcessingModuleHandleScanner::~ImageProcessingModuleHandleScanner()
{
	// 通知所有线程退出
	for (auto processor : _processors) {
		processor->requestInterruption();
	}

	// 唤醒所有等待的线程
	{
		QMutexLocker locker(&_mutex);
		_condition.wakeAll();
	}

	// 等待所有线程退出
	for (auto processor : _processors) {
		if (processor->isRunning()) {
			processor->wait(1000); // 使用超时机制，等待1秒
		}
		delete processor;
	}
}

void ImageProcessingModuleHandleScanner::onFrameCaptured(cv::Mat frame, size_t index)
{
	//手动读取本地图片
	//std::string imagePath = R"(C:\Users\zfkj4090\Desktop\xiuwenzhizhushouOBB\train\images\Image_20251024144247939.jpg)"; // 替换为你的图片路径
	//cv::Mat frame1 = cv::imread(imagePath, cv::IMREAD_COLOR);
	//frame = frame1.clone();
	if (frame.channels() == 4) {
		cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
	}
	if (frame.type() != CV_8UC3) {
		frame.convertTo(frame, CV_8UC3);
	}

	if (frame.empty()) {
		return; // 跳过空帧
	}

	QMutexLocker locker(&_mutex);
	MatInfo mat;
	mat.image = frame;
	mat.index = index;
	_queue.enqueue(mat);
	_condition.wakeOne();
}
