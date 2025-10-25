#include "ui_HandleScanner.h"

#include "HandleScanner.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>

#include "DlgProductScore.h"
#include "DlgProductSet.h"
#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include "rqw_RunEnvCheck.hpp"
#include "RuntimeInfoModule.hpp"
#include "ui_DlgProductScore.h"
#include "ui_DlgProductSet.h"
#include"Utilty.hpp"

#ifdef BUILD_WITHOUT_HARDWARE
void HandleScanner::cBox_testPushImg_checked(bool checked)
{
	if (checked)
	{
		Modules::getInstance().test_module.testImgPush=true;
	}
	else
	{
		Modules::getInstance().test_module.testImgPush = false;
	}
}
#endif

HandleScanner::HandleScanner(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::PunchPressClass())
{
	ui->setupUi(this);

	initializeComponents();

}

HandleScanner::~HandleScanner()
{
	destroyComponents();
	pbtn_stop_clicked();
	Modules::getInstance().stop();
	Modules::getInstance().destroy();
	delete ui;
}



void HandleScanner::build_ui()
{
	build_HandleScannerData();

#ifdef BUILD_WITHOUT_HARDWARE
	cBox_testPushImg = new QCheckBox(this);
	cBox_testPushImg->setText("图像推送状态");
	ui->gBox_infor->layout()->addWidget(cBox_testPushImg);
	QObject::connect(cBox_testPushImg, &QCheckBox::clicked,
		this, &HandleScanner::cBox_testPushImg_checked);
#endif
}

void HandleScanner::build_connect()
{
	connect(ui->pbtn_exit, &QPushButton::clicked, this, &HandleScanner::pbtn_exit_clicked);
	connect(ui->pbtn_set, &QPushButton::clicked, this, &HandleScanner::pbtn_set_clicked);
	connect(ui->pbtn_score, &QPushButton::clicked, this, &HandleScanner::pbtn_score_clicked);
	connect(ui->rbtn_debug, &QRadioButton::toggled, this, &HandleScanner::rbtn_debug_checked);
	connect(ui->rbtn_takePicture, &QRadioButton::toggled, this, &HandleScanner::rbtn_takePicture_checked);
	connect(ui->rbtn_removeFunc, &QRadioButton::toggled, this, &HandleScanner::rbtn_removeFunc_checked);
	connect(ui->ckb_shibiekuang, &QCheckBox::toggled, this, &HandleScanner::ckb_shibiekuang_checked);
	connect(ui->ckb_wenzi, &QCheckBox::toggled, this, &HandleScanner::ckb_wenzi_checked);
	connect(ui->pbtn_openSaveLocation, &QPushButton::clicked, this, &HandleScanner::pbtn_openSaveLocation_clicked);
	connect(ui->pbtn_start, &QPushButton::clicked, this, &HandleScanner::pbtn_start_clicked);
	connect(ui->pbtn_stop, &QPushButton::clicked, this, &HandleScanner::pbtn_stop_clicked);
	connect(ui->rbtn_strongLight, &QRadioButton::toggled, this, &HandleScanner::rbtn_strongLight_checked);
	connect(ui->rbtn_mediumLight, &QRadioButton::toggled, this, &HandleScanner::rbtn_mediumLight_checked);
	connect(ui->rbtn_weakLight, &QRadioButton::toggled, this, &HandleScanner::rbtn_weakLight_checked);
	connect(ui->btn_tingjigeshu, &QPushButton::clicked, this, &HandleScanner::btn_tingjigeshu_clicked);
	connect(ui->pbtn_resetProduct, &QPushButton::clicked, this, &HandleScanner::pbtn_resetProduct_clicked);

	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &HandleScanner::lb_title_clicked);
}

