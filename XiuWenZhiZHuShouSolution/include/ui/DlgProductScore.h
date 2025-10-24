#pragma once

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class DlgProductScoreClass; };
QT_END_NAMESPACE

class DlgProductScore : public QDialog
{
	Q_OBJECT

public:
	DlgProductScore(QWidget* parent = nullptr);
	~DlgProductScore();

public:
	void build_ui();
	void read_config();
	void build_connect();

private slots:
	void pbtn_close_clicked();

	void ckb_tangshangEnable_checked();
	void btn_tangshangSimilarity_clicked();
	void btn_tangshangArea_clicked();
	void ckb_queliaoEnable_checked();
	void btn_queliaoSimilarity_clicked();
	void btn_queliaoArea_clicked();
	void ckb_yiwuEnable_checked();
	void btn_yiwuSimilarity_clicked();
	void btn_yiwuArea_clicked();
	void ckb_zangwuEnable_checked();
	void btn_zangwuSimilarity_clicked();
	void btn_zangwuArea_clicked();
	void ckb_zhanlianEnable_checked();
	void btn_zhanlianSimilarity_clicked();
	void btn_zhanlianArea_clicked();
	void ckb_feiliaoEnable_checked();
	void btn_feiliaoSimilarity_clicked();
	void btn_feiliaoArea_clicked();
	void ckb_mojucuowu_checked();
	void btn_mojucuowuSimilarity_clicked();
	void btn_mojucuowuArea_clicked();

signals:
	void scoreFormClosed();

public:
	Ui::DlgProductScoreClass* ui;
};

