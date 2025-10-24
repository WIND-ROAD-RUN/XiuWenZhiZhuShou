#include "DlgProductSet.h"
#include "ui_DlgProductSet.h"

#include <QMessageBox>
#include <QTabWidget>
#include <QtConcurrent/qtconcurrentrun.h>

#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "NumberKeyboard.h"

#include <QTimer>
#include <QPointer>
#include <functional>

namespace {
	struct StepEx { size_t line; bool state; double delayMs; };

	static void runZMotionSequenceEx(DlgProductSet* self,
		const QVector<StepEx>& steps,
		std::function<void()> finished = {})
	{
		if (!self || steps.isEmpty()) {
			if (finished) finished();
			return;
		}
		QPointer<DlgProductSet> ctx(self);
		auto index = std::make_shared<int>(0);
		auto runner = std::make_shared<std::function<void()>>();

		*runner = [ctx, index, steps, finished, runner]() mutable {
			if (!ctx) return;

			const int i = *index;
			if (i >= steps.size()) {
				if (finished) finished();
				return;
			}

			const auto& s = steps[i];
			auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
			(void)zmotion->setIOOut(s.line, s.state);

			++(*index);
			if (*index >= steps.size()) {
				if (finished) finished();
				return;
			}

			QTimer::singleShot(s.delayMs, ctx, [runner]() {
				(*runner)();
				});
			};

		(*runner)();
	}
}

DlgProductSet::DlgProductSet(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductSetClass())
{
	ui->setupUi(this);

	build_ui();

	build_connect();

	initDOCheckItems();
	initDICheckItems();
}

DlgProductSet::~DlgProductSet()
{
	delete ui;
}

void DlgProductSet::showEvent(QShowEvent* event)
{
	if (ui && ui->tabWidget)
	{
		ui->tabWidget->setCurrentIndex(0);
	}
}

void DlgProductSet::build_ui()
{
	read_config();

	// 查看是否有相同的输入输出IO
	auto indicesDO = DOFindAllDuplicateIndices();
	setDOErrorInfo(indicesDO);
	auto indicesDI = DIFindAllDuplicateIndices();
	setDIErrorInfo(indicesDI);
}

void DlgProductSet::read_config()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	// 基本功能
	ui->cbox_qiyongerxiangji->setChecked(setConfig.qiyongerxiangji);
	ui->cbox_qiyongyundongkongzhiqi->setChecked(setConfig.qiyongyundongkongzhiqi);
	ui->cbox_yundongkongzhiqichonglian->setChecked(setConfig.yundongkongzhiqichonglian);

	// 基本参数
	ui->btn_yanshixiachawai1->setText(QString::number(setConfig.yanshixiachawai1));
	ui->btn_boliaoshijianwai1->setText(QString::number(setConfig.boliaoshijianwai1));

	ui->btn_yanshixiachawai2->setText(QString::number(setConfig.yanshixiachawai2));
	ui->btn_boliaoshijianwai2->setText(QString::number(setConfig.boliaoshijianwai2));

	ui->btn_yanshixiachanei1->setText(QString::number(setConfig.yanshixiachanei1));
	ui->btn_boliaoshijiannei1->setText(QString::number(setConfig.boliaoshijiannei1));

	ui->btn_yanshixiachanei2->setText(QString::number(setConfig.yanshixiachanei2));
	ui->btn_boliaoshijiannei2->setText(QString::number(setConfig.boliaoshijiannei2));

	ui->btn_xiangjichufachangdu->setText(QString::number(setConfig.xiangjichufachangdu));
	ui->btn_meizhuanmaichongshu->setText(QString::number(setConfig.meizhuanmaichongshu));
	ui->btn_shedingzhouchang->setText(QString::number(setConfig.shedingzhouchang));

	// 工位剔废参数
	ui->btn_waicetifeijuli1->setText(QString::number(setConfig.waicetifeijuli1));
	ui->btn_neicetifeijuli1->setText(QString::number(setConfig.neicetifeijuli1));
	ui->btn_rongyufangdou1->setText(QString::number(setConfig.tifeirongyufangdou1));
	ui->btn_waicetifeijuli2->setText(QString::number(setConfig.waicetifeijuli2));
	ui->btn_neicetifeijuli2->setText(QString::number(setConfig.neicetifeijuli2));
	ui->btn_rongyufangdou2->setText(QString::number(setConfig.tifeirongyufangdou2));
	ui->btn_tuxiangshunshizhenxuanzhuancishu1->setText(QString::number(setConfig.tuxiangshunshizhenxuanzhuancishu1));
	ui->btn_tuxiangshunshizhenxuanzhuancishu2->setText(QString::number(setConfig.tuxiangshunshizhenxuanzhuancishu2));

	// 工位限位设置
	// 一工位
	ui->pbtn_shangxianwei1->setText(QString::number(setConfig.shangXianWei1));
	ui->pbtn_xiaxianwei1->setText(QString::number(setConfig.xiaXianWei1));
	ui->pbtn_zuoxianwei1->setText(QString::number(setConfig.zuoXianWei1));
	ui->pbtn_youxianwei1->setText(QString::number(setConfig.youXianWei1));
	ui->pbtn_xiangsudangliang1->setText(QString::number(setConfig.xiangSuDangLiang1));
	ui->pbtn_zhongyangfenjiexian1->setText(QString::number(setConfig.zhongyangxianwei1));

	// 二工位
	ui->pbtn_shangxianwei2->setText(QString::number(setConfig.shangXianWei2));
	ui->pbtn_xiaxianwei2->setText(QString::number(setConfig.xiaXianWei2));
	ui->pbtn_zuoxianwei2->setText(QString::number(setConfig.zuoXianWei2));
	ui->pbtn_youxianwei2->setText(QString::number(setConfig.youXianWei2));
	ui->pbtn_xiangsudangliang2->setText(QString::number(setConfig.xiangSuDangLiang2));
	ui->pbtn_zhongyangfenjiexian2->setText(QString::number(setConfig.zhongyangxianwei2));

	// 光源设置
	ui->pbtn_qiangbaoguang->setText(QString::number(setConfig.qiangbaoguang));
	ui->pbtn_qiangzengyi->setText(QString::number(setConfig.qiangzengyi));
	ui->pbtn_zhongbaoguang->setText(QString::number(setConfig.zhongbaoguang));
	ui->pbtn_zhongzengyi->setText(QString::number(setConfig.zhongzengyi));
	ui->pbtn_ruobaoguang->setText(QString::number(setConfig.ruobaoguang));
	ui->pbtn_ruozengyi->setText(QString::number(setConfig.ruozengyi));

	// 图像存储设置
	ui->cBox_takeCamera1Pictures->setChecked(setConfig.takeWork1Pictures);
	ui->cBox_takeCamera2Pictures->setChecked(setConfig.takeWork2Pictures);

	ui->cBox_takeNgPictures->setChecked(setConfig.saveNGImg);
	ui->cBox_takeMaskPictures->setChecked(setConfig.saveMaskImg);
	ui->cBox_takeOkPictures->setChecked(setConfig.saveOKImg);

	ui->rbtn_saveJpeg->setChecked(setConfig.isSaveJPEG);
	ui->rbtn_saveBmp->setChecked(setConfig.isSaveBMP);
	ui->rbtn_savePng->setChecked(setConfig.isSavePNG);
	ui->btn_saveQuality->setText(QString::number(setConfig.saveImgQuality));

	// IO监控（无）

	// IO设置
	ui->btn_setDOpaizhao1->setText(QString::number(setConfig.DOpaizhao1));
	ui->btn_setDOpaizhao2->setText(QString::number(setConfig.DOpaizhao2));
	ui->btn_setDOguangyuan->setText(QString::number(setConfig.DOguangyuan));
	ui->btn_setDObaojing->setText(QString::number(setConfig.DObaojing));
	ui->btn_setDOtingji->setText(QString::number(setConfig.DOtingji));
	ui->btn_setDOxiachaqigangwai1->setText(QString::number(setConfig.DOxiachaqigangwai1));
	ui->btn_setDOxiachaqigangnei1->setText(QString::number(setConfig.DOxiachaqigangnei1));
	ui->btn_setDOxiachaqigangwai2->setText(QString::number(setConfig.DOxiachaqigangwai2));
	ui->btn_setDOxiachaqigangnei2->setText(QString::number(setConfig.DOxiachaqigangnei2));
	ui->btn_setDOboliaoqigangwai1->setText(QString::number(setConfig.DOboliaoqigangwai1));
	ui->btn_setDOboliaoqigangnei1->setText(QString::number(setConfig.DOboliaoqigangnei1));
	ui->btn_setDOboliaoqigangwai2->setText(QString::number(setConfig.DOboliaoqigangwai2));
	ui->btn_setDOboliaoqigangnei2->setText(QString::number(setConfig.DOboliaoqigangnei2));


	// 分数界面内容可选显示
	ui->ckb_tangshang->setChecked(setConfig.istangshang);
	ui->ckb_queliao->setChecked(setConfig.isqueliao);
	ui->ckb_yiwu->setChecked(setConfig.isyiwu);

	ui->ckb_zangwu->setChecked(setConfig.iszangwu);
	ui->ckb_zhanlian->setChecked(setConfig.isextra2);
	ui->ckb_feiliao->setChecked(setConfig.isextra3);
	ui->ckb_mojucuowu->setChecked(setConfig.isextra4);
	ui->ckb_extra5->setChecked(setConfig.isextra5);
	ui->ckb_extra6->setChecked(setConfig.isextra6);
	ui->ckb_extra7->setChecked(setConfig.isextra7);
	ui->ckb_extra8->setChecked(setConfig.isextra8);
	ui->ckb_extra9->setChecked(setConfig.isextra9);

	// 调试模式默认为关闭
	setConfig.debugMode = false;
	ui->cbox_debugMode->setChecked(setConfig.debugMode);

	// 默认显示第一个
	ui->tabWidget->setCurrentIndex(0);

	updateControlLinesFromCheckBoxes();
}

