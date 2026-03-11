#include "zhangdanWidget.h"
#include "SignalRelay.h"
#include "MaskWidget.h"
#include "SignalSend.h"
#include <QLabel>
#include <QDir>
#include <QDate>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QPushButton>
#include <QDebug>

#define LABEL_NUMS 4

/*
* 对象的初始化顺序是按照成员变量在类中声明的顺序进行初始化的（而不是初始化列表中的顺序）
*/

zhangdanWidget::zhangdanWidget(QWidget *parent)
	: QWidget(parent), _Path("./config/SNKCoin.ini"), settings(new QSettings(_Path, QSettings::IniFormat))
{
	ui.setupUi(this);
	Init();
}

zhangdanWidget::~zhangdanWidget()
{
	if (labelDate)
	{
		delete labelDate;
		labelDate = nullptr;
	}
	if (labelDateStr)
	{
		delete labelDateStr;
		labelDateStr = nullptr;
	}
	if (labelDateEnd)
	{
		delete labelDateEnd;
		labelDateEnd = nullptr;
	}
	if (labelDateEndStr)
	{
		delete labelDateEndStr;
		labelDateEndStr = nullptr;
	}
	if (labelCoins)
	{
		delete labelCoins;
		labelCoins = nullptr;
	}
	if (labelCoinNums)
	{
		delete labelCoinNums;
		labelCoinNums = nullptr;
	}
	if (labelGames)
	{
		delete labelGames;
		labelGames = nullptr;
	}
	if (labelGameNums)
	{
		delete labelGameNums;
		labelGameNums = nullptr;
	}
}

// 重置配置文件
void zhangdanWidget::resetConfig()
{
	settings->beginGroup(QString("Date"));
	settings->setValue(QString("DateStart"), QDate::currentDate().toString("yyyy-MM-dd"));
	settings->setValue(QString("DateEnd"),  QDate::currentDate().addMonths(1).toString("yyyy-MM-dd"));
	settings->setValue(QString("CoinNums"), 0);
	settings->setValue(QString("GameNums"), 0);
	settings->endGroup();
	updateDateUI();
}

void zhangdanWidget::Init()
{
	initSignalRelay();												// 初始化信号变量中转
	initSignalSend();												// 初始化信号发射器
	initConfig();													// 初始化配置文件
	initUI();														// 初始化UI界面
	BindSlots();													// 绑定槽函数
}

void zhangdanWidget::initSignalRelay()
{
	signalRelay = SignalRelay::GetInstance();
}

void zhangdanWidget::initSignalSend()
{
	signalSend = SignalSend::GetInstance();
}

void zhangdanWidget::initConfig()
{
	QString currDate = QDate::currentDate().toString("yyyy-MM-dd");
	QString Date = "Date";
	bool DateExist = isCheckGroupExist(Date);
	if (!DateExist)
	{
		// 如果不存在，创建日期组
		settings->beginGroup(Date);
		settings->setValue(QString("DateStart"), currDate);
		settings->setValue(QString("DateEnd"),  QDate::currentDate().addMonths(1).toString("yyyy-MM-dd"));
		settings->setValue(QString("CoinNums"), 0);
		settings->setValue(QString("GameNums"), 0);
		settings->endGroup();
	}

	bool totalConfigExist = isCheckGroupExist(QString("Total"));
	if (!totalConfigExist)
	{
		// 如果不存在，创建总计组
		settings->beginGroup(QString::fromStdString("Total"));
		settings->setValue(QString("TotalCoinNums"), 0);
		settings->setValue(QString("TotalGameNums"), 0);
		settings->endGroup();
	}

	// 获取总投币数
	settings->beginGroup(QString::fromLocal8Bit("Total"));
	int totalCoin = settings->value(QString::fromLocal8Bit("TotalCoinNums"), 0).toInt();
	settings->endGroup();

	bool configExist = isCheckGroupExist(currDate);
	if (!configExist)
	{
		// 当天组不存在，创建当天组
		settings->beginGroup(currDate);
		settings->setValue(QString("DailyCoinNums"), 0);
		settings->setValue(QString("DailyGameNums"), 0);
		settings->setValue(QString("DailyTotalNums"), totalCoin);
		settings->endGroup();
	}
}

