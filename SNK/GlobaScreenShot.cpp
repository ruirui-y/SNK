#include "GlobaScreenShot.h"
#include <QTimer>
#include <QDir>
#include "SerialPort.h"

#define thresold 0.95

GlobaScreenShot::GlobaScreenShot(QObject *parent)
	: Interface(parent),m_IsWhiteScreen(false)
{
	init();
}

GlobaScreenShot::~GlobaScreenShot()
{
	if (m_CountDownTimer)
	{
		delete m_CountDownTimer;
		m_CountDownTimer = nullptr;
	}
}

// 主界面截图
void GlobaScreenShot::mainScreenShot()
{
	if (m_SwitchMode)
	{
		cv::Mat image = CaptureStatic::CaptureFullScreen();
		// 整张截图的对比
		string key = CaptureStatic::CharacterMatch(image, m_ImageMap, thresold);
		sendSignal(key);

		// 截取游戏区域的截图的对比
		DisGameScreenShot(image, m_AvatarEnterRect, m_AvatarEnterMap);
		DisGameScreenShot(image, m_GameEndRect, m_GameEndMap);
		DisGameScreenShot(image, m_ShieldeRect, m_ShieldXMap);
	}
}

// 加载所需要的图片
void GlobaScreenShot::init()
{
	CaptureStatic::NoFileNameLoadImages("gameFlow", m_ImageMap);
	CaptureStatic::NoFileNameLoadImages("AvatarEnter", m_AvatarEnterMap);
	CaptureStatic::NoFileNameLoadImages("GameEnd", m_GameEndMap);
	CaptureStatic::NoFileNameLoadImages("ShieldX", m_ShieldXMap);

	// 初始化变量
	m_AvatarEnterRect = cv::Rect(374, 339, 52, 43);
	m_GameEndRect = cv::Rect(139, 113, 71, 30);
	m_ShieldeRect = cv::Rect(355, 5, 88, 100);

	m_SignalSend = SignalSend::GetInstance();	
	m_SignalRelay = SignalRelay::GetInstance();

	m_SwitchMode = true;

	m_PreCaptureImg = CaptureStatic::CaptureFullScreen();												// 初始化上一张截图的变量
	m_CountDownRect = cv::Rect(383, 22, 35, 31);														// 倒计时区域		

	m_CountDownTimer = new QTimer(this);																// 倒计时计时器

	m_handleTimer = new QTimer(this);																	// 实时发送手柄开启信号(打开手柄并屏蔽按键B)
	connect(m_handleTimer, &QTimer::timeout, this, [this]()
		{
			emit m_SignalSend->write_joystick_1b2b(SerialPort::OpenYaoGan, SerialPort::OpenYaoGan,
				SerialPort::CloseB, SerialPort::OpenB);
		});

	// 绑定信号槽
	BindSlot();
}

// 截取游戏区域的截图
void GlobaScreenShot::DisGameScreenShot(cv::Mat img, cv::Rect rect,map<string, cv::Mat>& imageMap)
{
	cv::Mat image = img(rect);
	
	// 比对图片
	string key = CaptureStatic::CharacterMatch(image,imageMap, thresold);
	sendSignal(key);
}

// 发送信号
void GlobaScreenShot::sendSignal(string str)
{
	if (str == "gameBegin")
	{
		// 串口线程来发送手柄的A按键
		emit gameBegin();
		qDebug() << "emit gameBegin";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "mainMenu")
	{
		emit m_SignalSend->write_hp(0, 0);																					// 主菜单界面清空血条数值
		// 串口线程来发送手柄的A按键
		emit mainMenu();
		qDebug() << "emit mainMenu";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "SelectedModeOne")
	{
		emit selectedModeOne();
		qDebug() << "emit slectedModeOne";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "SelectedModeTwo")
	{
		emit selectedModeTwo();
		qDebug() << "emit slectedModeTwo";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "SelectedModeTwoPlayerToPC")
	{
		emit m_SignalSend->selectedModeTwo_PlayerToPC();
		qDebug() << "emit SelectedModeTwoPlayerToPC";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "SelectedModeTwoPCToPlayer")
	{
		emit m_SignalSend->selectedModeTwo_PCToPlayer();
		qDebug() << "emit SelectedModeTwoPCToPlayer";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "EnterButton")
	{
		emit EnterButton();
		qDebug() << "emit EnterButton";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "EnterButton2")
	{
		emit m_SignalSend->EnterButton2();
		qDebug() << "emit EnterButton2";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "EnterButton3")
	{
		emit m_SignalSend->EnterButton2();																					// 发送2P信号
		qDebug() << "emit EnterButton3";
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "shieldX")
	{
		emit ShieldXButton();																								// 屏蔽按键X
		m_handleTimer->stop();																								// 停止手柄信号的发送
		qDebug() << "emit shieldX";
	}
	else if (str == "wuchubujiu")
	{
		emit wuchubujiu();
		qDebug() << "emit wuchubujiu";
	}
	else if (str == "AvatarEnter")																							// 进入游戏
	{
		emit m_SignalSend->write_hp(0, 0);																					// 进入游戏时打开血条
		emit AvatarEnter();
		qDebug() << "emit AvatarEnter";
		m_SwitchMode = false;																								// 关闭全局截屏
		QThread::msleep(10000);
		m_SwitchMode = true;																								// 开启全局截屏
	}
	else if (str == "white")
	{
		// 如果处于战斗场景中
		if (!m_SignalRelay->m_gameFlowInfo->isAvatarEnter)
		{
			m_SignalRelay->m_gameFlowInfo->isStartBloodCapture = false;
			emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);							// 关闭手柄
			emit m_SignalSend->WhiteScreen();																				// 发送白屏信号
			qDebug() << "emit FirstGameEnd";
		}
	}
	else if (str == "GameEndSuccess")
	{
		// 通知另外两个线程关闭截图
		m_SignalRelay->m_gameFlowInfo->isStartBloodCapture = false;
		m_SignalRelay->m_gameFlowInfo->globalLeftWin = true;
		qDebug() << "emit GameEndSuccess";
		qDebug() << QString::fromLocal8Bit("左边赢了，游戏结束了，开始下一局");
		emit gameEndSuccess();
		m_SwitchMode = false;																								// 关闭全局截屏
	}
	else if (str == "GameEndFail")
	{
		// 通知另外两个线程关闭截图
		m_SignalRelay->m_gameFlowInfo->isStartBloodCapture = false;
		m_SignalRelay->m_gameFlowInfo->globalRightWin = true;																// 右边赢了
		qDebug() << "emit GameEndFail";
		qDebug() << QString::fromLocal8Bit("右边赢了，游戏结束了，开始下一局");
		emit gameEndFail();
		m_SwitchMode = false;																								// 关闭全局截屏
	}
}