void DlgProductSet::build_connect()
{
	auto& globalThread = GlobalThread::getInstance();

	QObject::connect(ui->pbtn_close, &QPushButton::clicked, this, &DlgProductSet::pbtn_close_clicked);

	// 工位限位与像素定量
	QObject::connect(ui->pbtn_shangxianwei1, &QPushButton::clicked, this, &DlgProductSet::pbtn_shangxianwei1_clicked);
	QObject::connect(ui->pbtn_xiaxianwei1, &QPushButton::clicked, this, &DlgProductSet::pbtn_xiaxianwei1_clicked);
	QObject::connect(ui->pbtn_zuoxianwei1, &QPushButton::clicked, this, &DlgProductSet::pbtn_zuoxianwei1_clicked);
	QObject::connect(ui->pbtn_youxianwei1, &QPushButton::clicked, this, &DlgProductSet::pbtn_youxianwei1_clicked);
	QObject::connect(ui->pbtn_xiangsudangliang1, &QPushButton::clicked, this, &DlgProductSet::pbtn_xiangsudangliang1_clicked);
	QObject::connect(ui->pbtn_shangxianwei2, &QPushButton::clicked, this, &DlgProductSet::pbtn_shangxianwei2_clicked);
	QObject::connect(ui->pbtn_xiaxianwei2, &QPushButton::clicked, this, &DlgProductSet::pbtn_xiaxianwei2_clicked);
	QObject::connect(ui->pbtn_zuoxianwei2, &QPushButton::clicked, this, &DlgProductSet::pbtn_zuoxianwei2_clicked);
	QObject::connect(ui->pbtn_youxianwei2, &QPushButton::clicked, this, &DlgProductSet::pbtn_youxianwei2_clicked);
	QObject::connect(ui->pbtn_xiangsudangliang2, &QPushButton::clicked, this, &DlgProductSet::pbtn_xiangsudangliang2_clicked);
	QObject::connect(ui->pbtn_zhongyangfenjiexian1, &QPushButton::clicked, this, &DlgProductSet::pbtn_zhongyangfenjiexian1_clicked);
	QObject::connect(ui->pbtn_zhongyangfenjiexian2, &QPushButton::clicked, this, &DlgProductSet::pbtn_zhongyangfenjiexian2_clicked);

	// 图像存储相关
	QObject::connect(ui->cBox_takeNgPictures, &QCheckBox::clicked, this, &DlgProductSet::cBox_takeNgPictures_checked);
	QObject::connect(ui->cBox_takeMaskPictures, &QCheckBox::clicked, this, &DlgProductSet::cBox_takeMaskPictures_checked);
	QObject::connect(ui->cBox_takeOkPictures, &QCheckBox::clicked, this, &DlgProductSet::cBox_takeOkPictures_checked);
	QObject::connect(ui->cBox_takeCamera1Pictures, &QCheckBox::clicked, this, &DlgProductSet::cBox_takeCamera1Pictures_checked);
	QObject::connect(ui->cBox_takeCamera2Pictures, &QCheckBox::clicked, this, &DlgProductSet::cBox_takeCamera2Pictures_checked);
	QObject::connect(ui->rbtn_saveJpeg, &QRadioButton::clicked, this, &DlgProductSet::rbtn_saveJpeg_checked);
	QObject::connect(ui->rbtn_saveBmp, &QRadioButton::clicked, this, &DlgProductSet::rbtn_saveBmp_checked);
	QObject::connect(ui->rbtn_savePng, &QRadioButton::clicked, this, &DlgProductSet::rbtn_savePng_checked);
	QObject::connect(ui->btn_saveQuality, &QPushButton::clicked, this, &DlgProductSet::btn_saveQuality_clicked);

	// 基本功能
	QObject::connect(ui->cbox_qiyongerxiangji, &QCheckBox::clicked, this, &DlgProductSet::cbox_qiyongerxiangji_checked);
	QObject::connect(ui->cbox_qiyongyundongkongzhiqi, &QCheckBox::clicked, this, &DlgProductSet::cbox_qiyongyundongkongzhiqi_checked);
	QObject::connect(ui->cbox_yundongkongzhiqichonglian, &QCheckBox::clicked, this, &DlgProductSet::cbox_yundongkongzhiqichonglian_checked);

	// 光源设置
	QObject::connect(ui->pbtn_qiangbaoguang, &QPushButton::clicked, this, &DlgProductSet::pbtn_qiangbaoguang_clicked);
	QObject::connect(ui->pbtn_qiangzengyi, &QPushButton::clicked, this, &DlgProductSet::pbtn_qiangzengyi_clicked);
	QObject::connect(ui->pbtn_zhongbaoguang, &QPushButton::clicked, this, &DlgProductSet::pbtn_zhongbaoguang_clicked);
	QObject::connect(ui->pbtn_zhongzengyi, &QPushButton::clicked, this, &DlgProductSet::pbtn_zhongzengyi_clicked);
	QObject::connect(ui->pbtn_ruobaoguang, &QPushButton::clicked, this, &DlgProductSet::pbtn_ruobaoguang_clicked);
	QObject::connect(ui->pbtn_ruozengyi, &QPushButton::clicked, this, &DlgProductSet::pbtn_ruozengyi_clicked);

	// 工位剔废参数
	QObject::connect(ui->btn_waicetifeijuli1, &QPushButton::clicked, this, &DlgProductSet::btn_waicetifeijuli1_clicked);
	QObject::connect(ui->btn_neicetifeijuli1, &QPushButton::clicked, this, &DlgProductSet::btn_neicetifeijuli1_clicked);
	QObject::connect(ui->btn_rongyufangdou1, &QPushButton::clicked, this, &DlgProductSet::btn_rongyufangdou1_clicked);
	QObject::connect(ui->btn_waicetifeijuli2, &QPushButton::clicked, this, &DlgProductSet::btn_waicetifeijuli2_clicked);
	QObject::connect(ui->btn_neicetifeijuli2, &QPushButton::clicked, this, &DlgProductSet::btn_neicetifeijuli2_clicked);
	QObject::connect(ui->btn_rongyufangdou2, &QPushButton::clicked, this, &DlgProductSet::btn_rongyufangdou2_clicked);
	QObject::connect(ui->btn_tuxiangshunshizhenxuanzhuancishu1, &QPushButton::clicked, this, &DlgProductSet::btn_tuxiangshunshizhenxuanzhuancishu1_clicked);
	QObject::connect(ui->btn_tuxiangshunshizhenxuanzhuancishu2, &QPushButton::clicked, this, &DlgProductSet::btn_tuxiangshunshizhenxuanzhuancishu2_clicked);
	QObject::connect(ui->btn_xunzhaoquexiansuoshubashoudepianchazhi, &QPushButton::clicked, this, &DlgProductSet::btn_xunzhaoquexiansuoshubashoudepianchazhi_clicked);

	// 分数界面可选显示
	QObject::connect(ui->ckb_tangshang, &QCheckBox::clicked, this, &DlgProductSet::ckb_tangshang_checked);
	QObject::connect(ui->ckb_queliao, &QCheckBox::clicked, this, &DlgProductSet::ckb_queliao_checked);
	QObject::connect(ui->ckb_yiwu, &QCheckBox::clicked, this, &DlgProductSet::ckb_yiwu_checked);
	QObject::connect(ui->ckb_zangwu, &QCheckBox::clicked, this, &DlgProductSet::ckb_zangwu_checked);
	QObject::connect(ui->ckb_zhanlian, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra2_checked);
	QObject::connect(ui->ckb_feiliao, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra3_checked);
	QObject::connect(ui->ckb_mojucuowu, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra4_checked);
	QObject::connect(ui->ckb_extra5, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra5_checked);
	QObject::connect(ui->ckb_extra6, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra6_checked);
	QObject::connect(ui->ckb_extra7, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra7_checked);
	QObject::connect(ui->ckb_extra8, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra8_checked);
	QObject::connect(ui->ckb_extra9, &QCheckBox::clicked, this, &DlgProductSet::ckb_extra9_checked);

	// IO设置
	QObject::connect(ui->btn_setDOpaizhao1, &QPushButton::clicked, this, &DlgProductSet::btn_setDOpaizhao1_clicked);
	QObject::connect(ui->btn_setDOpaizhao2, &QPushButton::clicked, this, &DlgProductSet::btn_setDOpaizhao2_clicked);
	QObject::connect(ui->btn_setDOguangyuan, &QPushButton::clicked, this, &DlgProductSet::btn_setDOguangyuan_clicked);
	QObject::connect(ui->btn_setDObaojing, &QPushButton::clicked, this, &DlgProductSet::btn_setDObaojing_clicked);
	QObject::connect(ui->btn_setDOtingji, &QPushButton::clicked, this, &DlgProductSet::btn_setDOtingji_clicked);
	QObject::connect(ui->btn_setDOxiachaqigangwai1, &QPushButton::clicked, this, &DlgProductSet::btn_setDOxiachaqigangwai1_clicked);
	QObject::connect(ui->btn_setDOxiachaqigangnei1, &QPushButton::clicked, this, &DlgProductSet::btn_setDOxiachaqigangnei1_clicked);
	QObject::connect(ui->btn_setDOxiachaqigangwai2, &QPushButton::clicked, this, &DlgProductSet::btn_setDOxiachaqigangwai2_clicked);
	QObject::connect(ui->btn_setDOxiachaqigangnei2, &QPushButton::clicked, this, &DlgProductSet::btn_setDOxiachaqigangnei2_clicked);
	QObject::connect(ui->btn_setDOboliaoqigangwai1, &QPushButton::clicked, this, &DlgProductSet::btn_setDOboliaoqigangwai1_clicked);
	QObject::connect(ui->btn_setDOboliaoqigangnei1, &QPushButton::clicked, this, &DlgProductSet::btn_setDOboliaoqigangnei1_clicked);
	QObject::connect(ui->btn_setDOboliaoqigangwai2, &QPushButton::clicked, this, &DlgProductSet::btn_setDOboliaoqigangwai2_clicked);
	QObject::connect(ui->btn_setDOboliaoqigangnei2, &QPushButton::clicked, this, &DlgProductSet::btn_setDOboliaoqigangnei2_clicked);

	// 基本参数
	QObject::connect(ui->btn_yanshixiachawai1, &QPushButton::clicked, this, &DlgProductSet::btn_yanshixiachawai1_clicked);
	QObject::connect(ui->btn_boliaoshijianwai1, &QPushButton::clicked, this, &DlgProductSet::btn_boliaoshijianwai1_clicked);

	QObject::connect(ui->btn_yanshixiachawai2, &QPushButton::clicked, this, &DlgProductSet::btn_yanshixiachawai2_clicked);
	QObject::connect(ui->btn_boliaoshijianwai2, &QPushButton::clicked, this, &DlgProductSet::btn_boliaoshijianwai2_clicked);

	QObject::connect(ui->btn_yanshixiachanei1, &QPushButton::clicked, this, &DlgProductSet::btn_yanshixiachanei1_clicked);
	QObject::connect(ui->btn_boliaoshijiannei1, &QPushButton::clicked, this, &DlgProductSet::btn_boliaoshijiannei1_clicked);

	QObject::connect(ui->btn_yanshixiachanei2, &QPushButton::clicked, this, &DlgProductSet::btn_yanshixiachanei2_clicked);
	QObject::connect(ui->btn_boliaoshijiannei2, &QPushButton::clicked, this, &DlgProductSet::btn_boliaoshijiannei2_clicked);

	QObject::connect(ui->btn_xiangjichufachangdu, &QPushButton::clicked, this, &DlgProductSet::btn_xiangjichufachangdu_clicked);
	QObject::connect(ui->btn_meizhuanmaichongshu, &QPushButton::clicked, this, &DlgProductSet::btn_meizhuanmaichongshu_clicked);
	QObject::connect(ui->btn_shedingzhouchang, &QPushButton::clicked, this, &DlgProductSet::btn_shedingzhouchang_clicked);

	// IO监控
	QObject::connect(ui->cbox_DOpaizhao1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOpaizhao1_clicked);
	QObject::connect(ui->cbox_DOpaizhao2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOpaizhao2_clicked);
	QObject::connect(ui->cbox_DOguangyuan, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOguangyuan_clicked);
	QObject::connect(ui->cbox_DObaojing, &QCheckBox::clicked, this, &DlgProductSet::cbox_DObaojing_clicked);
	QObject::connect(ui->cbox_DOtingji, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOtingji_clicked);
	QObject::connect(ui->cbox_DOxiachaqigangwai1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOxiachaqigangwai1_clicked);
	QObject::connect(ui->cbox_DOxiachaqigangwai2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOxiachaqigangwai2_clicked);
	QObject::connect(ui->cbox_DOxiachaqigangnei1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOxiachaqigangnei1_clicked);
	QObject::connect(ui->cbox_DOxiachaqigangnei2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOxiachaqigangnei2_clicked);
	QObject::connect(ui->cbox_DOboliaoqigangwai1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOboliaoqigangwai1_clicked);
	QObject::connect(ui->cbox_DOboliaoqigangwai2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOboliaoqigangwai2_clicked);
	QObject::connect(ui->cbox_DOboliaoqigangnei1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOboliaoqigangnei1_clicked);
	QObject::connect(ui->cbox_DOboliaoqigangnei2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOboliaoqigangnei2_clicked);
	QObject::connect(ui->cbox_DOtifeidongzuowai1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOtifeidongzuowai1_clicked);
	QObject::connect(ui->cbox_DOtifeidongzuowai2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOtifeidongzuowai2_clicked);
	QObject::connect(ui->cbox_DOtifeidongzuonei1, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOtifeidongzuonei1_clicked);
	QObject::connect(ui->cbox_DOtifeidongzuonei2, &QCheckBox::clicked, this, &DlgProductSet::cbox_DOtifeidongzuonei2_clicked);


	QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, this, &DlgProductSet::tabWidget_indexChanged);
	QObject::connect(ui->cbox_debugMode, &QCheckBox::clicked, this, &DlgProductSet::cbox_debugMode_checked);
}
void DlgProductSet::setDIErrorInfo(const std::vector<std::vector<int>>& index)
{
	for (const auto& classic : index)
	{
		for (const auto& item : classic)
		{
			setDIErrorInfo(item);
		}
	}
}

