#include "toubiWidget.h"
#include <QFont>
#include <QDebug>
#include "SignalRelay.h"

int toubiWidget::coinCount = 1;																				// 默认一个硬币开始一句游戏
int toubiWidget::juCount = 1;																				// 默认一局游戏需要1个硬币
int toubiWidget::xuwanCount = 0;																			// 默认可以续玩1局游戏
bool toubiWidget::isGameMode = false;																		// 默认游戏没有结束

toubiWidget::toubiWidget(QWidget *parent)
	: QWidget(parent)
{
	init();
}

toubiWidget::~toubiWidget()
{
	Destroy();
}

// 初始话窗口
void toubiWidget::init()
{
	ui = new Ui::toubiWidgetClass;
	settings = QSharedPointer<QSettings>(new QSettings("SNK", "toubiWidget"));
	ui->setupUi(this);

	// 读取设置，如果没有找到，则使用默认值
	ui->toubiBox->setValue(settings->value("coinCount", 1).toInt());											// 设置硬币数量
	ui->juBox->setValue(settings->value("juCount", 1).toInt());													// 设置一局游戏需要的投币数量
	ui->xuwanBox->setValue(settings->value("xuwanCount", 0).toInt());											// 设置可以续玩的局数
	ui->comboBox->setCurrentIndex(settings->value("isGameMode", 0).toInt());									// 设置游戏模式
	
	coinCount = ui->toubiBox->value();																			// 获取硬币数量
	SignalRelay::GetInstance()->settingCoin = coinCount;														// 在全局变量类中记录一局游戏所需要的硬币数量
	juCount = ui->juBox->value();																				// 获取一局游戏需要的投币数量
	xuwanCount = ui->xuwanBox->value();																			// 获取可以续玩的局数
	SignalRelay::GetInstance()->xuwanCount = xuwanCount;														// 在全局变量类中记录胜利一局可以续玩多少局游戏
	isGameMode = ui->comboBox->currentIndex() == 0;																// 获取游戏模式

	// 保存设置
	settings->sync();

	BindSlot(); // 绑定槽函数
}

// 绑定槽函数
void toubiWidget::BindSlot()
{
	// 当投币数发生改变时，更新一局游戏需要的投币数量
	connect(ui->toubiBox, SIGNAL(valueChanged(int)), this, SLOT(setCoinCount(int)));
	connect(ui->juBox, SIGNAL(valueChanged(int)), this, SLOT(setJuCount(int)));
	connect(ui->xuwanBox, SIGNAL(valueChanged(int)), this, SLOT(setXuwanCount(int)));
	// 当游戏模式发生改变时，更新游戏模式
	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setIsGameMode(int)));
}

// 设置硬币数量
void toubiWidget::setCoinCount(int count)
{
	coinCount = count;
	SignalRelay::GetInstance()->settingCoin = coinCount;
	settings->setValue("coinCount", coinCount);
	settings->sync();
	qDebug() << "toubiWidget::setCoinCount : coninCount = " << coinCount;
}

// 设置coinCount个硬币可以玩多少局游戏
void toubiWidget::setJuCount(int count)
{
	juCount = count;
	settings->setValue("juCount", juCount);
	settings->sync();
	qDebug() << "toubiWidget::setJuCount : juCount = " << juCount;
}

// 设置可以续玩多少局游戏
void toubiWidget::setXuwanCount(int count)
{
	xuwanCount = count;
	SignalRelay::GetInstance()->xuwanCount = xuwanCount;
	settings->setValue("xuwanCount", xuwanCount);
	settings->sync();
	qDebug() << "toubiWidget::setXuwanCount : xuwanCount = " << xuwanCount;
}

// 设置游戏模式
void toubiWidget::setIsGameMode(int modeIndex)
{
	isGameMode = modeIndex == 0;
	int index = 0;
	if (isGameMode)
	{
		index = 0;
	}
	else
	{
		index = 1;
	}

	settings->setValue("isGameMode", index);
	settings->sync();
	qDebug() << "toubiWidget::setIsGameMode : isGameMode = " << isGameMode;
}

// 销毁指针变量
void toubiWidget::Destroy()
{
	if (ui)
	{
		delete ui;
		ui = nullptr;
	}
}
