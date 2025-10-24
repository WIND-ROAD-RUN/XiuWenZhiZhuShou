#pragma once

#include <QCheckBox>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DlgProductSetClass; };
QT_END_NAMESPACE

class DlgProductSet : public QDialog
{
	Q_OBJECT

public:
	DlgProductSet(QWidget* parent = nullptr);
	~DlgProductSet();

protected:
	void showEvent(QShowEvent* event) override;

public:
	void build_ui();
	void read_config();
	void build_connect();

private:
	struct DOCheckItem {
		size_t* line;      
		QCheckBox* box;    
	};
	std::vector<DOCheckItem> DOCheckItems;
	void initDOCheckItems();
	std::vector<DOCheckItem> DICheckItems;
	void initDICheckItems();

	std::vector<std::vector<int>> DOFindAllDuplicateIndices();
	void setDOErrorInfo(const std::vector<std::vector<int>>& index);
	void setDOErrorInfo(int index);
	std::vector<std::vector<int>> DIFindAllDuplicateIndices();
	void setDIErrorInfo(const std::vector<std::vector<int>>& index);
	void setDIErrorInfo(int index);

	void setAllDOCheckBoxCheckedToFalse();
	void setAllDOCheckBoxEnableToFalse();
	void setAllDOCheckBoxEnableToTrue();

	void updateControlLinesFromCheckBoxes();
signals:
	void pixToWorldChanged();
	void tifeijuliChanged();

private:
	// IO监控页面的调试模式
	bool isDebugIO{ false };

private slots:
	void pbtn_close_clicked();

	// 基本功能
	void cbox_qiyongerxiangji_checked();
	void cbox_qiyongyundongkongzhiqi_checked();
	void cbox_yundongkongzhiqichonglian_checked();

	// 基本参数
	void btn_yanshixiachawai1_clicked();
	void btn_boliaoshijianwai1_clicked();

	void btn_yanshixiachawai2_clicked();
	void btn_boliaoshijianwai2_clicked();

	void btn_yanshixiachanei1_clicked();
	void btn_boliaoshijiannei1_clicked();

	void btn_yanshixiachanei2_clicked();
	void btn_boliaoshijiannei2_clicked();

	void btn_xiangjichufachangdu_clicked();
	void btn_meizhuanmaichongshu_clicked();
	void btn_shedingzhouchang_clicked();

	// 工位剔废参数
	void btn_waicetifeijuli1_clicked();
	void btn_neicetifeijuli1_clicked();
	void btn_rongyufangdou1_clicked();
	void btn_waicetifeijuli2_clicked();
	void btn_neicetifeijuli2_clicked();
	void btn_rongyufangdou2_clicked();

	void btn_tuxiangshunshizhenxuanzhuancishu1_clicked();
	void btn_tuxiangshunshizhenxuanzhuancishu2_clicked();

	void btn_xunzhaoquexiansuoshubashoudepianchazhi_clicked();

	// 工作限位设置
	void pbtn_shangxianwei1_clicked();
	void pbtn_xiaxianwei1_clicked();
	void pbtn_zuoxianwei1_clicked();
	void pbtn_youxianwei1_clicked();
	void pbtn_xiangsudangliang1_clicked();
	void pbtn_shangxianwei2_clicked();
	void pbtn_xiaxianwei2_clicked();
	void pbtn_zuoxianwei2_clicked();
	void pbtn_youxianwei2_clicked();
	void pbtn_xiangsudangliang2_clicked();
	void pbtn_zhongyangfenjiexian1_clicked();
	void pbtn_zhongyangfenjiexian2_clicked();

	// 光源设置
	void pbtn_qiangbaoguang_clicked();
	void pbtn_qiangzengyi_clicked();

	void pbtn_zhongbaoguang_clicked();
	void pbtn_zhongzengyi_clicked();

	void pbtn_ruobaoguang_clicked();
	void pbtn_ruozengyi_clicked();

	// 图像存储设置
	void cBox_takeNgPictures_checked();
	void cBox_takeMaskPictures_checked();
	void cBox_takeOkPictures_checked();
	void cBox_takeCamera1Pictures_checked();
	void cBox_takeCamera2Pictures_checked();

	void rbtn_saveJpeg_checked(bool isChecked);
	void rbtn_saveBmp_checked(bool isChecked);
	void rbtn_savePng_checked(bool isChecked);
	void btn_saveQuality_clicked();

	// 监控IO
	void cbox_debugMode_checked(bool ischecked);

	void cbox_DOpaizhao1_clicked(bool isChecked);
	void cbox_DOpaizhao2_clicked(bool isChecked);
	void cbox_DOguangyuan_clicked(bool isChecked);
	void cbox_DObaojing_clicked(bool isChecked);
	void cbox_DOtingji_clicked(bool isChecked);

	void cbox_DOxiachaqigangwai1_clicked(bool isChecked);
	void cbox_DOxiachaqigangwai2_clicked(bool isChecked);
	void cbox_DOxiachaqigangnei1_clicked(bool isChecked);
	void cbox_DOxiachaqigangnei2_clicked(bool isChecked);

	void cbox_DOboliaoqigangwai1_clicked(bool isChecked);
	void cbox_DOboliaoqigangwai2_clicked(bool isChecked);
	void cbox_DOboliaoqigangnei1_clicked(bool isChecked);
	void cbox_DOboliaoqigangnei2_clicked(bool isChecked);

	void cbox_DOtifeidongzuowai1_clicked(bool isChecked);
	void cbox_DOtifeidongzuowai2_clicked(bool isChecked);
	void cbox_DOtifeidongzuonei1_clicked(bool isChecked);
	void cbox_DOtifeidongzuonei2_clicked(bool isChecked);

	// 设置IO
	void btn_setDOpaizhao1_clicked();
	void btn_setDOpaizhao2_clicked();
	void btn_setDOguangyuan_clicked();
	void btn_setDObaojing_clicked();
	void btn_setDOtingji_clicked();

	void btn_setDOxiachaqigangwai1_clicked();
	void btn_setDOxiachaqigangnei1_clicked();
	void btn_setDOxiachaqigangwai2_clicked();
	void btn_setDOxiachaqigangnei2_clicked();

	void btn_setDOboliaoqigangwai1_clicked();
	void btn_setDOboliaoqigangnei1_clicked();
	void btn_setDOboliaoqigangwai2_clicked();
	void btn_setDOboliaoqigangnei2_clicked();

	void tabWidget_indexChanged(int index);

	// 分数界面内容可选显示
	void ckb_tangshang_checked(bool isChecked);
	void ckb_queliao_checked(bool isChecked);
	void ckb_yiwu_checked(bool isChecked);

	void ckb_zangwu_checked(bool isChecked);
	void ckb_extra2_checked(bool isChecked);
	void ckb_extra3_checked(bool isChecked);
	void ckb_extra4_checked(bool isChecked);
	void ckb_extra5_checked(bool isChecked);
	void ckb_extra6_checked(bool isChecked);
	void ckb_extra7_checked(bool isChecked);
	void ckb_extra8_checked(bool isChecked);
	void ckb_extra9_checked(bool isChecked);

public slots:
	// 监控IO函数
	void monitorInPutSignal(size_t index, bool state);
	void monitorOutPutSignal(size_t index, bool state);

private:
	void updateMonitorIOThread();
public:
	Ui::DlgProductSetClass* ui;
};