void DlgProductSet::setDIErrorInfo(int index)
{
	QString text = "重复数值";
	switch (index)
	{
	default:
		break;
	}
}

void DlgProductSet::setAllDOCheckBoxCheckedToFalse()
{
	ui->cbox_DOpaizhao1->setChecked(false);
	ui->cbox_DOpaizhao2->setChecked(false);
	ui->cbox_DOguangyuan->setChecked(false);
	ui->cbox_DObaojing->setChecked(false);
	ui->cbox_DOtingji->setChecked(false);
	ui->cbox_DOxiachaqigangwai1->setChecked(false);
	ui->cbox_DOxiachaqigangwai2->setChecked(false);
	ui->cbox_DOxiachaqigangnei1->setChecked(false);
	ui->cbox_DOxiachaqigangnei2->setChecked(false);
	ui->cbox_DOboliaoqigangwai1->setChecked(false);
	ui->cbox_DOboliaoqigangwai2->setChecked(false);
	ui->cbox_DOboliaoqigangnei1->setChecked(false);
	ui->cbox_DOboliaoqigangnei2->setChecked(false);
	ui->cbox_DOtifeidongzuowai1->setChecked(false);
	ui->cbox_DOtifeidongzuowai2->setChecked(false);
	ui->cbox_DOtifeidongzuonei1->setChecked(false);
	ui->cbox_DOtifeidongzuonei2->setChecked(false);
}

