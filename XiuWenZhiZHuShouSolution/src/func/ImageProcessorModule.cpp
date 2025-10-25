#include "ImageProcessorModule.hpp"
#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "Utilty.hpp"

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
	imgPro(frame.image);

	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto proResult = imgPro.getProcessResult();

	if (proResult.size()!=0)
	{
		auto body = proResult[0];
		QString message{};
		message += QString("center_x:%1 ").arg(body.center_x);
		message += QString("center_y:%1 ").arg(body.center_y);
		message += QString("angle:%1 ").arg(body.angle);
		Modules::getInstance().communicationModule.broadcastMessage(message);
	}

	

	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);
}

void ImageProcessorHandleScanner::run_OpenRemoveFunc(MatInfo& frame)
{
	auto& configModule = Modules::getInstance().configManagerModule;
	auto& imgProModule = Modules::getInstance().imgProModule;
	auto& runtimeModule = Modules::getInstance().runtimeInfoModule;
	auto& setConfig = configModule.setConfig;
	DefectBox.clear();

	imageCenterX = 0;
	bodyCount = 0;
	_isbad = false;

	auto& imgPro = *_imgProcess;
	imgPro(frame.image);
	// 更新图像中心点X轴
	imageCenterX = frame.image.cols / 2.0;
	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto imageRealLocation = frame.location;

	auto& processResult = imgPro.getProcessResult();

	const auto& context = imgPro.getContext();
	auto currentPixToWorld = std::any_cast<double>(context.customFields.at("CurrentPixToWorld"));

	auto bodyIndexWithDefectsIndexMap = std::any_cast<BodyIndexWithDefectsIndexMap>(context.customFields.at("BodyIndexWithDefectsIndexMap"));


	int centralLimit = -1;

	if (context.customFields.find("CentralLimit") != context.customFields.end()) {
		centralLimit = std::any_cast<int>(context.customFields.at("CentralLimit"));
	}

	for (const auto& bodyWithDefect : bodyIndexWithDefectsIndexMap) {

		// calculate  body loc
		auto& currentHandleResult = processResult[bodyWithDefect.first];
		float centerYPixel = currentHandleResult.center_y * currentPixToWorld;
		bool isInner{ false };
		centerYPixel = imageRealLocation - centerYPixel;

		if (centralLimit!=-1)
		{
			isInner = is_inner(currentHandleResult, centralLimit, imageProcessingModuleIndex);
		}
		else
		{
			isInner = is_inner(currentHandleResult, imageCenterX, imageProcessingModuleIndex);
		}

		BodyInfo bodyInfo;
		bodyInfo.loc = centerYPixel;
		bodyInfo.isInner = isInner;

		// 判断是否为新把手
		bool isNewHandle = true;

		if (1 == imageProcessingModuleIndex)
		{
			QMutexLocker locker(&imgProModule.lastBodyInfos1_Mutex);

			for (const auto& lastInfo : imgProModule.lastBodyInfos1) {
				if (std::abs(bodyInfo.loc - lastInfo.loc) < setConfig.tifeirongyufangdou1 &&
					bodyInfo.isInner == lastInfo.isInner)
				{
					isNewHandle = false;
					break;
				}
			}
			if (isNewHandle) {
				++runtimeModule.statisticalInfo.produceCount;
				imgProModule.lastBodyInfos1.push_back(bodyInfo);

				while (imgProModule.lastBodyInfos1.size() > 50) {
					imgProModule.lastBodyInfos1.erase(imgProModule.lastBodyInfos1.begin());
				}
			}
		}
		else if (2 == imageProcessingModuleIndex)
		{
			QMutexLocker locker(&imgProModule.lastBodyInfos2_Mutex);

			for (const auto& lastInfo : imgProModule.lastBodyInfos2) {
				if (std::abs(bodyInfo.loc - lastInfo.loc) < setConfig.tifeirongyufangdou2
					&& bodyInfo.isInner == lastInfo.isInner)
				{
					isNewHandle = false;
					break;
				}
			}
			if (isNewHandle) {
				++runtimeModule.statisticalInfo.produceCount;
				imgProModule.lastBodyInfos2.push_back(bodyInfo);

				while (imgProModule.lastBodyInfos2.size() > 50) {
					imgProModule.lastBodyInfos2.erase(imgProModule.lastBodyInfos2.begin());
				}
			}
		}


		// calculate defect loc
		if (!bodyWithDefect.second.empty()) {
			if (1 == imageProcessingModuleIndex)
			{
				if (isInner)
				{
					centerYPixel += setConfig.neicetifeijuli1;
				}
				else
				{
					centerYPixel += setConfig.waicetifeijuli1;
				}
			}
			else {
				if (isInner)
				{
					centerYPixel += setConfig.neicetifeijuli2;
				}
				else
				{
					centerYPixel += setConfig.waicetifeijuli2;
				}
			}
			DefectInfoForPriorityQueue defectInfo;
			defectInfo.centerYLoc = centerYPixel;
			defectInfo.isInner = isInner;
			std::cout << "centerYLoc" << static_cast<int>(defectInfo.centerYLoc) << std::endl;
			DefectBox.push_back(defectInfo);

		}
	}


	run_OpenRemoveFunc_emitErrorInfo(defectResult.isBad);

	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);

	if (defectResult.isBad)
	{
		_isbad = true;
	}
	rw::rqw::ImageInfo imageInfo(rw::rqw::cvMatToQImage(frame.image));

	save_image(imageInfo, maskImg);

	auto proResult = imgPro.getProcessResult();

	if (proResult.size() != 0)
	{
		auto body = proResult[0];
		QString message{};
		message += QString("center_x:%1 ").arg(body.center_x);
		message += QString("center_y:%1 ").arg(body.center_y);
		message += QString("angle:%1 ").arg(body.angle);
		Modules::getInstance().communicationModule.broadcastMessage(message);
	}
}

