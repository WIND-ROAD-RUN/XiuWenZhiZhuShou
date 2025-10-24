#include "DlgProductScore.h"

#include <QMessageBox>

#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include "ui_DlgProductScore.h"

DlgProductScore::DlgProductScore(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductScoreClass())
{
	ui->setupUi(this);

	build_ui();

	build_connect();
}

DlgProductScore::~DlgProductScore()
{
	delete ui;
}

void DlgProductScore::build_ui()
{
	read_config();
}

void DlgProductScore::read_config()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;

	// 初始化参数

	// 烫伤
	ui->ckb_tangshang->setChecked(scoreConfig.tangShang);
	ui->btn_tangshangscore->setText(QString::number(scoreConfig.tangShangScore));
	ui->btn_tangshangarea->setText(QString::number(scoreConfig.tangShangArea));

	// 缺料
	ui->ckb_queliao->setChecked(scoreConfig.queLiao);
	ui->btn_queliaoscore->setText(QString::number(scoreConfig.queLiaoScore));
	ui->btn_queliaoarea->setText(QString::number(scoreConfig.queLiaoArea));

	// 异物
	ui->ckb_yiwu->setChecked(scoreConfig.yiWu);
	ui->btn_yiwuscore->setText(QString::number(scoreConfig.yiWuScore));
	ui->btn_yiwuarea->setText(QString::number(scoreConfig.yiWuArea));

	// 脏污
	ui->ckb_zangwu->setChecked(scoreConfig.zangWu);
	ui->btn_zangwuscore->setText(QString::number(scoreConfig.zangWuScore));
	ui->btn_zangwuarea->setText(QString::number(scoreConfig.zangWuArea));

	// 粘连
	ui->ckb_zhanlian->setChecked(scoreConfig.zhanlian);
	ui->btn_zhanlianScore->setText(QString::number(scoreConfig.zhanlianScore));
	ui->btn_zhanlianArea->setText(QString::number(scoreConfig.zhanlianArea));

	// 废料
	ui->ckb_feiliao->setChecked(scoreConfig.feiliao);
	ui->btn_feiliaoScore->setText(QString::number(scoreConfig.feiliaoScore));
	ui->btn_feiliaoArea->setText(QString::number(scoreConfig.feiliaoArea));

	// 模具错误
	ui->ckb_mojucuowu->setChecked(scoreConfig.mojucuowu);
	ui->btn_mojucuowuscore->setText(QString::number(scoreConfig.mojucuowuScore));
	ui->btn_mojucuowuArea->setText(QString::number(scoreConfig.mojucuowuArea));
}

void DlgProductScore::build_connect()
{
	QObject::connect(ui->btn_close, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_close_clicked);
	QObject::connect(ui->ckb_tangshang, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_tangshangEnable_checked);
	QObject::connect(ui->btn_tangshangscore, &QPushButton::clicked,
		this, &DlgProductScore::btn_tangshangSimilarity_clicked);
	QObject::connect(ui->btn_tangshangarea, &QPushButton::clicked,
		this, &DlgProductScore::btn_tangshangArea_clicked);
	QObject::connect(ui->ckb_queliao, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_queliaoEnable_checked);
	QObject::connect(ui->btn_queliaoscore, &QPushButton::clicked,
		this, &DlgProductScore::btn_queliaoSimilarity_clicked);
	QObject::connect(ui->btn_queliaoarea, &QPushButton::clicked,
		this, &DlgProductScore::btn_queliaoArea_clicked);
	QObject::connect(ui->ckb_yiwu, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_yiwuEnable_checked);
	QObject::connect(ui->btn_yiwuscore, &QPushButton::clicked,
		this, &DlgProductScore::btn_yiwuSimilarity_clicked);
	QObject::connect(ui->btn_yiwuarea, &QPushButton::clicked,
		this, &DlgProductScore::btn_yiwuArea_clicked);

	QObject::connect(ui->ckb_zangwu, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_zangwuEnable_checked);
	QObject::connect(ui->btn_zangwuscore, &QPushButton::clicked,
		this, &DlgProductScore::btn_zangwuSimilarity_clicked);
	QObject::connect(ui->btn_zangwuarea, &QPushButton::clicked,
		this, &DlgProductScore::btn_zangwuArea_clicked);

	QObject::connect(ui->ckb_zhanlian, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_zhanlianEnable_checked);
	QObject::connect(ui->btn_zhanlianScore, &QPushButton::clicked,
		this, &DlgProductScore::btn_zhanlianSimilarity_clicked);
	QObject::connect(ui->btn_zhanlianArea, &QPushButton::clicked,
		this, &DlgProductScore::btn_zhanlianArea_clicked);

	QObject::connect(ui->ckb_feiliao, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_feiliaoEnable_checked);
	QObject::connect(ui->btn_feiliaoScore, &QPushButton::clicked,
		this, &DlgProductScore::btn_feiliaoSimilarity_clicked);
	QObject::connect(ui->btn_feiliaoArea, &QPushButton::clicked,
		this, &DlgProductScore::btn_feiliaoArea_clicked);

	QObject::connect(ui->ckb_mojucuowu, &QCheckBox::clicked,
		this, &DlgProductScore::ckb_mojucuowu_checked);
	QObject::connect(ui->btn_mojucuowuscore, &QPushButton::clicked,
		this, &DlgProductScore::btn_mojucuowuSimilarity_clicked);
	QObject::connect(ui->btn_mojucuowuArea, &QPushButton::clicked,
		this, &DlgProductScore::btn_mojucuowuArea_clicked);
}