void DlgProductSet::setAllDOCheckBoxEnableToFalse()
{
	ui->cbox_DOpaizhao1->setEnabled(false);
	ui->cbox_DOpaizhao2->setEnabled(false);
	ui->cbox_DOguangyuan->setEnabled(false);
	ui->cbox_DObaojing->setEnabled(false);
	ui->cbox_DOtingji->setEnabled(false);
	ui->cbox_DOxiachaqigangwai1->setEnabled(false);
	ui->cbox_DOxiachaqigangwai2->setEnabled(false);
	ui->cbox_DOxiachaqigangnei1->setEnabled(false);
	ui->cbox_DOxiachaqigangnei2->setEnabled(false);
	ui->cbox_DOboliaoqigangwai1->setEnabled(false);
	ui->cbox_DOboliaoqigangwai2->setEnabled(false);
	ui->cbox_DOboliaoqigangnei1->setEnabled(false);
	ui->cbox_DOboliaoqigangnei2->setEnabled(false);
	ui->cbox_DOtifeidongzuowai1->setEnabled(false);
	ui->cbox_DOtifeidongzuowai2->setEnabled(false);
	ui->cbox_DOtifeidongzuonei1->setEnabled(false);
	ui->cbox_DOtifeidongzuonei2->setEnabled(false);
}

void DlgProductSet::setAllDOCheckBoxEnableToTrue()
{
	ui->cbox_DOpaizhao1->setEnabled(true);
	ui->cbox_DOpaizhao2->setEnabled(true);
	ui->cbox_DOguangyuan->setEnabled(true);
	ui->cbox_DObaojing->setEnabled(true);
	ui->cbox_DOtingji->setEnabled(true);
	ui->cbox_DOxiachaqigangwai1->setEnabled(true);
	ui->cbox_DOxiachaqigangwai2->setEnabled(true);
	ui->cbox_DOxiachaqigangnei1->setEnabled(true);
	ui->cbox_DOxiachaqigangnei2->setEnabled(true);
	ui->cbox_DOboliaoqigangwai1->setEnabled(true);
	ui->cbox_DOboliaoqigangwai2->setEnabled(true);
	ui->cbox_DOboliaoqigangnei1->setEnabled(true);
	ui->cbox_DOboliaoqigangnei2->setEnabled(true);
	ui->cbox_DOtifeidongzuowai1->setEnabled(true);
	ui->cbox_DOtifeidongzuowai2->setEnabled(true);
	ui->cbox_DOtifeidongzuonei1->setEnabled(true);
	ui->cbox_DOtifeidongzuonei2->setEnabled(true);
}