void zhangdanWidget::initUI()
{
	QVector<int> totalConfig = readConfig(QString::fromLocal8Bit("Total"));
	QVector<int> dailyConfig = readConfig(QDate::currentDate().toString("yyyy-MM-dd"));
	QVector<int> dateConfig = readConfig("Date");
	QVector<QString> dateConfigStr = readConfigStr("Date");

	// 创建布局管理器
	QGridLayout* gridLayout = new QGridLayout;
	QHBoxLayout* hBoxLayout = new QHBoxLayout;
	QVBoxLayout* vBoxLayout = new QVBoxLayout(this);

	// 设置布局的间距和对齐方式
	gridLayout->setSpacing(10);																					// 设置控件之间的间距
	gridLayout->setContentsMargins(20, 20, 20, 20);																// 设置布局的外边距

	// 创建字体对象并设置为加粗
	QFont boldFont;
	boldFont.setPointSizeF(12);
	boldFont.setBold(true);

	// 创建标签
	// 日期开始
	labelDate = new QLabel(this);
	labelDate->setText(QString::fromLocal8Bit("日期起始"));
	labelDate->setAlignment(Qt::AlignCenter);
	labelDate->setFont(boldFont);
	
	labelDateStr = new QLabel(this);
	labelDateStr->setText(dateConfigStr[0]);
	labelDateStr->setAlignment(Qt::AlignCenter);
	labelDateStr->setFont(boldFont);

	// 日期结束
	labelDateEnd = new QLabel(this);
	labelDateEnd->setText(QString::fromLocal8Bit("日期结束"));
	labelDateEnd->setAlignment(Qt::AlignCenter);
	labelDateEnd->setFont(boldFont);
	
	labelDateEndStr = new QLabel(this);
	labelDateEndStr->setText(dateConfigStr[1]);
	labelDateEndStr->setAlignment(Qt::AlignCenter);
	labelDateEndStr->setFont(boldFont);

	// 从日期开始时的累计投币
	labelCoins = new QLabel(this);
	labelCoins->setText(QString::fromLocal8Bit("累计投币"));
	labelCoins->setAlignment(Qt::AlignCenter);
	labelCoins->setFont(boldFont);

	labelCoinNums = new QLabel(this);
	labelCoinNums->setText(QString::fromLocal8Bit("%1").arg(dateConfig[0]));
	labelCoinNums->setAlignment(Qt::AlignCenter);
	labelCoinNums->setFont(boldFont);

	// 从日期开始的累计局数
	labelGames = new QLabel(this);
	labelGames->setText(QString::fromLocal8Bit("累计局数"));
	labelGames->setAlignment(Qt::AlignCenter);
	labelGames->setFont(boldFont);

	labelGameNums = new QLabel(this);
	labelGameNums->setText(QString::fromLocal8Bit("%1").arg(dateConfig[1]));
	labelGameNums->setAlignment(Qt::AlignCenter);
	labelGameNums->setFont(boldFont);

	// 总投币数
	QLabel* label1 = new QLabel(this);
	label1->setText(QString::fromLocal8Bit("总投币数"));
	label1->setAlignment(Qt::AlignCenter);
	label1->setFont(boldFont);
	QLabel* labelNum1 = new QLabel(this);
	labelNum1->setText(QString::fromLocal8Bit("%1").arg(totalConfig[0]));
	labelNum1->setAlignment(Qt::AlignCenter);
	labelNum1->setFont(boldFont);

	// 总比赛局数
	QLabel* label2 = new QLabel(this);
	label2->setText(QString::fromLocal8Bit("总比赛局数"));
	label2->setAlignment(Qt::AlignCenter);
	label2->setFont(boldFont);
	QLabel* labelNum2 = new QLabel(this);
	labelNum2->setText(QString::fromLocal8Bit("%1").arg(totalConfig[1]));
	labelNum2->setAlignment(Qt::AlignCenter);
	labelNum2->setFont(boldFont);

	// 当天投币数
	QLabel* label3 = new QLabel(this);
	label3->setText(QString::fromLocal8Bit("当天投币数"));
	label3->setAlignment(Qt::AlignCenter);
	label3->setFont(boldFont);
	QLabel* labelNum3 = new QLabel(this);
	labelNum3->setText(QString::fromLocal8Bit("%1").arg(dailyConfig[0]));
	labelNum3->setAlignment(Qt::AlignCenter);
	labelNum3->setFont(boldFont);

	// 当天比赛局数
	QLabel* label4 = new QLabel(this);
	label4->setText(QString::fromLocal8Bit("当天比赛局数"));
	label4->setAlignment(Qt::AlignCenter); 
	label4->setFont(boldFont);
	QLabel* labelNum4 = new QLabel(this);
	labelNum4->setText(QString::fromLocal8Bit("%1").arg(dailyConfig[1]));
	labelNum4->setAlignment(Qt::AlignCenter);
	labelNum4->setFont(boldFont);

	// 归零按钮，按下按钮，日期归零
	QPushButton* resetBtn = new QPushButton(this);
	resetBtn->setText(QString::fromLocal8Bit("归零"));
	resetBtn->setFixedSize(100, 30);
	connect(resetBtn, &QPushButton::clicked, this, &zhangdanWidget::resetConfig);

	// 将标签添加到布局中
	gridLayout->addWidget(labelDate, 0, 1);																		// 第一行，第一列
	gridLayout->addWidget(labelDateStr, 0, 2);																	// 第一行，第二列
	gridLayout->addWidget(labelDateEnd, 1, 1);																	// 第二行，第一列
	gridLayout->addWidget(labelDateEndStr, 1, 2);																// 第二行，第二列
	gridLayout->addWidget(labelCoins, 2, 1);																	// 第三行，第一列
	gridLayout->addWidget(labelCoinNums, 2, 2);																	// 第三行，第二列
	gridLayout->addWidget(labelGames, 3, 1);																	// 第四行，第一列
	gridLayout->addWidget(labelGameNums, 3, 2);																	// 第四行，第二列
	gridLayout->addWidget(label1, 4, 1);																		// 第五行，第一列
	gridLayout->addWidget(labelNum1, 4, 2);																		// 第五行，第二列
	gridLayout->addWidget(label2, 5, 1);																		// 第六行，第一列
	gridLayout->addWidget(labelNum2, 5, 2);																		// 第六行，第二列
	gridLayout->addWidget(label3, 6, 1);																		// 第七行，第一列
	gridLayout->addWidget(labelNum3, 6, 2);																		// 第七行，第二列
	gridLayout->addWidget(label4, 7, 1);																		// 第八行，第一列
	gridLayout->addWidget(labelNum4, 7, 2);																		// 第八行，第二列

	hBoxLayout->addWidget(resetBtn);
	hBoxLayout->setAlignment(Qt::AlignHCenter);
	
	vBoxLayout->addLayout(gridLayout);																			
	vBoxLayout->addSpacing(20);
	vBoxLayout->addLayout(hBoxLayout);

	// 设置布局
	this->setLayout(vBoxLayout);
	// 设置窗口的最小大小（可选）
	this->setMinimumSize(1000, 800);																			// 设置窗口的最小宽度和高度
}