void HandleScanner::build_HandleScannerData()
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;

	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;
	handleScannerConfig.isDebug = false;
	handleScannerConfig.isDefect = true;		// 默认开启剔废
	rbtn_removeFunc_checked(true);
	pbtn_start_clicked();						// 默认开启光源
	handleScannerConfig.isshibiekuang = true;
	handleScannerConfig.iswenzi = true;

	ui->label_produceTotalValue->setText(QString::number(handleScannerConfig.totalProductionVolume));
	ui->label_wasteProductsValue->setText(QString::number(handleScannerConfig.totalDefectiveVolume));
	ui->label_productionYieldValue->setText(QString::number(handleScannerConfig.productionYield));
	ui->rbtn_takePicture->setChecked(handleScannerConfig.isSaveImg);
	ui->rbtn_removeFunc->setChecked(handleScannerConfig.isDefect);
	ui->ckb_shibiekuang->setChecked(handleScannerConfig.isshibiekuang);
	ui->ckb_wenzi->setChecked(handleScannerConfig.iswenzi);
	ui->rbtn_strongLight->setChecked(handleScannerConfig.isqiangguang);
	ui->rbtn_mediumLight->setChecked(handleScannerConfig.iszhongguang);
	ui->rbtn_weakLight->setChecked(handleScannerConfig.isruoguang);

	isTakePictures = ui->rbtn_takePicture->isChecked();

	// 初始化图像查看器
	_picturesViewer = new PictureViewerThumbnails(this);

	// 初始化关闭窗体
	_dlgCloseForm = new DlgCloseForm(this);
	_dlgCloseForm->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);

	ini_clickableTitle();

	// 读取本地company.txt
	loadCompanyMessage();

	// 读取本地存图地址
	loadSaveImgPath();
}

void HandleScanner::ini_clickableTitle()
{
	// 初始化标题label
	clickableTitle = new rw::rqw::ClickableLabel(this);
	auto layoutTitle = ui->groupBox_head->layout();
	layoutTitle->replaceWidget(ui->label_title, clickableTitle);
	delete ui->label_title;
	clickableTitle->setText("把手检测");
	clickableTitle->setStyleSheet("QLabel {font-size: 30px;font-weight: bold;color: rgb(255, 255, 255);padding: 5px 5px;border-bottom: 2px solid #cccccc;}");
}

void HandleScanner::initializeComponents()
{
	build_ui();

	build_ImageEnlargedDisplay();

	build_motion();

	build_camera();

	build_connect();
}

void HandleScanner::destroyComponents()
{
	auto& globalStructData = GlobalData::getInstance();

	destroy_ImageEnlargedDisplay();
}

void HandleScanner::build_camera()
{
	auto& cameraModules = Modules::getInstance().cameraModule;
	auto errors = cameraModules.getBuildResults();
	updateCameraLabelState(1,true);
	updateCameraLabelState(2,true);

	for (const auto& error : errors)
	{
		auto index = static_cast<int>(error);
		updateCameraLabelState(index, false);
	}
}

void HandleScanner::build_motion()
{
	auto& motionControllerModule = Modules::getInstance().motionControllerModule;
	auto& isBuildZmotion = motionControllerModule.isConnectMotion;
	updateCameraLabelState(0, isBuildZmotion);
}

void HandleScanner::build_ImageEnlargedDisplay()
{
	imgDis1 = new rw::rqw::ClickableLabel(this);
	imgDis1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDis2 = new rw::rqw::ClickableLabel(this);
	imgDis2->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg1 = new rw::rqw::ClickableLabel(this);
	imgDisNg1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg2 = new rw::rqw::ClickableLabel(this);
	imgDisNg2->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_1, imgDis1);

	delete ui->label_imgDisplay_1;

	QObject::connect(imgDis1, &rw::rqw::ClickableLabel::clicked
		, this, &HandleScanner::imgDis1_clicked);
	QObject::connect(imgDis2, &rw::rqw::ClickableLabel::clicked
		, this, &HandleScanner::imgDis2_clicked);
	QObject::connect(imgDisNg1, &rw::rqw::ClickableLabel::clicked
		, this, &HandleScanner::imgDisNg1_clicked);
	QObject::connect(imgDisNg2, &rw::rqw::ClickableLabel::clicked
		, this, &HandleScanner::imgDisNg2_clicked);

	_workStationTitleMap = {
		{0,"一号工位"},
		{1,"二号工位"},
		{2,"NG一号工位"},
		{3,"NG二号工位"}
	};

	_imageEnlargedDisplay = new ImageEnlargedDisplay(this);
	_imageEnlargedDisplay->setMonitorValue(&_isImageEnlargedDisplay);
	_imageEnlargedDisplay->setMonitorDisImgIndex(&_currentImageEnlargedDisplayIndex);
	_imageEnlargedDisplay->initWorkStationTitleMap(_workStationTitleMap);
	_imageEnlargedDisplay->setNum(2);
	_imageEnlargedDisplay->show();
	_imageEnlargedDisplay->close();
}