void ImageProcessorHandleScanner::run_OpenRemoveFunc_emitErrorInfo(bool isbad)
{
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;
	auto& priorityQueue2 = Modules::getInstance().eliminateModule.productPriorityQueue2;

	if (isbad)
	{
		for (auto& item : DefectBox)
		{
			if (1 == imageProcessingModuleIndex)
			{
				item.index = 1;
				priorityQueue1->push((item));
			}
			else if (2 == imageProcessingModuleIndex)
			{
				item.index = 2;
				priorityQueue2->push((item));
			}
		}
	}
}

void ImageProcessorHandleScanner::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;

	auto& globalStruct = GlobalData::getInstance();
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	if (!isTakePictures)
	{
		return;
	}

	if ((imageProcessingModuleIndex == 1 && setConfig.takeWork1Pictures) || (imageProcessingModuleIndex == 2 && setConfig.takeWork2Pictures))
	{
		save_image_work(imageInfo, image);
	}
}

void ImageProcessorHandleScanner::save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	if (_isbad) {
		if (setConfig.saveNGImg)
		{
			if (1 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "NG1";
				imageSaveEngine->pushImage(imageInfo);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "NG2";
				imageSaveEngine->pushImage(imageInfo);
			}
		}
		if (setConfig.saveMaskImg)
		{
			rw::rqw::ImageInfo mask(image);
			if (1 == imageProcessingModuleIndex)
			{
				mask.classify = "Mask1";
				imageSaveEngine->pushImage(mask);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				mask.classify = "Mask2";
				imageSaveEngine->pushImage(mask);
			}
		}
	}
	else {
		if (setConfig.saveOKImg)
		{
			if (1 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "OK1";
				imageSaveEngine->pushImage(imageInfo);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "OK2";
				imageSaveEngine->pushImage(imageInfo);
			}
		}
	}
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
	std::string imagePath = R"(C:\Users\rw\Desktop\1\Image_20251024144541273.jpg)"; // 替换为你的图片路径
	cv::Mat frame1 = cv::imread(imagePath, cv::IMREAD_COLOR);
	frame = frame1.clone();
	if (frame.channels() == 4) {
		cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
	}
	if (frame.type() != CV_8UC3) {
		frame.convertTo(frame, CV_8UC3);
	}

	if (frame.empty()) {
		return; // 跳过空帧
	}

	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;


	QMutexLocker locker(&_mutex);
	MatInfo mat;
	mat.image = frame;
	mat.index = index;
	if (index == 1)
	{
		mat.location = zmotion->getModbus(2, 1);	// 获取拍照的位置
	}
	else
	{
		mat.location = zmotion->getModbus(6, 1);	// 获取拍照的位置
	}
	_queue.enqueue(mat);
	_condition.wakeOne();
}