// 检查当前组是否存在
bool zhangdanWidget::isCheckGroupExist(QString group)
{
	settings->beginGroup(group);
	QStringList keys = settings->allKeys();
	settings->endGroup();
	return keys.size() > 0;
}

void zhangdanWidget::BindSlots()
{
	connect(signalSend.get(), &SignalSend::toubiEnd, this, &zhangdanWidget::writeConfig);
}

// 更新日期UI
void zhangdanWidget::updateDateUI()
{
	settings->beginGroup(QString("Date"));
	labelDateStr->setText(settings->value(QString("DateStart"), QDate::currentDate().toString("yyyy-MM-dd")).toString());
	labelDateEndStr->setText(settings->value(QString("DateEnd"), QDate::currentDate().addMonths(1).toString("yyyy-MM-dd")).toString());
	labelCoinNums->setText(QString::fromLocal8Bit("%1").arg(settings->value(QString("CoinNums"), 0).toInt()));
	labelGameNums->setText(QString::fromLocal8Bit("%1").arg(settings->value(QString("GameNums"), 0).toInt()));
	settings->endGroup();
}

// 读取配置文件
QVector<int> zhangdanWidget::readConfig(QString group)
{
	if (group == QString("Total"))
	{
		settings->beginGroup(group);
		QVector<int> result;
		result.push_back(settings->value(QString("TotalCoinNums"), 0).toInt());
		result.push_back(settings->value(QString("TotalGameNums"), 0).toInt());
		settings->endGroup();
		return result;
	}
	else if (group == QDate::currentDate().toString("yyyy-MM-dd"))
	{
		settings->beginGroup(group);
		QVector<int> result;
		result.push_back(settings->value(QString("DailyCoinNums"), 0).toInt());
		result.push_back(settings->value(QString("DailyGameNums"), 0).toInt());
		result.push_back(settings->value(QString("DailyTotalNums"), 0).toInt());
		settings->endGroup();
		return result;
	}
	else if (group == QString("Date"))
	{
		settings->beginGroup(group);
		QVector<int> result;
		result.push_back(settings->value(QString("CoinNums"), 0).toInt());
		result.push_back(settings->value(QString("GameNums"), 0).toInt());
		settings->endGroup();
		return result;
	}
	return QVector<int>();
}