void HandleScanner::destroy_ImageEnlargedDisplay()
{
	if (_imageEnlargedDisplay)
	{
		_imageEnlargedDisplay->close();
		delete _imageEnlargedDisplay;
		_imageEnlargedDisplay = nullptr;
	}
}

void HandleScanner::loadCompanyMessage()
{
	// 可执行程序所在目录
	const QString filePath = globalPath.companyTxtPath;
	QFile file(filePath);
	QString content;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
		in.setCodec("UTF-8");
#endif
		content = in.readAll();
		file.close();
		// 统一换行，去掉首尾空白
		content.replace("\r\n", "\n").trimmed();
	}
	else
	{
		// 文件不存在或打不开时给一个默认占位
		content = "";
	}
	if (ui && ui->label_CompanyInfo)
	{
		// 多行显示
		ui->label_CompanyInfo->setWordWrap(true);
		// 设置文本（trim 再次安全处理）
		ui->label_CompanyInfo->setText(content.trimmed());
		// 全量内容放到 tooltip，便于查看被压缩的部分
		ui->label_CompanyInfo->setToolTip(content.trimmed());
	}
}

void HandleScanner::loadSaveImgPath()
{
	// 可执行程序所在目录
	const QString filePath = globalPath.saveAddressTxtPath;
	QFile file(filePath);
	QString content;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
		in.setCodec("UTF-8");
#endif
		content = in.readAll();
		file.close();
		// 统一换行，去掉首尾空白
		content.replace("\r\n", "\n").trimmed();
		if (!content.isEmpty())
		{
			globalPath.imageSaveRootPath = content.trimmed();
			globalPath.imageSaveRootPath += R"(\)";
			if (!rw::rqw::RunEnvCheck::isFileExist(globalPath.imageSaveRootPath))
			{
				QDir dir;
				dir.mkpath(globalPath.imageSaveRootPath);
			}
		}
	}
	else
	{
		content = globalPath.imageSaveRootPath;
	}
}

