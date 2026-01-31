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

#pragma region DebugSaveAndReload
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
#pragma endregion

	imgPro(frame.image);

	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto proResult = imgPro.getProcessResult();
	auto indexMap = imgPro.getProcessResultIndexMap();

	if (!proResult.empty())
	{
		auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

		QString payload;
		bool hasValidData = false;

		auto itBody = indexMap.find(ClassId::body);
		if (itBody != indexMap.end() && !itBody->second.empty())
		{
			const auto& bodyIndexSet = itBody->second;

			for (const auto idx : bodyIndexSet)
			{
				if (idx >= proResult.size()) continue;

				auto body = proResult[idx];
				auto& xiangsudangliang = mainWindowConfig.xiangsudangliang;
				int drawCenter_X = body.center_x;
				int drawCenter_Y = body.center_y;

				// 坐标放缩并把角度从弧度转为度
				body.center_x = static_cast<int>(body.center_x * xiangsudangliang);
				body.center_y = static_cast<int>(body.center_y * xiangsudangliang);
				body.angle = body.angle * 180.0 / CV_PI;

				// 如果箭头指向长边则旋转90度使其指向短边
				if (body.height > body.width) {
					body.angle += 90.0;
					if (body.angle >= 360.0) body.angle -= 360.0;
				}

				auto area = body.area * xiangsudangliang * xiangsudangliang;

				if (area < mainWindowConfig.xiandingtiji * 100) {
					continue;
				}

				drawCenterPointAndAngle(maskImg, body.angle, drawCenter_X, drawCenter_Y);

				if (!hasValidData)
				{
					payload += QString("Image\n");
					hasValidData = true;
				}

				payload += QString("[X:%1;").arg(body.center_x);
				payload += QString("Y:%1;").arg(body.center_y);
				payload += QString("A:%1;").arg(body.angle);
				payload += QString("ATTR:0;");
				payload += QString("ID:0]\n");
			}
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

	rw::rqw::ImageInfo imageInfo(rw::rqw::cvMatToQImage(frame.image));

	save_image(imageInfo, rw::rqw::cvMatToQImage(frame.image));
}

void ImageProcessorHandleScanner::run_OpenRemoveFunc(MatInfo& frame)
{
	auto& imgPro = *_imgProcess;

#pragma region DebugSaveAndReload
	//// 构建临时调试图片存储路径
	//QString tempDir = "D:\\zfkjData\\XiuWenZhiZHuShouSolution\\TempDebugImages";
	//QDir dir(tempDir);
	//if (!dir.exists())
	//{
	//	dir.mkpath(tempDir);  // 创建目录(如果不存在)
	//}

	//// 使用固定文件名，每次覆盖，避免文件积累
	//QString tempImagePath = QString("%1\\Debug_Worker_%2.jpg")
	//	.arg(tempDir)
	//	.arg(_workIndex);

	//// 保存原始图片
	//bool saveSuccess = cv::imwrite(tempImagePath.toStdString(), frame.image);
	//if (saveSuccess)
	//{
	//	//qDebug() << "Debug image saved to:" << tempImagePath;

	//	// 立即读取验证
	//	cv::Mat reloadedImage = cv::imread(tempImagePath.toStdString(), cv::IMREAD_COLOR);
	//	if (!reloadedImage.empty())
	//	{
	//		//qDebug() << "Successfully reloaded image, size:" << reloadedImage.rows << "x" << reloadedImage.cols;
	//		frame.image = reloadedImage.clone();
	//	}
	//	else
	//	{
	//		//qWarning() << "Failed to reload image from:" << tempImagePath;
	//	}
	//}
	//else
	//{
	//	//qWarning() << "Failed to save debug image to:" << tempImagePath;
	//}
#pragma endregion

	imgPro(frame.image);

	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto proResult = imgPro.getProcessResult();
	auto indexMap = imgPro.getProcessResultIndexMap();

	if (!proResult.empty())
	{
		auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

		QString payload;
		bool hasValidData = false;

		auto itBody = indexMap.find(ClassId::body);
		if (itBody != indexMap.end() && !itBody->second.empty())
		{
			const auto& bodyIndexSet = itBody->second;

			for (const auto idx : bodyIndexSet)
			{
				if (idx >= proResult.size()) continue;

				auto body = proResult[idx];
				auto& xiangsudangliang = mainWindowConfig.xiangsudangliang;
				int drawCenter_X = body.center_x;
				int drawCenter_Y = body.center_y;

				// 坐标放缩并把角度从弧度转为度
				body.center_x = static_cast<int>(body.center_x * xiangsudangliang);
				body.center_y = static_cast<int>(body.center_y * xiangsudangliang);
				body.angle = body.angle * 180.0 / CV_PI * -1;

				// 如果箭头指向长边则旋转90度使其指向短边
				if (body.height > body.width) {
					body.angle += 90.0;
					if (body.angle >= 360.0) body.angle -= 360.0;
				}

				auto area = body.area * xiangsudangliang * xiangsudangliang;

				if (area < mainWindowConfig.xiandingtiji * 100) {
					continue;
				}

				drawCenterPointAndAngle(maskImg, body.angle, drawCenter_X, drawCenter_Y);

				if (!hasValidData)
				{
					payload += QString("Image\n");
					hasValidData = true;
				}

				payload += QString("[X:%1;").arg(body.center_x);
				payload += QString("Y:%1;").arg(body.center_y);
				payload += QString("A:%1;").arg(body.angle);
				payload += QString("ATTR:0;");
				payload += QString("ID:0]\n");
			}
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

	rw::rqw::ImageInfo imageInfo(maskImg);

	save_image(imageInfo, rw::rqw::cvMatToQImage(frame.image));
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

void ImageProcessorHandleScanner::drawCenterPointAndAngle(QImage& maskImg, double angle, int centerX, int centerY)
{
	// 确保QImage格式正确
	if (maskImg.format() != QImage::Format_RGB888 &&
		maskImg.format() != QImage::Format_RGBA8888 &&
		maskImg.format() != QImage::Format_RGB32 &&
		maskImg.format() != QImage::Format_ARGB32)
	{
		maskImg = maskImg.convertToFormat(QImage::Format_RGB888);
	}

	// 根据QImage格式创建对应的cv::Mat
	cv::Mat mat;
	if (maskImg.format() == QImage::Format_RGB888)
	{
		mat = cv::Mat(maskImg.height(), maskImg.width(), CV_8UC3,
			maskImg.bits(), maskImg.bytesPerLine());
	}
	else if (maskImg.format() == QImage::Format_RGBA8888 ||
		maskImg.format() == QImage::Format_ARGB32)
	{
		mat = cv::Mat(maskImg.height(), maskImg.width(), CV_8UC4,
			maskImg.bits(), maskImg.bytesPerLine());
	}
	else if (maskImg.format() == QImage::Format_RGB32)
	{
		mat = cv::Mat(maskImg.height(), maskImg.width(), CV_8UC4,
			maskImg.bits(), maskImg.bytesPerLine());
	}

	// 转换为BGR格式（OpenCV标准格式）
	cv::Mat matBGR;
	if (mat.channels() == 4)
	{
		cv::cvtColor(mat, matBGR, cv::COLOR_RGBA2BGR);
	}
	else if (mat.channels() == 3)
	{
		cv::cvtColor(mat, matBGR, cv::COLOR_RGB2BGR);
	}
	else
	{
		matBGR = mat.clone();
	}

	// 绘制中心点 (使用红色圆圈)
	cv::circle(matBGR, cv::Point(centerX, centerY), 5, cv::Scalar(0, 0, 255), -1);
	cv::circle(matBGR, cv::Point(centerX, centerY), 8, cv::Scalar(0, 0, 255), 2);

	// 转换角度：垂直向上为0°，顺时针旋转为正
	// 1. 标准坐标系：0°向右，逆时针为正
	// 2. 您的需求：0°向上，顺时针为正
	// 转换公式：标准角度 = 90° - 您的角度
	double angleInRadians = (90.0 - angle) * CV_PI / 180.0;

	int lineLength = 100;

	// 在图像坐标系中绘制（Y轴向下）
	int endX = centerX + static_cast<int>(lineLength * std::cos(angleInRadians));
	int endY = centerY - static_cast<int>(lineLength * std::sin(angleInRadians));

	// 绘制角度指示线 (使用绿色)
	cv::arrowedLine(matBGR, cv::Point(centerX, centerY), cv::Point(endX, endY),
		cv::Scalar(0, 255, 0), 2, cv::LINE_AA, 0, 0.3);

	// 在中心点附近显示角度文本
	QString angleText = QString::number(angle, 'f', 1) + "°";
	cv::putText(matBGR, angleText.toStdString(),
		cv::Point(centerX + 10, centerY - 10),
		cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 0), 2);

	// 转换回QImage
	cv::Mat matRGB;
	cv::cvtColor(matBGR, matRGB, cv::COLOR_BGR2RGB);
	maskImg = QImage(matRGB.data, matRGB.cols, matRGB.rows,
		matRGB.step, QImage::Format_RGB888).copy();
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

	if (frame.empty()) {
		return; // 跳过空帧
	}

	if (frame.channels() == 4) {
		cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
	}
	if (frame.type() != CV_8UC3) {
		frame.convertTo(frame, CV_8UC3);
	}

	// 强制深拷贝并确保内存连续
	cv::Mat processFrame;
	if (!frame.isContinuous() || frame.step != frame.cols * frame.elemSize()) {
		frame.copyTo(processFrame); // 保证标准步长与连续内存
	}
	else {
		processFrame = frame.clone();
	}

	// 确保标准的步长
	if (processFrame.step != processFrame.cols * processFrame.elemSize()) {
		cv::Mat temp;
		processFrame.copyTo(temp);
		processFrame = temp;
	}

	QMutexLocker locker(&_mutex);
	MatInfo mat;
	mat.image = processFrame;
	mat.index = index;
	_queue.enqueue(mat);
	_condition.wakeOne();
}