void DlgProductScore::pbtn_close_clicked()
{
	emit scoreFormClosed();
	this->close();
}

void DlgProductScore::ckb_tangshangEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.tangShang = ui->ckb_tangshang->isChecked();
}

void DlgProductScore::btn_tangshangSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_tangshangscore->setText(value);
		scoreConfig.tangShangScore = value.toDouble();
	}
}

void DlgProductScore::btn_tangshangArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_tangshangarea->setText(value);
		scoreConfig.tangShangArea = value.toDouble();
	}
}

void DlgProductScore::ckb_queliaoEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.queLiao = ui->ckb_queliao->isChecked();
}

void DlgProductScore::btn_queliaoSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_queliaoscore->setText(value);
		scoreConfig.queLiaoScore = value.toDouble();
	}
}

void DlgProductScore::btn_queliaoArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_queliaoarea->setText(value);
		scoreConfig.queLiaoArea = value.toDouble();
	}
}

void DlgProductScore::ckb_yiwuEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.yiWu = ui->ckb_yiwu->isChecked();
}

void DlgProductScore::btn_yiwuSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_yiwuscore->setText(value);
		scoreConfig.yiWuScore = value.toDouble();
	}
}

void DlgProductScore::btn_yiwuArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_yiwuarea->setText(value);
		scoreConfig.yiWuArea = value.toDouble();
	}
}

void DlgProductScore::ckb_zangwuEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.zangWu = ui->ckb_zangwu->isChecked();
}

void DlgProductScore::btn_zangwuSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_zangwuscore->setText(value);
		scoreConfig.zangWuScore = value.toDouble();
	}
}

void DlgProductScore::btn_zangwuArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_zangwuarea->setText(value);
		scoreConfig.zangWuArea = value.toDouble();
	}
}

void DlgProductScore::ckb_zhanlianEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.zhanlian = ui->ckb_zhanlian->isChecked();
}

void DlgProductScore::btn_zhanlianSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_zhanlianScore->setText(value);
		scoreConfig.zhanlianScore = value.toDouble();
	}
}

void DlgProductScore::btn_zhanlianArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_zhanlianArea->setText(value);
		scoreConfig.zhanlianArea = value.toDouble();
	}
}

void DlgProductScore::ckb_feiliaoEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.feiliao = ui->ckb_feiliao->isChecked();
}

void DlgProductScore::btn_feiliaoSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_feiliaoScore->setText(value);
		scoreConfig.feiliaoScore = value.toDouble();
	}
}

void DlgProductScore::btn_feiliaoArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_feiliaoArea->setText(value);
		scoreConfig.feiliaoArea = value.toDouble();
	}
}

void DlgProductScore::ckb_mojucuowu_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.mojucuowu = ui->ckb_mojucuowu->isChecked();
}

void DlgProductScore::btn_mojucuowuSimilarity_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0 || value.toDouble() > 100)
		{
			QMessageBox::warning(this, "提示", "请输入[0,100]]的数值");
			return;
		}
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_mojucuowuscore->setText(value);
		scoreConfig.mojucuowuScore = value.toDouble();
	}
}

void DlgProductScore::btn_mojucuowuArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->btn_mojucuowuArea->setText(value);
		scoreConfig.mojucuowuArea = value.toDouble();
	}
}