void DlgProductSet::updateControlLinesFromCheckBoxes()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	ControlLines::DObaojing = setConfig.DObaojing;
	ControlLines::DOtingji = setConfig.DOtingji;
	ControlLines::DOpaizhao1 = setConfig.DOpaizhao1;
	ControlLines::DOpaizhao2 = setConfig.DOpaizhao2;
	ControlLines::DOguangyuan = setConfig.DOguangyuan;
	ControlLines::DOxiachaqigangwai1 = setConfig.DOxiachaqigangwai1;
	ControlLines::DOxiachaqigangwai2 = setConfig.DOxiachaqigangwai2;
	ControlLines::DOxiachaqigangnei1 = setConfig.DOxiachaqigangnei1;
	ControlLines::DOxiachaqigangnei2 = setConfig.DOxiachaqigangnei2;
	ControlLines::DOboliaoqigangwai1 = setConfig.DOboliaoqigangwai1;
	ControlLines::DOboliaoqigangwai2 = setConfig.DOboliaoqigangwai2;
	ControlLines::DOboliaoqigangnei1 = setConfig.DOboliaoqigangnei1;
	ControlLines::DOboliaoqigangnei2 = setConfig.DOboliaoqigangnei2;
}

void DlgProductSet::initDOCheckItems()
{
	DOCheckItems = {
		{ &ControlLines::DOpaizhao1,     ui->cbox_DOpaizhao1 },
		{ &ControlLines::DOpaizhao2,     ui->cbox_DOpaizhao2 },
		{ &ControlLines::DOguangyuan,    ui->cbox_DOguangyuan },
		{ &ControlLines::DObaojing,      ui->cbox_DObaojing },
		{ &ControlLines::DOtingji,       ui->cbox_DOtingji },
		{ &ControlLines::DOxiachaqigangwai1,ui->cbox_DOxiachaqigangwai1 },
		{ &ControlLines::DOxiachaqigangwai2,ui->cbox_DOxiachaqigangwai2 },
		{ &ControlLines::DOxiachaqigangnei1,ui->cbox_DOxiachaqigangnei1 },
		{ &ControlLines::DOxiachaqigangnei2,ui->cbox_DOxiachaqigangnei2 },
		{ &ControlLines::DOboliaoqigangwai1,ui->cbox_DOboliaoqigangwai1 },
		{ &ControlLines::DOboliaoqigangwai2,ui->cbox_DOboliaoqigangwai2 },
		{ &ControlLines::DOboliaoqigangnei1,ui->cbox_DOboliaoqigangnei1 },
		{ &ControlLines::DOboliaoqigangnei2,ui->cbox_DOboliaoqigangnei2 }
	};
}

void DlgProductSet::initDICheckItems()
{
	DICheckItems = {
	   { }
	};
}

std::vector<std::vector<int>> DlgProductSet::DOFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	std::vector<int> values = {
		setConfig.DOpaizhao1,
		setConfig.DOpaizhao2,
		setConfig.DOguangyuan,
		setConfig.DObaojing,
		setConfig.DOtingji,
		setConfig.DOxiachaqigangwai1,
		setConfig.DOxiachaqigangnei1,
		setConfig.DOxiachaqigangwai2,
		setConfig.DOxiachaqigangnei2,
		setConfig.DOboliaoqigangwai1,
		setConfig.DOboliaoqigangnei1,
		setConfig.DOboliaoqigangwai2,
		setConfig.DOboliaoqigangnei2
	};

	std::unordered_map<int, std::vector<int>> valueToIndices;
	for (size_t i = 0; i < values.size(); ++i) {
		valueToIndices[values[i]].push_back(static_cast<int>(i));
	}

	std::vector<std::vector<int>> result;
	std::set<int> used; // 防止重复分组
	for (const auto& pair : valueToIndices) {
		if (pair.second.size() > 1) {
			// 只收集未被收录过的index组
			bool alreadyUsed = false;
			for (int idx : pair.second) {
				if (used.count(idx)) {
					alreadyUsed = true;
					break;
				}
			}
			if (!alreadyUsed) {
				result.push_back(pair.second);
				used.insert(pair.second.begin(), pair.second.end());
			}
		}
	}
	return result;
}

void DlgProductSet::setDOErrorInfo(const std::vector<std::vector<int>>& index)
{
	ui->lb_DOpaizhao1->clear();
	ui->lb_DOpaizhao2->clear();
	ui->lb_DOguangyuan->clear();
	ui->lb_DObaojing->clear();
	ui->lb_DOtingji->clear();

	ui->lb_DOxiachaqigangwai1->clear();
	ui->lb_DOxiachaqigangnei1->clear();
	ui->lb_DOxiachaqigangwai2->clear();
	ui->lb_DOxiachaqigangnei2->clear();

	ui->lb_DOboliaoqigangwai1->clear();
	ui->lb_DOboliaoqigangnei1->clear();
	ui->lb_DOboliaoqigangwai2->clear();
	ui->lb_DOboliaoqigangnei2->clear();

	for (const auto& classic : index)
	{
		for (const auto& item : classic)
		{
			setDOErrorInfo(item);
		}
	}
}

void DlgProductSet::setDOErrorInfo(int index)
{
	QString text = "重复数值";
	switch (index)
	{
	case 0:
		ui->lb_DOpaizhao1->setText(text);
		break;
	case 1:
		ui->lb_DOpaizhao2->setText(text);
		break;
	case 2:
		ui->lb_DOguangyuan->setText(text);
		break;
	case 3:
		ui->lb_DObaojing->setText(text);
		break;
	case 4:
		ui->lb_DOtingji->setText(text);
		break;
	case 5:
		ui->lb_DOxiachaqigangwai1->setText(text);
		break;	 
	case 6:		 
		ui->lb_DOxiachaqigangnei1->setText(text);
		break;	 
	case 7:		 
		ui->lb_DOxiachaqigangwai2->setText(text);
		break;	 
	case 8:		 
		ui->lb_DOxiachaqigangnei2->setText(text);
		break;
	case 9:
		ui->lb_DOboliaoqigangwai1->setText(text);
		break;
	case 10:
		ui->lb_DOboliaoqigangnei1->setText(text);
		break;
	case 11:
		ui->lb_DOboliaoqigangwai2->setText(text);
		break;
	case 12:
		ui->lb_DOboliaoqigangnei2->setText(text);
		break;
	default:
		break;
	}
}

std::vector<std::vector<int>> DlgProductSet::DIFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	std::vector<int> values = {

	};

	std::unordered_map<int, std::vector<int>> valueToIndices;
	for (size_t i = 0; i < values.size(); ++i) {
		valueToIndices[values[i]].push_back(static_cast<int>(i));
	}

	std::vector<std::vector<int>> result;
	std::set<int> used; // 防止重复分组
	for (const auto& pair : valueToIndices) {
		if (pair.second.size() > 1) {
			// 只收集未被收录过的index组
			bool alreadyUsed = false;
			for (int idx : pair.second) {
				if (used.count(idx)) {
					alreadyUsed = true;
					break;
				}
			}
			if (!alreadyUsed) {
				result.push_back(pair.second);
				used.insert(pair.second.begin(), pair.second.end());
			}
		}
	}
	return result;
}

