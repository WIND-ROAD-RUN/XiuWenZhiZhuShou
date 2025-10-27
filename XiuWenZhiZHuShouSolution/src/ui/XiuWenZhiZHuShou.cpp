#include "ui_XiuWenZhiZHuShou.h"

#include "XiuWenZhiZHuShou.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include "rqw_RunEnvCheck.hpp"
#include "RuntimeInfoModule.hpp"
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
	, ui(new Ui::XiuWenZhiZHuShouClass())
{
	ui->setupUi(this);

	initializeComponents();

}

HandleScanner::~HandleScanner()
{
	destroyComponents();
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
	connect(ui->rbtn_debug, &QRadioButton::toggled, this, &HandleScanner::rbtn_debug_checked);
	connect(ui->rbtn_removeFunc, &QRadioButton::toggled, this, &HandleScanner::rbtn_removeFunc_checked);
	connect(ui->btn_xiangsudangliang, &QPushButton::clicked, this, &HandleScanner::btn_xiangsudangliang_clicked);
	
	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &HandleScanner::lb_title_clicked);
}

void HandleScanner::build_HandleScannerData()
{
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.mainWindowConfig;

	mainWindowConfig.isDebug = false;
	mainWindowConfig.isDefect = true;		// 默认开启剔废
	rbtn_removeFunc_checked(true);

	ui->rbtn_removeFunc->setChecked(mainWindowConfig.isDefect);
	ui->btn_xiangsudangliang->setText(QString::number(mainWindowConfig.xiangsudangliang));

	// 初始化关闭窗体
	_dlgCloseForm = new DlgCloseForm(this);
	_dlgCloseForm->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);

	ini_clickableTitle();

	// 读取本地company.txt
	loadCompanyMessage();

	// 通信日志只读
	ui->plainTextEdit_communication->setReadOnly(true);
}

void HandleScanner::ini_clickableTitle()
{
	// 初始化标题label
	clickableTitle = new rw::rqw::ClickableLabel(this);
	auto layoutTitle = ui->groupBox_head->layout();
	layoutTitle->replaceWidget(ui->label_title, clickableTitle);
	delete ui->label_title;
	clickableTitle->setText("蜘蛛手检测");
	clickableTitle->setStyleSheet("QLabel {font-size: 30px;font-weight: bold;color: rgb(255, 255, 255);padding: 5px 5px;border-bottom: 2px solid #cccccc;}");
}

void HandleScanner::initializeComponents()
{
	build_ui();

	build_camera();

	build_connect();
}

void HandleScanner::destroyComponents()
{
}

void HandleScanner::build_camera()
{
	auto& cameraModules = Modules::getInstance().cameraModule;
	auto errors = cameraModules.getBuildResults();
	updateCameraLabelState(1,true);

	for (const auto& error : errors)
	{
		auto index = static_cast<int>(error);
		updateCameraLabelState(index, false);
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
	default:
		break;
	}
}

void HandleScanner::onCameraNGDisplay(QPixmap image, size_t index, bool isbad)
{
	if (index == 1)
	{
		ui->label_imgDisplay_1->setPixmap(image.scaled(ui->label_imgDisplay_1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

		minimizeCount = 3; // 重置最小化计数器
	}

}

void HandleScanner::appendTcpLog(const QString text)
{
	if (ui->plainTextEdit_communication)
	{
		// AutoConnection: 若在 UI 线程内则直调，否则排队到 UI 线程
		QMetaObject::invokeMethod(
			ui->plainTextEdit_communication,
			"appendPlainText",
			Qt::AutoConnection,
			Q_ARG(QString, text)
		);
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

void HandleScanner::rbtn_debug_checked(bool checked)
{
	auto isRuning = ui->rbtn_removeFunc->isChecked();
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;

	if (!isRuning) {
		if (checked) {
			runningState = RunningState::Debug;
			if (camera1)
			{
				camera1->setTriggerState(false);
				camera1->setFrameRate(5);
			}
		}
		else {
			runningState = RunningState::Stop;
		}
	}
	else {
		ui->rbtn_debug->setChecked(false);
	}

	emit changeToDebugMode();
}

void HandleScanner::rbtn_removeFunc_checked(bool checked)
{
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	if (checked)
	{
		runningState = RunningState::OpenRemoveFunc;
		if (camera1)
		{
			camera1->setTriggerState(true);
			camera1->setFrameRate(50);
		}
		ui->rbtn_debug->setChecked(false);
	}
	else
	{
		runningState = RunningState::Stop;
	}

	emit changeToRemoveMode();
}

void HandleScanner::btn_xiangsudangliang_clicked()
{
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
		auto& generalConfig = Modules::getInstance().configManagerModule.mainWindowConfig;
		ui->btn_xiangsudangliang->setText(value);
		generalConfig.xiangsudangliang = value.toDouble();
	}
}