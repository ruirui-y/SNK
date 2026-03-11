#include "MaskWidget.h"
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

#include "SignalRelay.h"
#include "SignalSend.h"
#include "ShowLabel.h"
#include "QStyle"

#define TIME_INTERVAL 250

MaskWidget::MaskWidget(QWidget *parent)
	: QWidget(parent)
{
	init();
}

MaskWidget::~MaskWidget()
{
	if (m_CoinLabel)
	{
		delete m_CoinLabel;
		m_CoinLabel = nullptr;
	}

	if (m_UpdataLabelTextTimer)
	{
		delete m_UpdataLabelTextTimer;
		m_UpdataLabelTextTimer = nullptr;
	}
}

void MaskWidget::init()
{	
	/* 初始化全局变量类和全局信号类 */
	m_SignalRelay = SignalRelay::GetInstance();
	m_SignalSend = SignalSend::GetInstance();

	// 加载设置
	InitTotalCoin();

	/* 初始化用于刷新标签文本的定时器 */
	m_UpdataLabelTextTimer = new QTimer(this);

	InitItemLayout();
	BindSlots();
}

void MaskWidget::InitTotalCoin()
{
	totalCoin_single = m_SignalRelay->settingCoin;
	totalCoin_Double = m_SignalRelay->settingCoin * 2;
}

void MaskWidget::InitItemLayout()
{
	/* 加载背景 */
	ui.setupUi(this);
	m_CurrMask = QPixmap(":/mask1.png");
	m_CoinLabel = new ShowLabel(this);
	m_CoinLabel->setObjectName("TextLabel");


	ui.leftLabel->move(rect().left(), rect().top());

	// 获取显示器分辨率
	QScreen* screen = QGuiApplication::primaryScreen();
	int screenWidth = screen->geometry().width();
	int screenHeight = screen->geometry().height();
	qDebug() << "screenWidth:" << screenWidth << "screenHeight:" << screenHeight;

	// 获取中心位置
	int centerX = (screenWidth / 2 - ui.leftLabel->width() / 2);
	int centerY = (screenHeight / 2 - ui.leftLabel->height() / 2);

	qDebug() << "centerX:" << centerX << "centerY:" << centerY;

	// 设置中心位置
	int adjustX = 60;
	int adjustY = screenHeight / 7 * 2;
	ui.leftLabel->move(centerX - adjustX, centerY + adjustY + 40);

	// 设置右边标签
	ui.rightLabel->setText(QString::number(0) + " / " + QString::number(m_SignalRelay->settingCoin));
	ui.rightLabel->move(centerX + ui.leftLabel->width() + 20, centerY + adjustY + 40);

	// 设置还剩余多少币的展示标签
	m_CoinLabel->move(centerX - 3 * adjustX, centerY + adjustY + 300);

	setGeometry(QGuiApplication::primaryScreen()->geometry());

	update();
}

void MaskWidget::BindSlots()
{
	connect(m_UpdataLabelTextTimer, &QTimer::timeout, this, &MaskWidget::UpdataLabelText);
	connect(m_SignalSend.get(), &SignalSend::OnePKeyPress_Single_CoinMask, this, &MaskWidget::On1P2PKeyPress);
	connect(m_SignalSend.get(), &SignalSend::TwoPKeyPress_Single_CoinMask, this, &MaskWidget::On1P2PKeyPress);
}

void MaskWidget::On1P2PKeyPress()
{
	if (m_SignalRelay->m_gameFlowInfo->OneP_Single == 1)
	{
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PlayerToPCMode;
	}
	else if (m_SignalRelay->m_gameFlowInfo->TwoP_Single == 1)
	{
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PCToPlayerMode;
	}
	StartListen();
}

void MaskWidget::UpdataLabelText()
{
	InitTotalCoin();
	int totalCoin = m_isSingle ? totalCoin_single : totalCoin_Double;
	qDebug() << "totalCoin:" << totalCoin;
	m_CoinLabel->UpdateStyleSheet(QString::fromLocal8Bit("双人游戏需要再投 ") + QString::number(totalCoin - m_SignalRelay->coin) + " " + QString::fromLocal8Bit("个币"));
	qDebug() << QString::fromLocal8Bit("双人游戏需要再投 ") << totalCoin - m_SignalRelay->coin << QString::fromLocal8Bit("个币");

	setLabelText();

	if (m_SignalRelay->coin >= totalCoin && _bIsCoinReduce)
	{
		/* 不管是单人还是双人模式，都需要停止投币轮询，并开启全局截屏 */
		m_SignalRelay->coin -= totalCoin;														// 减去总投币数
		emit m_SignalSend->StopCoinPolling();													// 停止投币轮询
		emit m_SignalSend->toubiEnd(m_isSingle);												// 投币轮询结束，双人模式
		emit m_SignalSend->OpenSwitchMode();													// 开启全局截屏
		hideMask();
	}
}

// 显示遮罩层
bool MaskWidget::showMask(bool isSingle, bool bIsCoinReduce, bool isAllowListen)
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	/* 单人或者双人模式 isSingle 用来切换UI */
	m_isSingle = isSingle;

	// 单人模式的UI
	if (m_isSingle)
	{
		m_CoinLabel->hide();
	}

	// 双人模式的UI
	if (!m_isSingle)
	{
		m_CoinLabel->UpdateStyleSheet(QString::fromLocal8Bit("双人游戏需要再投 ") + QString::number(m_SignalRelay->coin) + " " + QString::fromLocal8Bit("个币"));
		m_CoinLabel->show();
	}

	/* 是否允许扣币*/
	_bIsCoinReduce = bIsCoinReduce;
	m_UpdataLabelTextTimer->start(TIME_INTERVAL);

	setGeometry(QGuiApplication::primaryScreen()->geometry());

	/* 是否允许监听1P2P按键信号 */
	m_SignalRelay->m_gameFlowInfo->isAllowRecv1P2PSignal_CoinMask = isAllowListen;

	update();
	showFullScreen();
	return true;
}

// 隐藏遮罩层
bool MaskWidget::hideMask()
{
	m_UpdataLabelTextTimer->stop();
	hide();
	return false;
}

void MaskWidget::StartListen(bool isSingle, bool bIsCoinReduce)
{
	m_isSingle = isSingle;
	_bIsCoinReduce = bIsCoinReduce;
	m_UpdataLabelTextTimer->start(TIME_INTERVAL);
}

// 设置右边标签显示的数字
void MaskWidget::setLabelText()
{
	int totalCoin = m_isSingle ? totalCoin_single : totalCoin_Double;

	ui.rightLabel->setText(QString::number(m_SignalRelay->coin) + " / " + QString::number(totalCoin));
	update();
}

// 绘制投币按钮
void MaskWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_CurrMask);
	ui.leftLabel->show();
	ui.rightLabel->show();
}