void DlgProductSet::pbtn_close_clicked()
{
	auto& motionControllerModule = Modules::getInstance().motionControllerModule;

	// 关闭监控IO线程
	motionControllerModule._isUpdateMonitorInfo = false;
	motionControllerModule.monitorMotionIoStateThread->setRunning(false);
	this->ui->cbox_debugMode->setChecked(false);
	cbox_debugMode_checked(false);

	this->close();
}

void DlgProductSet::pbtn_shangxianwei1_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_shangxianwei1->setText(value);
		setConfig.shangXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiaxianwei1_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_xiaxianwei1->setText(value);
		setConfig.xiaXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_zuoxianwei1_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zuoxianwei1->setText(value);
		setConfig.zuoXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_youxianwei1_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_youxianwei1->setText(value);
		setConfig.youXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiangsudangliang1_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_xiangsudangliang1->setText(value);
		setConfig.xiangSuDangLiang1 = value.toDouble();
		emit pixToWorldChanged();
	}
}

void DlgProductSet::pbtn_shangxianwei2_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_shangxianwei2->setText(value);
		setConfig.shangXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiaxianwei2_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_xiaxianwei2->setText(value);
		setConfig.xiaXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_zuoxianwei2_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zuoxianwei2->setText(value);
		setConfig.zuoXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_youxianwei2_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_youxianwei2->setText(value);
		setConfig.youXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiangsudangliang2_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_xiangsudangliang2->setText(value);
		setConfig.xiangSuDangLiang2 = value.toDouble();
		emit pixToWorldChanged();
	}
}

void DlgProductSet::pbtn_zhongyangfenjiexian1_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 )
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zhongyangfenjiexian1->setText(value);
		setConfig.zhongyangxianwei1 = value.toInt();
	}
}

void DlgProductSet::pbtn_zhongyangfenjiexian2_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 )
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zhongyangfenjiexian2->setText(value);
		setConfig.zhongyangxianwei2 = value.toInt();
	}
}

void DlgProductSet::pbtn_qiangbaoguang_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 1000)
		{
			QMessageBox::warning(this, "提示", "请输入0-1000的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_qiangbaoguang->setText(value);
		setConfig.qiangbaoguang = value.toInt();
	}
}

void DlgProductSet::pbtn_qiangzengyi_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 15)
		{
			QMessageBox::warning(this, "提示", "请输入0-15的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_qiangzengyi->setText(value);
		setConfig.qiangzengyi = value.toInt();
	}
}

void DlgProductSet::pbtn_zhongbaoguang_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 1000)
		{
			QMessageBox::warning(this, "提示", "请输入0-1000的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zhongbaoguang->setText(value);
		setConfig.zhongbaoguang = value.toInt();
	}
}

void DlgProductSet::pbtn_zhongzengyi_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 15)
		{
			QMessageBox::warning(this, "提示", "请输入0-15的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zhongzengyi->setText(value);
		setConfig.zhongzengyi = value.toInt();
	}
}

void DlgProductSet::pbtn_ruobaoguang_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 1000)
		{
			QMessageBox::warning(this, "提示", "请输入0-1000的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_ruobaoguang->setText(value);
		setConfig.ruobaoguang = value.toInt();
	}
}

void DlgProductSet::pbtn_ruozengyi_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 15)
		{
			QMessageBox::warning(this, "提示", "请输入0-15的数值");
			return;
		}
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_ruozengyi->setText(value);
		setConfig.ruozengyi = value.toInt();
	}
}

void DlgProductSet::cBox_takeNgPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveNGImg = ui->cBox_takeNgPictures->isChecked();
}

void DlgProductSet::cBox_takeMaskPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveMaskImg = ui->cBox_takeMaskPictures->isChecked();
}

void DlgProductSet::cBox_takeOkPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveOKImg = ui->cBox_takeOkPictures->isChecked();
}

void DlgProductSet::cbox_debugMode_checked(bool ischecked)
{
	auto& isDebug = Modules::getInstance().configManagerModule.setConfig.debugMode;
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	isDebug = ischecked;
	isDebugIO = ischecked;
	if (isDebugIO)
	{
		setAllDOCheckBoxCheckedToFalse();

		setAllDOCheckBoxEnableToTrue();

		monitorZMotionMonitorThread->setRunning(false);
	}
	else
	{
		setAllDOCheckBoxCheckedToFalse();

		setAllDOCheckBoxEnableToFalse();

		monitorZMotionMonitorThread->setRunning(true);
	}
}

void DlgProductSet::cbox_DOpaizhao1_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOpaizhao1, isChecked);
	}
}

void DlgProductSet::cbox_DOpaizhao2_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOpaizhao2, isChecked);
	}
}

void DlgProductSet::cbox_DOguangyuan_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOguangyuan, isChecked);
	}
}

void DlgProductSet::cbox_DObaojing_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DObaojing, isChecked);
	}
}

void DlgProductSet::cbox_DOtingji_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOtingji, isChecked);
	}
}

void DlgProductSet::cbox_DOxiachaqigangwai1_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOxiachaqigangwai1, isChecked);
	}
}

void DlgProductSet::cbox_DOxiachaqigangwai2_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOxiachaqigangwai2, isChecked);
	}
}

void DlgProductSet::cbox_DOxiachaqigangnei1_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOxiachaqigangnei1, isChecked);
	}
}

void DlgProductSet::cbox_DOxiachaqigangnei2_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOxiachaqigangnei2, isChecked);
	}
}

void DlgProductSet::cbox_DOboliaoqigangwai1_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOboliaoqigangwai1, isChecked);
	}
}

void DlgProductSet::cbox_DOboliaoqigangwai2_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOboliaoqigangwai2, isChecked);
	}
}

void DlgProductSet::cbox_DOboliaoqigangnei1_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOboliaoqigangnei1, isChecked);
	}
}

void DlgProductSet::cbox_DOboliaoqigangnei2_clicked(bool isChecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::DOboliaoqigangnei2, isChecked);
	}
}

void DlgProductSet::cbox_DOtifeidongzuowai1_clicked(bool isChecked)
{
	if (!isDebugIO) return;
	if (this->property("tifei1waiBusy").toBool()) return;
	this->setProperty("tifei1waiBusy", true);

	const int intervalMs = 100;

	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	const QVector<StepEx> steps = {
		{ ControlLines::DOxiachaqigangwai1, true,  setConfig.yanshixiachawai1 },
		{ ControlLines::DOboliaoqigangwai1, true,  setConfig.boliaoshijianwai1 },
		{ ControlLines::DOxiachaqigangwai1, false, setConfig.yanshixiachawai1 },
		{ ControlLines::DOboliaoqigangwai1, false, setConfig.boliaoshijianwai1 },
	};

	runZMotionSequenceEx(this, steps, [this]() {
		this->setProperty("tifei1waiBusy", false);
		});
}

