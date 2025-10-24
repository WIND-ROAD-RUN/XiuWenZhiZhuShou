#pragma once

#include <QMainWindow>
#include "PictureViewerThumbnails.h"
#include "ImageEnlargedDisplay.h"
#include "rqw_LabelClickable.h"
#include "DlgCloseForm.h"
#include<QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class PunchPressClass; };
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

	void build_motion();

	void build_ImageEnlargedDisplay();
	void destroy_ImageEnlargedDisplay();

private:
	void loadCompanyMessage();
	void loadSaveImgPath();
public slots:
	void updateCameraLabelState(int cameraIndex, bool state);

	void onUpdateStatisticalInfoUI();

	void onCameraNGDisplay(QPixmap image, size_t index, bool isbad);

	void lb_title_clicked();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void pbtn_score_clicked();
	void rbtn_debug_checked(bool checked);
	void pbtn_openSaveLocation_clicked();
	void rbtn_takePicture_checked();
	void rbtn_removeFunc_checked(bool checked);
	void ckb_shibiekuang_checked(bool checked);
	void ckb_wenzi_checked(bool checked);
	void pbtn_start_clicked();
	void pbtn_stop_clicked();
	void rbtn_strongLight_checked(bool isChecked);
	void rbtn_mediumLight_checked(bool isChecked);
	void rbtn_weakLight_checked(bool isChecked);
	void btn_tingjigeshu_clicked();
	void pbtn_resetProduct_clicked();

signals:
	void shibiekuangChanged();
	void wenziChanged();
	void changeToDebugMode();
	void changeToRemoveMode();

private:
	bool _isImageEnlargedDisplay{ false };
	int _currentImageEnlargedDisplayIndex{ 0 };
	std::map<int, QString> _workStationTitleMap{};

	QPixmap _lastImage1{};
	QPixmap _lastImage2{};
	QPixmap _lastImageNg1{};
	QPixmap _lastImageNg2{};

	rw::rqw::ClickableLabel* imgDis1 = nullptr;
	rw::rqw::ClickableLabel* imgDis2 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg1 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg2 = nullptr;
private slots:
	void imgDis1_clicked();
	void imgDis2_clicked();
	void imgDisNg1_clicked();
	void imgDisNg2_clicked();

	
private:
	Ui::PunchPressClass* ui;
	int minimizeCount{ 3 };
private:
	PictureViewerThumbnails* _picturesViewer = nullptr;
	ImageEnlargedDisplay* _imageEnlargedDisplay = nullptr;
	rw::rqw::ClickableLabel* clickableTitle = nullptr;
	DlgCloseForm* _dlgCloseForm = nullptr;
};