void HandleScanner::updateCameraLabelState(int cameraIndex, bool state)
{
	switch (cameraIndex)
	{
	case 0:
		if (state) {
			ui->label_cardState->setText("连接成功");
			ui->label_cardState->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_cardState->setText("连接失败");
			ui->label_cardState->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	case 1:
		if (state) {
			ui->label_camera1State->setText("连接成功");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_camera1State->setText("连接失败");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	case 2:
		if (state) {
			ui->label_camera2State->setText("连接成功");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_camera2State->setText("连接失败");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	default:
		break;
	}
}

void HandleScanner::onUpdateStatisticalInfoUI()
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	ui->label_produceTotalValue->setText(QString::number(statisticalInfo.produceCount.load()));
	ui->label_wasteProductsValue->setText(QString::number(statisticalInfo.wasteCount.load()));
	ui->label_productionYieldValue->setText(QString::number(statisticalInfo.productionYield.load(), 'f', 2) + "%");
}

void HandleScanner::onCameraNGDisplay(QPixmap image, size_t index, bool isbad)
{
	if (index == 1)
	{
		if (!_isImageEnlargedDisplay)
		{
			imgDis1->setPixmap(image.scaled(imgDis1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			if (isbad)
			{
				imgDisNg1->setPixmap(image.scaled(imgDisNg1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
				_lastImageNg1 = image;
			}
		}
		else
		{
			if (_currentImageEnlargedDisplayIndex == 0)
			{
				_imageEnlargedDisplay->setShowImg(image);
			}
			if (isbad && _currentImageEnlargedDisplayIndex == 2)
			{
				_imageEnlargedDisplay->setShowImg(image);
				_lastImageNg1 = image;
			}
		}
		_lastImage1 = image;
	}
	else if (index == 2)
	{
		if (!_isImageEnlargedDisplay)
		{
			imgDis2->setPixmap(image.scaled(imgDis2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			if (isbad)
			{
				imgDisNg2->setPixmap(image.scaled(imgDisNg2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
				_lastImageNg2 = image;
			}
		}
		else
		{
			if (_currentImageEnlargedDisplayIndex == 1)
			{
				_imageEnlargedDisplay->setShowImg(image);
			}
			if (isbad && _currentImageEnlargedDisplayIndex == 3)
			{
				_imageEnlargedDisplay->setShowImg(image);
				_lastImageNg2 = image;
			}
		}
		_lastImage2 = image;
	}
}

void HandleScanner::lb_title_clicked()
{
	if (0 != minimizeCount)
	{
		minimizeCount--;
	}
	else if (0 >= minimizeCount)
	{
		// 最小化主窗体
		this->showMinimized();

		auto& _dlgProductSet = Modules::getInstance().uiModule._dlgProductSet;
		auto& _dlgProductScore = Modules::getInstance().uiModule._dlgProductScore;

		// 最小化所有子窗体（如果已创建且可见）
		if (_dlgProductSet && _dlgProductSet->isVisible())
			_dlgProductSet->showMinimized();
		if (_dlgProductScore && _dlgProductScore->isVisible())
			_dlgProductScore->showMinimized();
		if (_picturesViewer && _picturesViewer->isVisible())
			_picturesViewer->showMinimized();
		if (_imageEnlargedDisplay && _imageEnlargedDisplay->isVisible())
			_imageEnlargedDisplay->showMinimized();

		minimizeCount = 3; // 重置最小化计数器
	}

}

void HandleScanner::pbtn_exit_clicked()
{
#ifdef NDEBUG
	if (_dlgCloseForm)
	{
		_dlgCloseForm->exec();
	}
#else
	this->close();
#endif
}

void HandleScanner::pbtn_set_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		if (numKeyBord.getValue() == "1234") {
			auto& _dlgProductSet = Modules::getInstance().uiModule._dlgProductSet;

			_dlgProductSet->setFixedSize(this->width(), this->height());
			_dlgProductSet->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
			_dlgProductSet->exec();
		}
		else {
			QMessageBox::warning(this, "Error", "密码错误，请重新输入");
		}
	}
}

void HandleScanner::pbtn_score_clicked()
{
	auto& _dlgProductScore = Modules::getInstance().uiModule._dlgProductScore;
	_dlgProductScore->setFixedSize(this->width(), this->height());
	_dlgProductScore->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_dlgProductScore->exec();
}

void HandleScanner::rbtn_debug_checked(bool checked)
{
	auto isRuning = ui->rbtn_removeFunc->isChecked();
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	if (!isRuning) {
		if (checked) {
			runningState = RunningState::Debug;
			if (camera1)
			{
				camera1->setTriggerState(false);
				camera1->setFrameRate(5);
			}
			if (camera2)
			{
				camera2->setTriggerState(false);
				camera2->setFrameRate(5);
			}
			ui->rbtn_takePicture->setChecked(false);
		}
		else {
			runningState = RunningState::Stop;
		}
		ui->ckb_shibiekuang->setVisible(checked);
		ui->ckb_wenzi->setVisible(checked);
	}
	else {
		ui->rbtn_debug->setChecked(false);
	}

	emit changeToDebugMode();
}

void HandleScanner::pbtn_openSaveLocation_clicked()
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	QString imageSavePath = imageSaveEngine->getRootPath();

	_picturesViewer->setRootPath(imageSavePath);
	_picturesViewer->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	
#ifdef NDEBUG
	_picturesViewer->showFullScreen();
#else
	_picturesViewer->show();
#endif
}

void HandleScanner::rbtn_takePicture_checked()
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;

	if (ui->rbtn_debug->isChecked() == true)
	{
		ui->rbtn_takePicture->setChecked(false);
	}
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;
	handleScannerConfig.isSaveImg = ui->rbtn_takePicture->isChecked();
	isTakePictures = ui->rbtn_takePicture->isChecked();
}

void HandleScanner::rbtn_removeFunc_checked(bool checked)
{
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;
	if (checked)
	{
		runningState = RunningState::OpenRemoveFunc;
		if (camera1)
		{
			camera1->setTriggerState(true);
			camera1->setFrameRate(50);
		}
		if (camera2)
		{
			camera2->setTriggerState(true);
			camera2->setFrameRate(50);
		}
		ui->rbtn_debug->setChecked(false);
		ui->ckb_shibiekuang->setVisible(false);
		ui->ckb_wenzi->setVisible(false);
	}
	else
	{
		runningState = RunningState::Stop;
	}

	emit changeToRemoveMode();
}

void HandleScanner::ckb_shibiekuang_checked(bool checked)
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;

	handleScannerConfig.isshibiekuang = ui->ckb_shibiekuang->isChecked();

	emit shibiekuangChanged();
}

void HandleScanner::ckb_wenzi_checked(bool checked)
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;

	handleScannerConfig.iswenzi = ui->ckb_wenzi->isChecked();

	emit wenziChanged();
}

void HandleScanner::pbtn_start_clicked()
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	auto isSuccess = zmotion->setIOOut(ControlLines::DOguangyuan, true);

}

void HandleScanner::pbtn_stop_clicked()
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	auto isSuccess = zmotion->setIOOut(ControlLines::DOguangyuan, false);

}

void HandleScanner::rbtn_strongLight_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	if (camera1)
	{
		auto isSuccess1 = camera1->setExposureTime(static_cast<size_t>(setConfig.qiangbaoguang));
		auto isSuccess2 = camera1->setGain(static_cast<size_t>(setConfig.qiangzengyi));
	}
	if (camera2)
	{
		auto isSuccess1 = camera2->setExposureTime(static_cast<size_t>(setConfig.qiangbaoguang));
		auto isSuccess2 = camera2->setGain(static_cast<size_t>(setConfig.qiangzengyi));
	}

	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;
	handleScannerConfig.isqiangguang = true;
	handleScannerConfig.iszhongguang = false;
	handleScannerConfig.isruoguang = false;
}

void HandleScanner::rbtn_mediumLight_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	if (camera1)
	{
		auto isSuccess1 = camera1->setExposureTime(static_cast<size_t>(setConfig.zhongbaoguang));
		auto isSuccess2 = camera1->setGain(static_cast<size_t>(setConfig.zhongzengyi));
	}
	if (camera2)
	{
		auto isSuccess1 = camera2->setExposureTime(static_cast<size_t>(setConfig.zhongbaoguang));
		auto isSuccess2 = camera2->setGain(static_cast<size_t>(setConfig.zhongzengyi));
	}
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;
	handleScannerConfig.isqiangguang = false;
	handleScannerConfig.iszhongguang = true;
	handleScannerConfig.isruoguang = false;
}