void DlgProductSet::cbox_DOtifeidongzuowai2_clicked(bool isChecked)
{
	if (!isDebugIO) return;
	if (this->property("tifeiwai2Busy").toBool()) return;
	this->setProperty("tifeiwai2Busy", true);
	const int intervalMs = 100;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	const QVector<StepEx> steps = {
		{ ControlLines::DOxiachaqigangwai2, true,  setConfig.yanshixiachawai2 },
		{ ControlLines::DOboliaoqigangwai2, true,  setConfig.boliaoshijianwai2 },
		{ ControlLines::DOxiachaqigangwai2, false, setConfig.yanshixiachawai2 },
		{ ControlLines::DOboliaoqigangwai2, false, setConfig.boliaoshijianwai2 },
	};
	runZMotionSequenceEx(this, steps, [this]() {
		this->setProperty("tifeiwai2Busy", false);
		});
}

void DlgProductSet::cbox_DOtifeidongzuonei1_clicked(bool isChecked)
{
	if (!isDebugIO) return;
	if (this->property("tifeinei1Busy").toBool()) return;
	this->setProperty("tifeinei1Busy", true);
	const int intervalMs = 100;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	const QVector<StepEx> steps = {
		{ ControlLines::DOxiachaqigangnei1, true,  setConfig.yanshixiachanei1 },
		{ ControlLines::DOboliaoqigangnei1, true,  setConfig.boliaoshijiannei1 },
		{ ControlLines::DOxiachaqigangnei1, false, setConfig.yanshixiachanei1 },
		{ ControlLines::DOboliaoqigangnei1, false, setConfig.boliaoshijiannei1 },
	};
	runZMotionSequenceEx(this, steps, [this]() {
		this->setProperty("tifeinei1Busy", false);
		});
}

void DlgProductSet::cbox_DOtifeidongzuonei2_clicked(bool isChecked)
{
	if (!isDebugIO) return;
	if (this->property("tifeinei2Busy").toBool()) return;
	this->setProperty("tifeinei2Busy", true);
	const int intervalMs = 100;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	const QVector<StepEx> steps = {
		{ ControlLines::DOxiachaqigangnei2, true,  setConfig.yanshixiachanei2 },
		{ ControlLines::DOboliaoqigangnei2, true,  setConfig.boliaoshijiannei2 },
		{ ControlLines::DOxiachaqigangnei2, false, setConfig.yanshixiachanei2 },
		{ ControlLines::DOboliaoqigangnei2, false, setConfig.boliaoshijiannei2 },
	};
	runZMotionSequenceEx(this, steps, [this]() {
		this->setProperty("tifeinei2Busy", false);
		});
}

void DlgProductSet::btn_setDOpaizhao1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOpaizhao1->setText(value);
		ControlLines::DOpaizhao1 = value.toInt();
		setConfig.DOpaizhao1 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOpaizhao2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOpaizhao2->setText(value);
		ControlLines::DOpaizhao2 = value.toInt();
		setConfig.DOpaizhao2 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOguangyuan_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOguangyuan->setText(value);
		ControlLines::DOguangyuan = value.toInt();
		setConfig.DOguangyuan = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDObaojing_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDObaojing->setText(value);
		ControlLines::DObaojing = value.toInt();
		setConfig.DObaojing = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOtingji_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOtingji->setText(value);
		ControlLines::DOtingji = value.toInt();
		setConfig.DOtingji = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOxiachaqigangwai1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOxiachaqigangwai1->setText(value);
		ControlLines::DOxiachaqigangwai1 = value.toInt();
		setConfig.DOxiachaqigangwai1 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOxiachaqigangnei1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOxiachaqigangnei1->setText(value);
		ControlLines::DOxiachaqigangnei1 = value.toInt();
		setConfig.DOxiachaqigangnei1 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOxiachaqigangwai2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOxiachaqigangwai2->setText(value);
		ControlLines::DOxiachaqigangwai2 = value.toInt();
		setConfig.DOxiachaqigangwai2 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOxiachaqigangnei2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOxiachaqigangnei2->setText(value);
		ControlLines::DOxiachaqigangnei2 = value.toInt();
		setConfig.DOxiachaqigangnei2 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOboliaoqigangwai1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOboliaoqigangwai1->setText(value);
		ControlLines::DOboliaoqigangwai1 = value.toInt();
		setConfig.DOboliaoqigangwai1 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOboliaoqigangnei1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOboliaoqigangnei1->setText(value);
		ControlLines::DOboliaoqigangnei1 = value.toInt();
		setConfig.DOboliaoqigangnei1 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOboliaoqigangwai2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOboliaoqigangwai2->setText(value);
		ControlLines::DOboliaoqigangwai2 = value.toInt();
		setConfig.DOboliaoqigangwai2 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::btn_setDOboliaoqigangnei2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入0-100的数值");
			return;
		}
		ui->btn_setDOboliaoqigangnei2->setText(value);
		ControlLines::DOboliaoqigangnei2 = value.toInt();
		setConfig.DOboliaoqigangnei2 = value.toInt();
		auto duplicateIndices = DOFindAllDuplicateIndices();
		setDOErrorInfo(duplicateIndices);
		updateMonitorIOThread();
	}
}

void DlgProductSet::cBox_takeCamera1Pictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.takeWork1Pictures = ui->cBox_takeCamera1Pictures->isChecked();
}

void DlgProductSet::cBox_takeCamera2Pictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.takeWork2Pictures = ui->cBox_takeCamera2Pictures->isChecked();
}

void DlgProductSet::rbtn_saveJpeg_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;

	imageSaveEngine->setSaveImgFormat(rw::rqw::ImageSaveFormat::JPEG);
	setConfig.isSaveJPEG = true;
	setConfig.isSavePNG = false;
	setConfig.isSaveBMP = false;
}

void DlgProductSet::rbtn_saveBmp_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;

	imageSaveEngine->setSaveImgFormat(rw::rqw::ImageSaveFormat::BMP);
	setConfig.isSaveJPEG = false;
	setConfig.isSavePNG = false;
	setConfig.isSaveBMP = true;
}

void DlgProductSet::rbtn_savePng_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;

	imageSaveEngine->setSaveImgFormat(rw::rqw::ImageSaveFormat::PNG);
	setConfig.isSaveJPEG = false;
	setConfig.isSavePNG = true;
	setConfig.isSaveBMP = false;
}

void DlgProductSet::btn_saveQuality_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 1 || value.toInt() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入1-100的数值");
			return;
		}
		ui->btn_saveQuality->setText(value);
		setConfig.saveImgQuality = value.toInt();
		auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
		imageSaveEngine->setSaveImgQuality(setConfig.saveImgQuality);
	}
}

void DlgProductSet::cbox_qiyongerxiangji_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.qiyongerxiangji = ui->cbox_qiyongerxiangji->isChecked();
}

void DlgProductSet::cbox_qiyongyundongkongzhiqi_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.qiyongyundongkongzhiqi = ui->cbox_qiyongyundongkongzhiqi->isChecked();
}

void DlgProductSet::cbox_yundongkongzhiqichonglian_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.yundongkongzhiqichonglian = ui->cbox_yundongkongzhiqichonglian->isChecked();
}

