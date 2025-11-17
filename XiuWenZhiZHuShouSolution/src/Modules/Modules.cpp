#include "Modules.hpp"

#include <QMessageBox>

#include "XiuWenZhiZHuShou.h"
#include "rqw_RunEnvCheck.hpp"

Modules::Modules()
{

}

Modules::~Modules()
{

}

bool Modules::build()
{
	// 读取配置
	auto configManagerBuild = configManagerModule.build();

	auto communciationModule = communicationModule.build();

	// 构建相机
	auto cameraBuild = cameraModule.build();

	// 构建显示信息模块
	auto runtimeInfoModuleBuild = runtimeInfoModule.build();

	// 构建图像处理模块
	auto imgProModuleBuild = imgProModule.build();

	// 构建存图模块
	imgSaveModule.build();

	// 构建UI模块
	uiModule.build();

	// 构建重连模块
	reconnectModule.build();

#ifdef BUILD_WITHOUT_HARDWARE
	test_module.build();
#endif
	

	return true;
}

void Modules::destroy()
{
	

#ifdef BUILD_WITHOUT_HARDWARE
	test_module.destroy();
#endif
	runtimeInfoModule.destroy();
	imgSaveModule.destroy();
	imgProModule.destroy();
	cameraModule.destroy();
	configManagerModule.destroy();
	uiModule.destroy();
	reconnectModule.destroy();
	communicationModule.destroy();
}

void Modules::start()
{
	uiModule.start();
	configManagerModule.start();
	communicationModule.start();
	runtimeInfoModule.start();
	imgProModule.start();
	imgSaveModule.start();
	cameraModule.start();
	reconnectModule.start();

#ifdef BUILD_WITHOUT_HARDWARE
	test_module.start();
#endif
}

void Modules::stop()
{
#ifdef BUILD_WITHOUT_HARDWARE
	test_module.stop();
#endif
	reconnectModule.stop();
	cameraModule.stop();
	imgSaveModule.stop();
	imgProModule.stop();
	runtimeInfoModule.stop();
	configManagerModule.stop();
	uiModule.stop();
	communicationModule.stop();
}

void Modules::connect()
{
#pragma region connect camera and imgProModule

	QObject::connect(&cameraModule, &CameraModule::frameCaptured1,
		imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleHandleScanner::onFrameCaptured, Qt::DirectConnection);

#pragma endregion

#pragma region connect UIModule and imgProModule
	QObject::connect(imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleHandleScanner::imageNGReady, uiModule._handleScanner, &HandleScanner::onCameraNGDisplay);
	QObject::connect(imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleHandleScanner::updateMainWindowShowTXT, uiModule._handleScanner, &HandleScanner::appendTcpLog);
#pragma endregion

#pragma region connect UIModule and ReconnectModule
	// 更新UI界面
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadHandleScanner::updateCameraLabelState,
		uiModule._handleScanner, &HandleScanner::updateCameraLabelState, Qt::QueuedConnection);
	// 相机重连
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadHandleScanner::buildCamera,
		&cameraModule, &CameraModule::onBuildCamera, Qt::QueuedConnection);
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadHandleScanner::startMonitor,
		&cameraModule, &CameraModule::onStartCamera, Qt::QueuedConnection);
	// 相机销毁
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadHandleScanner::destroyCamera,
		&cameraModule, &CameraModule::onDestroyCamera, Qt::QueuedConnection);

#pragma endregion

#ifdef BUILD_WITHOUT_HARDWARE
	QObject::connect(test_module.testImgPushThread.get(), &TestImgPushThread::imgReady,
			imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleHandleScanner::onFrameCaptured, Qt::DirectConnection);
	/*QObject::connect(test_module.testImgPushThread.get(), &TestImgPushThread::imgReady,
		imgProModule.imageProcessingModule2.get(), &ImageProcessingModuleHandleScanner::onFrameCaptured, Qt::DirectConnection);*/
#endif

#pragma region connect UIModule and CommunicationModule
	QObject::connect(&communicationModule, &CommunicationModule::updateMainwindowUi,
		uiModule._handleScanner, &HandleScanner::updateCameraLabelState);
#pragma endregion

}

bool Modules::check()
{
#pragma region check single instance
	if (!rw::rqw::RunEnvCheck::isSingleInstance("XiuWenZhiZHuShou.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "已经有程序在运行，请勿多次打开");
		return false;
	}
#pragma endregion

#pragma region check run env
	if (rw::rqw::RunEnvCheck::isProcessRunning("MVS.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "检测到海康威视软件正在运行，请先关闭后再启动本程序。");
		return false;
	}

	if (rw::rqw::RunEnvCheck::isProcessRunning("BasedCam3.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "检测到度申相机平台软件正在运行，请先关闭后再启动本程序。");
		return false;
	}

#pragma endregion

#pragma region check directory exist
	EnsureDirectoryExists(globalPath.projectHome);
	EnsureDirectoryExists(globalPath.configRootPath);
	EnsureDirectoryExists(globalPath.modelRootPath);
	EnsureDirectoryExists(globalPath.txtPath);
	EnsureDirectoryExists(globalPath.imageSaveRootPath);
#pragma endregion

#pragma region check model exist
	if (!rw::rqw::RunEnvCheck::isFileExist(globalPath.modelPath))
	{
		QMessageBox::warning(nullptr, "错误", "模型文件缺失");
		return false;
	}
#pragma endregion

#pragma region check config format and exist
	rw::oso::StorageContext storageContext(rw::oso::StorageType::Xml);

	checkFileExistAndFormat<cdm::MainWindowConfig>(globalPath.MainWindowConfigPath, storageContext);
#pragma endregion

	return true;
}

bool Modules::EnsureDirectoryExists(const QString& dirPath)
{
	QDir dir(dirPath);
	if (!dir.exists()) {
		return dir.mkpath(".");
	}
	return true;
}