void HandleScanner::rbtn_weakLight_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	if (camera1)
	{
		auto isSuccess1 = camera1->setExposureTime(static_cast<size_t>(setConfig.ruobaoguang));
		auto isSuccess2 = camera1->setGain(static_cast<size_t>(setConfig.ruozengyi));
	}
	if (camera2)
	{
		auto isSuccess1 = camera2->setExposureTime(static_cast<size_t>(setConfig.ruobaoguang));
		auto isSuccess2 = camera2->setGain(static_cast<size_t>(setConfig.ruozengyi));
	}
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;
	handleScannerConfig.isqiangguang = false;
	handleScannerConfig.iszhongguang = false;
	handleScannerConfig.isruoguang = true;
}

void HandleScanner::btn_tingjigeshu_clicked()
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		ui->btn_tingjigeshu->setText(value);
		handleScannerConfig.tingjigeshu = value.toUInt();
	}
}

void HandleScanner::pbtn_resetProduct_clicked()
{
	auto& handleScannerConfig = Modules::getInstance().configManagerModule.handleScannerConfig;
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	statisticalInfo.produceCount = 0;
	statisticalInfo.wasteCount = 0;
	statisticalInfo.productionYield = 0.0;
	handleScannerConfig.totalProductionVolume = 0;
	handleScannerConfig.totalDefectiveVolume = 0;
	handleScannerConfig.productionYield = 0.0;
	onUpdateStatisticalInfoUI();
}

void HandleScanner::imgDis1_clicked()
{
	if (!_lastImage1.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImage1);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 0;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void HandleScanner::imgDis2_clicked()
{
	if (!_lastImage2.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImage2);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 1;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void HandleScanner::imgDisNg1_clicked()
{
	if (!_lastImageNg1.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNg1);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 2;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void HandleScanner::imgDisNg2_clicked()
{
	if (!_lastImageNg2.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNg2);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 3;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