void DlgProductSet::btn_yanshixiachawai1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_yanshixiachawai1->setText(value);
		setConfig.yanshixiachawai1 = value.toDouble();
	}
}

void DlgProductSet::btn_boliaoshijianwai1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_boliaoshijianwai1->setText(value);
		setConfig.boliaoshijianwai1 = value.toDouble();
	}
}

void DlgProductSet::btn_yanshixiachawai2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_yanshixiachawai2->setText(value);
		setConfig.yanshixiachawai2 = value.toDouble();
	}
}

void DlgProductSet::btn_boliaoshijianwai2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_boliaoshijianwai2->setText(value);
		setConfig.boliaoshijianwai2 = value.toDouble();
	}
}

void DlgProductSet::btn_yanshixiachanei1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_yanshixiachanei1->setText(value);
		setConfig.yanshixiachanei1 = value.toDouble();
	}
}

void DlgProductSet::btn_boliaoshijiannei1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_boliaoshijiannei1->setText(value);
		setConfig.boliaoshijiannei1 = value.toDouble();
	}
}

void DlgProductSet::btn_yanshixiachanei2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_yanshixiachanei2->setText(value);
		setConfig.yanshixiachanei2 = value.toDouble();
	}
}

void DlgProductSet::btn_boliaoshijiannei2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_boliaoshijiannei2->setText(value);
		setConfig.boliaoshijiannei2 = value.toDouble();
	}
}

void DlgProductSet::btn_xiangjichufachangdu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_xiangjichufachangdu->setText(value);
		setConfig.xiangjichufachangdu = value.toDouble();
		auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;

		bool isSet = zmotion->setModbus(4, 1, value.toFloat());

		if (!isSet)
		{
			QMessageBox::warning(this, "警告", "设定拉袋长度失败!");
		}
	}
}

void DlgProductSet::btn_meizhuanmaichongshu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_meizhuanmaichongshu->setText(value);
		setConfig.meizhuanmaichongshu = value.toDouble();
	}
}

void DlgProductSet::btn_shedingzhouchang_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_shedingzhouchang->setText(value);
		setConfig.shedingzhouchang = value.toDouble();
	}
}

void DlgProductSet::btn_waicetifeijuli1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_waicetifeijuli1->setText(value);
		setConfig.waicetifeijuli1 = value.toDouble();
	}
}

void DlgProductSet::btn_neicetifeijuli1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

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
		ui->btn_neicetifeijuli1->setText(value);
		setConfig.neicetifeijuli1 = value.toDouble();
	}
}

void DlgProductSet::btn_rongyufangdou1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

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
		ui->btn_rongyufangdou1->setText(value);
		setConfig.tifeirongyufangdou1 = value.toDouble();
	}
}

void DlgProductSet::btn_waicetifeijuli2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

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
		ui->btn_waicetifeijuli2->setText(value);
		setConfig.waicetifeijuli2 = value.toDouble();
	}
}

void DlgProductSet::btn_neicetifeijuli2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

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
		ui->btn_neicetifeijuli2->setText(value);
		setConfig.neicetifeijuli2 = value.toDouble();
	}
}

void DlgProductSet::btn_rongyufangdou2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

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
		ui->btn_rongyufangdou2->setText(value);
		setConfig.tifeirongyufangdou2 = value.toDouble();
	}
}

void DlgProductSet::btn_tuxiangshunshizhenxuanzhuancishu1_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0)
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		ui->btn_tuxiangshunshizhenxuanzhuancishu1->setText(value);
		setConfig.tuxiangshunshizhenxuanzhuancishu1 = value.toInt();
	}
}

void DlgProductSet::btn_tuxiangshunshizhenxuanzhuancishu2_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toInt() < 0)
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		ui->btn_tuxiangshunshizhenxuanzhuancishu2->setText(value);
		setConfig.tuxiangshunshizhenxuanzhuancishu2 = value.toInt();
	}
}

void DlgProductSet::btn_xunzhaoquexiansuoshubashoudepianchazhi_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_xunzhaoquexiansuoshubashoudepianchazhi->setText(value);
		setConfig.xunzhaoquexiansuoshubashoudepianchazhi = value.toDouble();
	}
}

void DlgProductSet::tabWidget_indexChanged(int index)
{
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	auto& _isUpdateMonitorInfo = Modules::getInstance().motionControllerModule._isUpdateMonitorInfo;

	switch (index) {
	case 0:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 1:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 2:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 3:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 4:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 5:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 6:
		_isUpdateMonitorInfo = true;
		monitorZMotionMonitorThread->setRunning(true);
		break;
	case 7:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 8:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	default:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	}
}

void DlgProductSet::ckb_tangshang_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.istangshang = isChecked;
}

void DlgProductSet::ckb_queliao_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isqueliao = isChecked;
}

void DlgProductSet::ckb_yiwu_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isyiwu = isChecked;
}

void DlgProductSet::ckb_zangwu_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.iszangwu = isChecked;
}

void DlgProductSet::ckb_extra2_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra2 = isChecked;
}

void DlgProductSet::ckb_extra3_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra3 = isChecked;
}

void DlgProductSet::ckb_extra4_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra4 = isChecked;
}

void DlgProductSet::ckb_extra5_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra5 = isChecked;
}

void DlgProductSet::ckb_extra6_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra6 = isChecked;
}

void DlgProductSet::ckb_extra7_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra7 = isChecked;
}

void DlgProductSet::ckb_extra8_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra8 = isChecked;
}

void DlgProductSet::ckb_extra9_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	setConfig.isextra9 = isChecked;
}

void DlgProductSet::monitorInPutSignal(size_t index, bool state)
{
	if (isDebugIO) return;
	for (const auto& item : DICheckItems)
	{
		if (index == *item.line)
		{
			if (item.box)
			{
				QSignalBlocker blocker(item.box);
				item.box->setChecked(state);
			}
			break;
		}
	}
}

void DlgProductSet::monitorOutPutSignal(size_t index, bool state)
{
	if (isDebugIO) return;

	for (const auto& item : DOCheckItems)
	{
		if (index == *item.line)
		{
			if (item.box)
			{
				QSignalBlocker blocker(item.box);
				item.box->setChecked(state);
			}
			break;
		}
	}
}

void DlgProductSet::updateMonitorIOThread()
{
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	QVector<size_t> monitorIList = { };
	QVector<size_t> monitorOList = { ControlLines::DOpaizhao1,
		ControlLines::DOpaizhao2,ControlLines::DOguangyuan,
		ControlLines::DObaojing,ControlLines::DOtingji,
		ControlLines::DOxiachaqigangwai1,ControlLines::DOxiachaqigangwai2,
		ControlLines::DOxiachaqigangnei1,ControlLines::DOxiachaqigangnei2,
		ControlLines::DOboliaoqigangwai1,ControlLines::DOboliaoqigangwai2,
		ControlLines::DOboliaoqigangnei1,ControlLines::DOboliaoqigangnei2
	};
	monitorZMotionMonitorThread->setMonitorIList(monitorIList);
	monitorZMotionMonitorThread->setMonitorOList(monitorOList);
}