// 绑定信号槽
void GlobaScreenShot::BindSlot()
{
	connect(this, &GlobaScreenShot::gameBegin, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->gameBegin();
		});
	connect(this, &GlobaScreenShot::mainMenu, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->mainMenu();
		});
	connect(this, &GlobaScreenShot::selectedModeOne, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->selectedModeOne();
		});
	connect(this, &GlobaScreenShot::selectedModeTwo, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->selectedModeTwo();
		});
	connect(this, &GlobaScreenShot::EnterButton, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->EnterButton();
		});
	connect(this, &GlobaScreenShot::ShieldXButton, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->ShieldXButton();
		});
	connect(this, &GlobaScreenShot::wuchubujiu, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->wuchubujiu();
		});
	connect(this, &GlobaScreenShot::AvatarEnter, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->ShieldXButton();
		});
	connect(this, &GlobaScreenShot::AvatarEnter, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->AvatarEnter();
		});
	connect(this, &GlobaScreenShot::FirstGameEnd, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->FirstGameEnd();
		});
	connect(this, &GlobaScreenShot::gameEndSuccess, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->gameEndSuccess();
		});
	connect(this, &GlobaScreenShot::gameEndFail, m_SignalSend.get(), [&]()
		{
			emit m_SignalSend->gameEndFailed();
		});

	// 绑定打开全局截屏
	connect(m_SignalSend.get(), &SignalSend::OpenSwitchMode, this, &GlobaScreenShot::OpenSwitchMode);
	connect(m_SignalSend.get(), &SignalSend::CloseSwitchMode, this, &GlobaScreenShot::CloseSwitchMode);

	/* 倒计时截图的定时器 */
	connect(m_CountDownTimer, &QTimer::timeout, this, &GlobaScreenShot::JudeFightIsEnd);
	/* 是否开始倒计时截图 */
	connect(m_SignalSend.get(), &SignalSend::StartCountDownCrop, this, [this]()
		{
			m_CountDownTimer->start(2000);
		},Qt::QueuedConnection);
	connect(m_SignalSend.get(), &SignalSend::StopCountDownCrop, this, [this]()
		{
			m_CountDownTimer->stop();
		},Qt::QueuedConnection);

	/* 实时截屏来打开手柄信号 */
	connect(m_SignalSend.get(), &SignalSend::StartSelectPlayerCheck, this, &GlobaScreenShot::StartSendHandleSignal);
}

// 启动游戏
void GlobaScreenShot::startGame()
{
	QString startBatPath = QDir::currentPath() + "/Bat/start.bat";
	// 使用 system 函数执行批处理文件
	system(startBatPath.toStdString().c_str());
}

/* 判断一局战斗是否结束*/
void GlobaScreenShot::JudeFightIsEnd()
{
	cv::Mat image = CaptureStatic::CaptureFullScreen();
	cv::Mat CurrcountDownImg = image(m_CountDownRect);																					// 当前倒计时图片
	cv::Mat PrecountDownImg = m_PreCaptureImg(m_CountDownRect);																			// 上一张倒计时图片

	bool isSame = CaptureStatic::CompareImage(CurrcountDownImg, PrecountDownImg, thresold);
	if (isSame)
	{
		qDebug() << QString::fromLocal8Bit("经倒计时检测发现当前战斗已经结束");
		emit m_SignalSend->FightEnd();
		m_CountDownTimer->stop();
	}
	m_PreCaptureImg = image;																											// 更新上一张图片
}



// 槽函数 ***************************************************************************************************//

// 定时处理截取的图片
void GlobaScreenShot::onTimeDisScreenShot()
{
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &GlobaScreenShot::mainScreenShot);
	timer->start(100);
	startGame();																														// 启动游戏
	qDebug() << QString::fromLocal8Bit("全局截屏线程ID: ") << QThread::currentThreadId();
}

// 打开全局截屏
void GlobaScreenShot::OpenSwitchMode()
{
	m_SwitchMode = true;
}

void GlobaScreenShot::CloseSwitchMode()
{
	m_SwitchMode = false;
}

void GlobaScreenShot::InitSelf()
{
	onTimeDisScreenShot();
}

void GlobaScreenShot::StartSendHandleSignal()
{
	m_handleTimer->start(1000);
}