QVector<QString> zhangdanWidget::readConfigStr(QString group)
{
	if (group == QString("Date"))
	{
		settings->beginGroup(group);
		QVector<QString> result;
		result.push_back(settings->value(QString("DateStart"), QDate::currentDate().toString("yyyy-MM-dd")).toString());
		result.push_back(settings->value(QString("DateEnd"), QDate::currentDate().addMonths(1).toString("yyyy-MM-dd")).toString());
		settings->endGroup();
		return result;
	}
	return QVector<QString>();
}

void zhangdanWidget::writeConfig(bool isSingleMode)
{
	int _GameNums = isSingleMode ? 1 : 2;													// 单人模式记录1局，双人模式记录2局

	QString dailyBillDate = QDate::currentDate().toString("yyyy-MM-dd");

	// 从指定日期开始记录的投币数
	settings->beginGroup(QString("Date"));
	int coinNums = settings->value(QString("CoinNums"), 0).toInt();
	int gameNums = settings->value(QString("GameNums"), 0).toInt();
	settings->endGroup();

	coinNums += SignalRelay::GetInstance()->settingCoin;
	gameNums += _GameNums;

	// 当天的投币数，比赛局数，累计总投币数
	settings->beginGroup(dailyBillDate);
	int dailyCoin = settings->value(QString::fromLocal8Bit("DailyCoinNums"), 0).toInt();
	int dailyGameNums = settings->value(QString::fromLocal8Bit("DailyGameNums"), 0).toInt();
	int dailyTotalNums = settings->value(QString::fromLocal8Bit("DailyTotalNums"), 0).toInt();
	settings->endGroup();

	dailyCoin += SignalRelay::GetInstance()->settingCoin;
	dailyGameNums += _GameNums;
	dailyTotalNums += SignalRelay::GetInstance()->settingCoin;

	// 总记的投币数，总比赛局数
	settings->beginGroup(QString::fromLocal8Bit("Total"));
	int totalCoin = settings->value(QString::fromLocal8Bit("TotalCoinNums"), 0).toInt();
	int totalGameNums = settings->value(QString::fromLocal8Bit("TotalGameNums"), 0).toInt();
	settings->endGroup();

	totalCoin += SignalRelay::GetInstance()->settingCoin;
	totalGameNums += _GameNums;

	// 写入配置文件
	settings->beginGroup(QString("Date"));
	settings->setValue(QString("CoinNums"), coinNums);
	settings->setValue(QString("GameNums"), gameNums);
	settings->endGroup();
	updateDateUI();

	settings->beginGroup(dailyBillDate);
	settings->setValue(QString::fromLocal8Bit("DailyCoinNums"), dailyCoin);
	settings->setValue(QString::fromLocal8Bit("DailyGameNums"), dailyGameNums);
	settings->setValue(QString::fromLocal8Bit("DailyTotalNums"), dailyTotalNums);
	settings->endGroup();

	settings->beginGroup(QString::fromLocal8Bit("Total"));
	settings->setValue(QString::fromLocal8Bit("TotalCoinNums"), totalCoin);
	settings->setValue(QString::fromLocal8Bit("TotalGameNums"), totalGameNums);
	settings->endGroup();
}
