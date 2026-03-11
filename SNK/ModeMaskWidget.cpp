#include "ModeMaskWidget.h"
#include<QTimer>
#include <QThread>
#include "CaptureStatic.h"
#include "MaskWidget.h"
#include "SerialPort.h"

#define ANIM_PATH ":/ModeMaskWidget/ModeMaskWidget/"
#define ANIM_COUNTS 8
#define ANIM_FRAME_TIME 50

ModeMaskWidget::ModeMaskWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// 初始化变量
	if (m_SignalSend == nullptr)
	{
		m_SignalSend = SignalSend::GetInstance();
	}
	if (m_SignalRelay == nullptr)
	{
		m_SignalRelay = SignalRelay::GetInstance();
	}

	// 初始化两个按钮
	//m_1PButton = new QPushButton(ui.widget);
	//m_2PButton = new QPushButton(ui.widget);
	
	/* 加载动画序列帧 */
	qDebug() << QString::fromLocal8Bit("加载动画帧资源");
	const QString Path = ANIM_PATH;
	int PixmapCount = CaptureStatic::LoadAnimPixmaps(m_ModePixmaps, Path, ANIM_COUNTS);

	m_AnimTimer = new QTimer(this);

	setGeometry(QGuiApplication::primaryScreen()->geometry());

	BindSlot();
	hide();
	update();
}

ModeMaskWidget::~ModeMaskWidget()
{
	if (m_AnimTimer)
	{
		delete m_AnimTimer;
		m_AnimTimer = nullptr;
	}
}

// 绑定槽函数
void ModeMaskWidget::BindSlot()
{
	/* 单人双人模式信号对应的槽函数(New) */
	connect(m_SignalSend.get(), &SignalSend::OnePKeyPress_Single, this, &ModeMaskWidget::On1P2PButtonClicked);									// 1P单人模式			
	connect(m_SignalSend.get(), &SignalSend::TwoPKeyPress_Single, this, &ModeMaskWidget::On1P2PButtonClicked);									// 2P单人模式
	connect(m_SignalSend.get(), &SignalSend::DoubleModeKeyPress, this, &ModeMaskWidget::On1P2PButtonClicked);									// 1P2P双人模式

	connect(this, &ModeMaskWidget::OpenSwitchMode, m_SignalSend.get(), &SignalSend::OpenSwitchMode);											// 开启全局截屏
	connect(m_AnimTimer, &QTimer::timeout, this, &ModeMaskWidget::CalcuCurrFrameIndex);															// 动画播放
}

// 显示遮罩层，并置顶
void ModeMaskWidget::showMask()
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	
	m_AnimTimer->start(ANIM_FRAME_TIME);

	setGeometry(QGuiApplication::primaryScreen()->geometry());
	showFullScreen();
	/* 只有遮罩层显示时才允许监听1P2P信号 */
	m_SignalRelay->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask = true;								// 可以接受1p/2p信号
}

// 隐藏遮罩层
void ModeMaskWidget::hideMask()
{
	hide();
}

void ModeMaskWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), m_ModePixmaps[m_CurrentFrame]);
}

// 计算当前帧的索引
void ModeMaskWidget::CalcuCurrFrameIndex()
{
	m_CurrentFrame = (m_CurrentFrame + 1) % m_ModePixmaps.size();
	update();
}

// 槽函数 **********************************************************************************************//

/* 当1P2P按键按下时 */
void ModeMaskWidget::On1P2PButtonClicked()
{
	// 如果1P按键或2P按键没有按下并且允许接受1P2P信号，否则不再响应
	if (!m_SignalRelay->m_gameFlowInfo->isPress1P2P)
	{
		m_AnimTimer->stop();

		m_SignalRelay->m_gameFlowInfo->isPress1P2P = true;											// 1P或者2P已按下
		emit m_SignalSend->write_single_register(SerialPort::m_charge, SerialPort::ChargeOpen);		// 打开充电灯	

		if (m_SignalRelay->m_gameFlowInfo->OneP_Single == 1)
		{
			qDebug() << QString::fromLocal8Bit("玩家对电脑") << m_SignalRelay->coin << m_SignalRelay->settingCoin;

			m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PlayerToPCMode;
			MaskWidget::GetInstance()->StartListen();												// 监听投币

			hideMask();																				// 遮罩层不显示时不允许监听1P2P信号
		}
		else if (m_SignalRelay->m_gameFlowInfo->TwoP_Single == 1)
		{
			qDebug() << QString::fromLocal8Bit("电脑对玩家") << m_SignalRelay->coin << m_SignalRelay->settingCoin;

			m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PCToPlayerMode;
			MaskWidget::GetInstance()->StartListen();												// 监听投币

			hideMask();																				// 遮罩层不显示时不允许监听1P2P信号
		}
		else if (m_SignalRelay->m_gameFlowInfo->DoubleMode == 1)
		{
			qDebug() << QString::fromLocal8Bit("双人模式") << m_SignalRelay->coin << m_SignalRelay->settingCoin;

			hideMask();																				// 遮罩层不显示时不允许监听1P2P信号
			/* 检测当前玩家的币是否满足双人模式的条件 */
			if (m_SignalRelay->coin < 2 * m_SignalRelay->settingCoin)
			{
				QTimer::singleShot(1000, [this]()
					{
						MaskWidget::GetInstance()->showMask(false, true, true);
					});
			}
			else
			{
				MaskWidget::GetInstance()->StartListen(false, true);								// 监听投币
			}
			m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PlayerToPlayerMode;
		}
	}
}