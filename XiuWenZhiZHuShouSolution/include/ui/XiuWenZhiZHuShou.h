#pragma once

#include <QMainWindow>
#include "PictureViewerThumbnails.h"
#include "rqw_LabelClickable.h"
#include "DlgCloseForm.h"
#include<QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class XiuWenZhiZHuShouClass; };
QT_END_NAMESPACE

class HandleScanner : public QMainWindow
{
	Q_OBJECT
public:
	HandleScanner(QWidget *parent = nullptr);
	~HandleScanner();
#ifdef BUILD_WITHOUT_HARDWARE
public:
	QCheckBox * cBox_testPushImg{nullptr};
public slots:
	void cBox_testPushImg_checked(bool checked);
#endif
public:
	void build_ui();
	void build_connect();
	void build_HandleScannerData();
	void ini_clickableTitle();
public:
	void initializeComponents();
	void destroyComponents();
public:
	void build_camera();
private:
	void loadCompanyMessage();
public slots:
	void updateCameraLabelState(int cameraIndex, bool state);

	void onCameraNGDisplay(QPixmap image, size_t index, bool isbad);

	void lb_title_clicked();

	void appendTcpLog(const QString text);
private slots:
	void pbtn_exit_clicked();
	void rbtn_debug_checked(bool checked);
	void rbtn_removeFunc_checked(bool checked);
	void btn_xiangsudangliang_clicked();

signals:
	void changeToDebugMode();
	void changeToRemoveMode();
private:
	Ui::XiuWenZhiZHuShouClass* ui;
	int minimizeCount{ 3 };
private:
	rw::rqw::ClickableLabel* clickableTitle = nullptr;
	DlgCloseForm* _dlgCloseForm = nullptr;
};
