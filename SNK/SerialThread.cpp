#include "SerialThread.h"
#include "SerialPort.h"
#include "MaskWidget.h"
#include "CaptureStatic.h"
#include "Enum.h"
#include "Global.h"
#include "TextRecognition.h"

#include <QThread>
#include <QDebug>

#define FLASH_TIMES						20 // 闪烁次数
#define LIGHT							255 // 灯条亮度

// 发送按键接受的数据的长度
#define RECV_DATA_LEN					11

#define SinleMenuButtonWidth			248
#define SinleMenuButtonHeight			22
#define SinleMenuButtonX				275

#define DoubleMenuButtonWidth			206
#define DoubleMenuButtonHeight			18
#define DoubleMenuButtonX				296

#define THREASOLD						0.80

#define COIN_POLL_TIME					500																							// 轮询投币时间

SerialThread::SerialThread(QObject *parent)
	: QObject(parent)
{
	hWnd = NULL;
	m_SignalSend = SignalSend::GetInstance();
	m_SignalRelay = SignalRelay::GetInstance();
	m_KeyPress = QSharedPointer<KeyPress>(new KeyPress);

	BindSlots();																									// 绑定槽函数
	onTimerSerial();

	CaptureStatic::NoFileNameLoadImages("gameFlow", m_ImageMap);													// 加载游戏流程图片
	initMenuButtonRect();																							// 初始化菜单按钮位置
	initMenuButtonMap();																							// 初始化按钮类型对应的按钮图片
	initMenuButtonRectMap();
	qDebug() << QString::fromLocal8Bit("SerialThread::SerialThread() 构造函数");
}

SerialThread::~SerialThread()
{
}

// 切换遮罩
void SerialThread::switchMask()
{
	// 每一帧都需要执行
	emit m_SignalSend->read_registers(SerialPort::MB_REG_SWITCH, 3);												// 读取投币数

	// 判断总投币数是否大于玩一局游戏所需要的投币数
	if (!m_SignalRelay->m_gameFlowInfo->isCoinFlipEnd)
	{
		/* 如果获取的投币数大于玩一局游戏所需要的投币数并且未扣除硬币时，避免当投币数足够多时第二次进入循环导致多次扣除 */
		if (m_SignalRelay->coin >= m_SignalRelay->settingCoin)
		{
			qDebug() << QString::fromLocal8Bit("投币数 >= 1时，统计到的投币数: ") << m_SignalRelay->coin;
			m_SignalRelay->m_gameFlowInfo->isCoinFlipEnd = true;													// 标记第一轮投币监听结束

			// 当投币数 == 1时不会走else语句，需要单独处理
			if (m_SignalRelay->settingCoin == 1)
			{
				emit m_SignalSend->stopStandbyAnimation();															// 避免当投币为1个时的情况
				emit m_SignalSend->CloseSwitchMode();
			}

			QTimer::singleShot(1200, [this]() {
				MaskWidget::GetInstance()->hideMask();																// 隐藏投币遮罩
				ModeMaskWidget::GetInstance()->showMask();															// 显示单人游戏双人游戏遮罩，可以接受1p/2p信号
				});
		}
		else
		{
			qDebug() << QString::fromLocal8Bit("统计到的投币数: ") << m_SignalRelay->coin;
			if (m_SignalRelay->coin > 0)
			{
				// 发射投币信号，通知视频停止播放
				emit m_SignalSend->stopStandbyAnimation();													
				// 显示遮罩
				MaskWidget::GetInstance()->showMask();
			}
		}
	}
}

/* 0个币的投币轮询 */
void SerialThread::onCoinZero()
{
	// 每一帧都需要执行
	emit m_SignalSend->read_registers(SerialPort::MB_REG_SWITCH, 3);												// 读取投币
}

// 线程启动函数
void SerialThread::onTimerSerial()
{
	// 在该线程启动的最开始去关闭所有的灯光，打开VS灯，然后关闭两个手柄
	emit m_SignalSend->write_win_led(SerialPort::WinClose, SerialPort::WinClose);									// 打开hp灯

	// 血量充满
	emit m_SignalSend->write_hp(100, 100);
	emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);							// 关闭两个手柄
	emit m_SignalSend->write_color_led(1, 3);																		// 氛围灯恢复正常
	emit m_SignalSend->write_single_register(SerialPort::m_vs, SerialPort::VsClose);								// 关闭vs灯

	emit m_SignalSend->write_single_register(SerialPort::m_charge, SerialPort::ChargeClose);						// 关闭充电灯
	emit m_SignalSend->write_single_register(SerialPort::m_shake1, SerialPort::ShakeClose);
	emit m_SignalSend->write_single_register(SerialPort::m_shake2, SerialPort::ShakeClose);

	CoinPollTimer = new QTimer(this);
	connect(CoinPollTimer, &QTimer::timeout, this, &SerialThread::switchMask);
	CoinPollTimer_Zero = new QTimer(this);
	connect(CoinPollTimer_Zero, &QTimer::timeout, this, &SerialThread::onCoinZero);
	qDebug() << QString::fromLocal8Bit("SerialThread::onTimerSerial() 初始化用于轮询监听投币和1p，2p计时器");
}

// 将拳皇程序显示到最前面
void SerialThread::setTopKOF()
{
	//发现句柄
	hWnd = FindWindowA("UnrealWindow", "KOFXV  ");
	if (hWnd != NULL)
	{
		// 置顶窗口
		SetForegroundWindow(hWnd);
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("SerialThread::setTopKOF() 未找到KOF窗口");
		return;
	}
}

// 游戏整体流程的处理 *********************************************************************************//
// 当游戏开始时，需要发送一个按键，然后播放过场动画
void SerialThread::DisGameBegin()
{
	emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);							// 关闭两个手柄
	emit m_SignalSend->write_single_register(SerialPort::MB_REG_KEY, SerialPort::SendAKey1P);
	//emit m_SignalSend->startMinScreenStandAnimation();															// 开始播放最小化屏幕过场动画
	// 要先将游戏置顶,才去发送信号，防止信号丢失
	qDebug() << QString::fromLocal8Bit("SerialThread::DisGameBegin()");

	QThread::msleep(2000);																							// 睡上2s钟，等界面稳定下来

	while (1)
	{
		setTopKOF();
		cv::Mat image = CaptureStatic::CaptureFullScreen();
		string key = CaptureStatic::CharacterMatch(image, m_ImageMap, THREASOLD);
		if (key == "gameBegin")
		{
			qDebug() << QString::fromLocal8Bit("当前界面是 gameBegin");
			emit m_SignalSend->write_single_register(SerialPort::MB_REG_KEY, SerialPort::SendAKey1P);
			QThread::msleep(1000);																					// 睡上1s钟，等串口发送成功
		}
		else
		{
			qDebug() << QString::fromLocal8Bit("按键发送成功！！！！！！");
			break;
		}
	}

	// 开始轮询监听投币信号和1p/2p遮罩信号
	if (m_SignalRelay->settingCoin != 0)
	{
		qDebug() << QString::fromLocal8Bit("收费模式，启动收费模式的投币轮询监听");
		emit m_SignalSend->StartPlayMoive();																		// 获取本地记忆币设置存储，发送按键成功信号，播放动画
		CoinPollTimer->start(COIN_POLL_TIME);																		// 500ms刷新一次
	}
	// 当记忆币为0时
	QTimer::singleShot(200, this, [=]()
		{
			if (m_SignalRelay->settingCoin == 0)
			{
				qDebug() << QString::fromLocal8Bit("免费模式, 启动免费模式的投币轮询监听");
				CoinPollTimer_Zero->start(COIN_POLL_TIME);															// 500ms刷新一次
				ModeMaskWidget::GetInstance()->showMask();															// 显示等待用户按下1p还是2p的遮罩,并允许监听1P2P按键信号
			}
		}); // 启动定时器

	/* 当选择游戏界面撤去时，会自动的开启全局截屏 */
}

// 当出现主菜单时，需要发送按键，进入模式选择1界面
void SerialThread::DisMainMenu()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisMainMenu()");
	// 通过串口发送任意键
	setTopKOF();																									// 置顶窗口
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	m_KeyPress->sendEnterKey();																						// 发送回车键
	QThread::msleep(200);																							// 睡上2s钟，等界面稳定下来
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
}

// 当出现模式选择1界面时，需要发送按键，进入模式选择2界面
void SerialThread::DisModeSelectOne()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisModeSelectOne()");
	// 通过串口发送任意键
	setTopKOF();																									// 置顶窗口
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	m_KeyPress->sendEnterKey();																						// 发送回车键
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
}

// 当出现模式选择2界面时，需要发送按键，进入游戏界面
void SerialThread::DisModeSelectTwo()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisModeSelectTwo()");
	qDebug() << "m_SignalRelay->m_gameFlowInfo->CurrGameMode = " << m_SignalRelay->m_gameFlowInfo->CurrGameMode;
	// 通过串口发送任意键
	// 需要判断是否是1p还是2p，然后发送不同的按键
	setTopKOF();																									// 置顶窗口
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	/* 玩家对电脑 或者 电脑对玩家 */
	if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode ::PlayerToPCMode|| m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PCToPlayerMode)
	{
		setTopKOF();
		m_KeyPress->sendDownKey();
		QThread::msleep(500);
	}
	/* 玩家对玩家 */
	else if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PlayerToPlayerMode)
	{
		setTopKOF();
		m_KeyPress->sendEnterKey();																					// 发送回车键
		QThread::msleep(500);
	}

	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
}

/* 选择模式界面2P 玩家对电脑 按键Enter 或者 按键Down */
void SerialThread::DisModeSelectTwo_PlayerToPC()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisModeSelectTwo_PlayerToPC()");
	qDebug() << "m_SignalRelay->m_gameFlowInfo->CurrGameMode = " << m_SignalRelay->m_gameFlowInfo->CurrGameMode;

	/* 如果当前选择的是玩家对电脑只需要在按下一次Enter */
	if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PlayerToPCMode)
	{
		setTopKOF();
		m_KeyPress->sendEnterKey();
		QThread::msleep(500);
	}
	/* 如果当前选择的是电脑对玩家需要在按下一次向下按键Down */
	if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PCToPlayerMode)
	{
		setTopKOF();
		m_KeyPress->sendDownKey();
		QThread::msleep(500);
	}
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
}

/* 选择模式界面2P 电脑对玩家 按键Enter */
void SerialThread::DisModeSelectTwo_PCToPlayer()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisModeSelectTwo_PCToPlayer()");
	qDebug() << "m_SignalRelay->m_gameFlowInfo->CurrGameMode = " << m_SignalRelay->m_gameFlowInfo->CurrGameMode;

	setTopKOF();
	m_KeyPress->sendEnterKey();
	QThread::msleep(500);
	
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
}

// 当出现游戏界面时，需要发送按键，进入游戏选角色界面
void SerialThread::DisEnterButton()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisEnterButton()");
	setTopKOF();																									// 置顶窗口
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	// 通过串口发送任意键
	// 发送1P按键
	emit m_SignalSend->write_single_register(SerialPort::MB_REG_KEY, SerialPort::SendStartKey1P);
	if (m_SignalRelay->m_gameFlowInfo->PlayerNums == 1)
	{
		// 打开手柄屏蔽按键B
		emit m_SignalSend->write_joystick_1b2b(SerialPort::OpenYaoGan, SerialPort::OpenYaoGan, SerialPort::CloseB, SerialPort::OpenB);
	}
	QThread::msleep(2000);																							// 睡上2s钟，等界面稳定下来
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
	emit m_SignalSend->StartSelectPlayerCheck();																	// 启动定时器，一直发送手柄打开信号
}

void SerialThread::DisEnterButton2()
{
	qDebug() << QString::fromLocal8Bit("SerialThread::DisEnterButton2()");
	setTopKOF();																									// 置顶窗口
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	// 通过串口发送任意键
	// 发送2P按键
	emit m_SignalSend->write_single_register(SerialPort::MB_REG_KEY, SerialPort::SendStartKey2P);
	// 打开手柄屏蔽按键B
	emit m_SignalSend->write_joystick_1b2b(SerialPort::OpenYaoGan, SerialPort::OpenYaoGan, SerialPort::CloseB, SerialPort::OpenB);
	QThread::msleep(500);																							// 睡上2s钟，等界面稳定下来
	emit m_SignalSend->OpenSwitchMode();																			// 开启全局截屏;
	emit m_SignalSend->StartSelectPlayerCheck();																	// 启动定时器，一直发送手柄打开信号
}

// 当即将进入游戏界面时，将B键的屏蔽打开
void SerialThread::DisAvatarEnter()
{
	if (m_SignalRelay->m_gameFlowInfo->isAvatarEnter)
	{
		qDebug() << QString::fromLocal8Bit("SerialThread::DisAvatarEnter()");
		setTopKOF();																								// 置顶窗口
		// 将B键的屏蔽打开
		emit m_SignalSend->write_1b_2b(SerialPort::OpenB, SerialPort::OpenB);

		// 打开VS灯
		emit m_SignalSend->write_single_register(SerialPort::m_vs, SerialPort::VsOpen);								// 打开vs灯

		// 开启血条的检测
		m_SignalRelay->m_gameFlowInfo->isStartBloodCapture = true;
		m_SignalRelay->m_gameFlowInfo->isAvatarEnter = false;
	}
}

void SerialThread::DisGameOver()
{
	if (m_SignalRelay->m_gameFlowInfo->isGameOver)
	{
		qDebug() << QString::fromLocal8Bit("SerialThread::DisGameOver()");

		// 1.先判断胜利类型，在判断奖励状态最后执行对应的跳转指令
		JudgeRewardStateAndJump();

		// 2.判断奖励状态看是否需要弹出投币界面
		// 读取投币数
		m_SignalRelay->m_gameFlowInfo->reset();																		// 先重置所有游戏状态信息
		emit m_SignalSend->read_registers(SerialPort::MB_REG_SWITCH, 3);
		qDebug() << QString::fromLocal8Bit("SerialThread::DisGameOver() 读取投币数成功");
		QThread::msleep(3000);

		// 重置所有事件与变量,并且重新开始播放最上方小屏幕的过场动画																	
		/*emit m_SignalSend->startMinScreenStandAnimation();	*/														
		std::function<void()> func = m_SignalRelay->m_rewardState == RewardState::NoRewards? 
			std::bind(&SerialThread::DisNoReward_EndLogic, this) : std::bind(&SerialThread::DisHasReward_EndLogic, this);
		func();
	}
}

// 左边赢了，爆闪
void SerialThread::OnEndLeftChangeLight()
{
	// 0.记录胜利
	bIsLeftWin = true;

	emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);							// 关闭两个手柄
	// 1. 改变氛围灯的颜色
	emit m_SignalSend->write_color_led(1,1);
	// 2. 左边win灯亮起
	emit m_SignalSend->write_win_led(SerialPort::WinOpen, SerialPort::WinClose);
	// 3. 改变2P血条的颜色为红色
	emit m_SignalSend->write_Two_hp_color(LIGHT, 0, 0);
	// 4.爆闪
	//int i = 0;
	//while (i < FLASH_TIMES)
	//{
	//	emit m_SignalSend->write_hp(100, 100);
	//	emit m_SignalSend->write_hp(0, 0);
	//	i++;
	//}

	QTimer::singleShot(3000, this, [=]()
		{
			// 5.恢复2P血条的颜色
			emit m_SignalSend->write_Two_hp_color(0, 0, LIGHT);
			// 6.将血条归0
			emit m_SignalSend->write_hp(0, 0);

			// 7.关闭所有的win灯
			emit m_SignalSend->write_win_led(SerialPort::WinClose, SerialPort::WinClose);

			// 8.关闭vs灯
			emit m_SignalSend->write_single_register(SerialPort::m_vs, SerialPort::VsClose);
			// 9.氛围灯恢复正常
			emit m_SignalSend->write_color_led(1, 3);

			// 游戏结束的处理
			DisGameOver();
		});
}

// 右边赢了，爆闪
void SerialThread::OnEndRightChangeLight()
{
	// 0.记录胜利
	bIsLeftWin = false;

	emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);		// 关闭两个手柄
	// 1. 改变氛围灯的颜色
	emit m_SignalSend->write_color_led(3,3);
	// 2. 右边win灯亮起
	emit m_SignalSend->write_win_led(SerialPort::WinClose, SerialPort::WinOpen);
	// 3. 改变1P的血条的颜色为蓝色
	emit m_SignalSend->write_One_hp_color(0, 0, LIGHT);
	// 4.爆闪
	//int i = 0;
	//while (i < FLASH_TIMES)
	//{
	//	emit m_SignalSend->write_hp(100, 100);
	//	emit m_SignalSend->write_hp(0, 0);
	//	i++;
	//}

	QTimer::singleShot(3000, this, [=]()
		{
			// 5.恢复1P血条的颜色
			emit m_SignalSend->write_One_hp_color(LIGHT, 0, 0);
			// 6.将1P血条归0
			emit m_SignalSend->write_hp(0, 0);
			// 7.关闭所有的win灯
			emit m_SignalSend->write_win_led(SerialPort::WinClose, SerialPort::WinClose);
			// 8.关闭vs灯
			emit m_SignalSend->write_single_register(SerialPort::m_vs, SerialPort::VsClose);
			// 9.氛围灯恢复正常
			emit m_SignalSend->write_color_led(1, 3);

			// 游戏结束的处理
			DisGameOver();
		});
}

// 绑定槽函数
void SerialThread::BindSlots()
{
	/*																				游戏流程相关																									*/

	connect(m_SignalSend.get(), &SignalSend::gameBegin, this, &SerialThread::DisGameBegin);														// 游戏开始

	// 主菜单
	connect(m_SignalSend.get(), &SignalSend::mainMenu, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisMainMenu, this));
		});

	// 游戏模式1
	connect(m_SignalSend.get(), &SignalSend::selectedModeOne, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisModeSelectOne, this));
		});

	// 玩家对玩家
	connect(m_SignalSend.get(), &SignalSend::selectedModeTwo, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisModeSelectTwo, this));
		});

	// 玩家对电脑
	connect(m_SignalSend.get(), &SignalSend::selectedModeTwo_PlayerToPC, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisModeSelectTwo_PlayerToPC, this));
		});

	// 电脑对玩家
	connect(m_SignalSend.get(), &SignalSend::selectedModeTwo_PCToPlayer, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisModeSelectTwo_PCToPlayer, this));
		});

	// 玩家确认1
	connect(m_SignalSend.get(), &SignalSend::EnterButton, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisEnterButton, this));
		});

	// 玩家确认2
	connect(m_SignalSend.get(), &SignalSend::EnterButton2, this, [this]()
		{
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisEnterButton2, this));
		});

	// 屏蔽X避免选地图时弹出音效界面
	connect(m_SignalSend.get(), &SignalSend::ShieldXButton, this, [this]()
		{
			if (!m_SignalRelay->m_gameFlowInfo->isSheildX)
			{
				// 屏蔽按键X
				emit m_SignalSend->write_1b_2b(SerialPort::CloseB, SerialPort::CloseB);
				m_SignalRelay->m_gameFlowInfo->isSheildX = true;
			}
		});

	// 当用户手快按下了X键，则发送ESC键返回
	connect(m_SignalSend.get(), &SignalSend::wuchubujiu, this, [this]()
		{
			m_KeyPress->sendEscapeKey();
		});

	// 即将进入游戏，关闭按键B的屏蔽并停止最小屏幕的过场动画
	connect(m_SignalSend.get(), &SignalSend::AvatarEnter, this, [this]()
		{
			// 将血条打开
			emit m_SignalSend->write_hp(100, 100);
			//emit m_SignalSend->stopMinScreenStandAnimation();											// 停止最小屏幕的过场动画
			emit m_SignalSend->AddTaskToThreadPool(std::bind(&SerialThread::DisAvatarEnter, this));
		});

	// 游戏结束屏蔽手柄
	connect(m_SignalSend.get(), &SignalSend::FirstGameEnd, this, [=]()
		{
			// 关闭手柄
			emit m_SignalSend->write_joystick(SerialPort::CloseYaoGan, SerialPort::CloseYaoGan);
		});

	// 本局游戏无论是成功还是失败，win闪烁，以及爆闪
	connect(m_SignalSend.get(), &SignalSend::gameEndSuccess, this, &SerialThread::OnEndLeftChangeLight);
	connect(m_SignalSend.get(), &SignalSend::gameEndFailed, this, &SerialThread::OnEndRightChangeLight);



	/*																				投币相关																										*/

	// 关闭投币轮询
	connect(m_SignalSend.get(), &SignalSend::StopCoinPolling, this, [&]()
		{
			CoinPollTimer->stop();																											// 停止定时器的轮询，此时模式选择已经完毕，ModeMask撤销
			CoinPollTimer_Zero->stop();																										// 停止定时器的轮询，此时模式选择已经完毕，ModeMask撤销
		});

	// 开启投币轮询
	connect(m_SignalSend.get(), &SignalSend::StartCoinPolling, this, [&]()
		{
			CoinPollTimer->start(COIN_POLL_TIME);																							// 开启定时器的轮询
			CoinPollTimer_Zero->start(COIN_POLL_TIME);																						// 开启定时器的轮询
		});
}

// 初始化菜单按钮的位置
void SerialThread::initMenuButtonRect()
{
	m_SingleModeMenuButtonRect = new MenuButtonRect;
	m_SingleModeMenuButtonRect->AgainFightRect = cv::Rect(SinleMenuButtonX, 154, SinleMenuButtonWidth, SinleMenuButtonHeight);
	m_SingleModeMenuButtonRect->SelectSequenceRect = cv::Rect(SinleMenuButtonX, 179, SinleMenuButtonWidth, SinleMenuButtonHeight);
	m_SingleModeMenuButtonRect->SelectMemberRect = cv::Rect(SinleMenuButtonX, 203, SinleMenuButtonWidth, SinleMenuButtonHeight);
	m_SingleModeMenuButtonRect->FightSetRect = cv::Rect(SinleMenuButtonX, 227, SinleMenuButtonWidth, SinleMenuButtonHeight);
	m_SingleModeMenuButtonRect->MainMenuRect = cv::Rect(SinleMenuButtonX, 252, SinleMenuButtonWidth, SinleMenuButtonHeight);

	m_DoubleModeMenuButtonRect = new MenuButtonRect;
	m_DoubleModeMenuButtonRect->AgainFightRect = cv::Rect(DoubleMenuButtonX, 156, DoubleMenuButtonWidth, DoubleMenuButtonHeight);
	m_DoubleModeMenuButtonRect->SelectSequenceRect = cv::Rect(DoubleMenuButtonX, 181, DoubleMenuButtonWidth, DoubleMenuButtonHeight);
	m_DoubleModeMenuButtonRect->SelectMemberRect = cv::Rect(DoubleMenuButtonX, 205, DoubleMenuButtonWidth, DoubleMenuButtonHeight);
	m_DoubleModeMenuButtonRect->FightSetRect = cv::Rect(DoubleMenuButtonX, 229, DoubleMenuButtonWidth, DoubleMenuButtonHeight);
	m_DoubleModeMenuButtonRect->MainMenuRect = cv::Rect(DoubleMenuButtonX, 254, DoubleMenuButtonWidth, DoubleMenuButtonHeight);
}

// 初始化按钮类型与对应图片的映射
void SerialThread::initMenuButtonMap()
{
	CaptureStatic::LoadImages(MenuButtonImgMap);
}

// 初始化按钮类型与对应区域的映射
void SerialThread::initMenuButtonRectMap()
{
	MenuButtonRectMap.insert({ EndMenuButtonType::SingleAgainFight, m_SingleModeMenuButtonRect->AgainFightRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::SingleSelectSequence, m_SingleModeMenuButtonRect->SelectSequenceRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::SingleSelectMember, m_SingleModeMenuButtonRect->SelectMemberRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::SingleFightSet, m_SingleModeMenuButtonRect->FightSetRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::SingleMainMenu, m_SingleModeMenuButtonRect->MainMenuRect });

	MenuButtonRectMap.insert({ EndMenuButtonType::DoubleAgainFight, m_DoubleModeMenuButtonRect->AgainFightRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::DoubleSelectSequence, m_DoubleModeMenuButtonRect->SelectSequenceRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::DoubleSelectMember, m_DoubleModeMenuButtonRect->SelectMemberRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::DoubleFightSet, m_DoubleModeMenuButtonRect->FightSetRect });
	MenuButtonRectMap.insert({ EndMenuButtonType::DoubleMainMenu, m_DoubleModeMenuButtonRect->MainMenuRect });

	// 游戏模式 玩家对玩家，玩家对电脑
	MenuButtonRectMap.insert({ EndMenuButtonType::GameModeDouble, cv::Rect(216,173,584 - 216, 20) });
	MenuButtonRectMap.insert({ EndMenuButtonType::GameModeSingle, cv::Rect(216,196,584 - 216, 20) });
}

// 再次进行匹配
EndMenuButtonType SerialThread::AgainCropMatch(int ButtonType)
{
	cv::Mat image = CaptureStatic::CaptureFullScreen();
	
	// 获取对应按钮的区域
	auto it = MenuButtonRectMap.find(ButtonType);
	if (it == MenuButtonRectMap.end())
	{
		qDebug() << QString::fromLocal8Bit("AgainCropMatch : 未找到对应按钮类型");
		return None;
	}
	cv::Rect rect = MenuButtonRectMap[ButtonType];
	if (rect.empty())
	{
		qDebug() << QString::fromLocal8Bit("AgainCropMatch : rect 是 空的");
		return None;
	}
	cv::Mat matchImg = image(rect);

	// 旧版(图片匹配)
	/*ButtonType = CaptureStatic::CompareImage(matchImg, MenuButtonImgMap, THREASOLD);*/
	//qDebug() << "AgainCropMatch : buttonType: " << ButtonType;
	//EndMenuButtonType currButtonType = (EndMenuButtonType)ButtonType;

	// 新版(文本识别)
	cv::Mat templateImg = MenuButtonImgMap[ButtonType];
	bool ret = TextRecognition::GetInstance()->CompareImageText(matchImg, templateImg);
	EndMenuButtonType currButtonType = ret ? (EndMenuButtonType)ButtonType : None;
	qDebug() << "AgainCropMatch : buttonType: " << ButtonType;

	return currButtonType;
}

/* 计算游戏胜利类型 */
GameWinType SerialThread::CalcGameWinType()
{
	GameWinType gameWinType = GameWinType::NoneGameWinType;

	// 单人模式1P胜利 
	if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PlayerToPCMode)
	{
		gameWinType = bIsLeftWin ? GameWinType::LeftPlayerWin_Single : GameWinType::PlayerFalied;
	}
	// 单人模式2P胜利
	else if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PCToPlayerMode)
	{
		gameWinType = bIsLeftWin ? GameWinType::PlayerFalied : GameWinType::RightPlayerWin_Single;
		/*gameWinType = GameWinType::PlayerFalied;*/
	}
	// 双人模式1P胜利
	else if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PlayerToPlayerMode)
	{
		gameWinType = bIsLeftWin ? GameWinType::LeftPlayerWin_Double : GameWinType::RightPlayerWin_Double;
	}
	// 双人模式2P胜利
	else if (m_SignalRelay->m_gameFlowInfo->CurrGameMode == GameMode::PlayerToPlayerMode)
	{
		gameWinType = bIsLeftWin ? GameWinType::LeftPlayerWin_Double : GameWinType::RightPlayerWin_Double;
	}

	DeferRaii deferRaii([this, gameWinType]()
		{
			this->PrintWinType(gameWinType);
		});

	return gameWinType;
}

/* 判断奖励状态并跳转 */
void SerialThread::JudgeRewardStateAndJump()
{
	// 1.计算游戏胜利类型
	m_CurrGameWinType = CalcGameWinType();

	// 2.根据胜利类型先判断当前奖励状态在进行游戏界面跳转
	// 失败
	if (m_CurrGameWinType == GameWinType::PlayerFalied || m_SignalRelay->xuwanCount == 0)
	{
		m_SignalRelay->m_rewardState = RewardState::NoRewards;
		FaliedGameJump();
		return;
	}
	// 胜利，判断奖励状态来实现游戏界面跳转
	else
	{
		if (m_SignalRelay->m_rewardState == NoRewards)
		{	
			DisNoReward(m_CurrGameWinType);
		}
		else if (m_SignalRelay->m_rewardState == HasRewards)
		{
			DisHasReward(m_CurrGameWinType);
		}
	}
}

/* 失败的游戏跳转 */
void SerialThread::FaliedGameJump()
{
	qDebug() << QString::fromLocal8Bit("进行失败跳转");

	if (m_SignalRelay->m_gameFlowInfo->PlayerNums == 1)
	{
		// 单人ESC按键匹配
		KeySend(ESC, SingleAgainFight);

		// 单人Up按键匹配
		KeySend(UP, SingleMainMenu);

		// 单人Enter按键匹配
		KeySend(ENTER, SingleMainMenu);
	}
	else if (m_SignalRelay->m_gameFlowInfo->PlayerNums == 2)
	{
		// 双人ESC按键匹配
		KeySend(ESC, DoubleAgainFight);

		// 双人Up按键匹配
		KeySend(UP, DoubleMainMenu);

		// 双人Enter按键匹配
		KeySend(ENTER, DoubleMainMenu);
	}
}

/* 成功的单人模式游戏跳转 */
void SerialThread::SingleGameJump()
{
	qDebug() << QString::fromLocal8Bit("进行单人模式跳转");
	// ESC按键匹配
	KeySend(ESC, SingleAgainFight);

	// Down按键匹配
	KeySend(DOWN, SingleSelectSequence);

	// Down按键匹配
	KeySend(DOWN, SingleSelectMember);

	// Enter按键匹配
	KeySend(ENTER, SingleSelectMember);

	qDebug() << QString::fromLocal8Bit("选择成员跳转");
}

/* 成功的双人模式游戏跳转 */
void SerialThread::DoubleGameJump()
{
	qDebug() << QString::fromLocal8Bit("进行双人模式跳转");
	// 双人ESC按键匹配
	KeySend(ESC, DoubleAgainFight);

	// 双人Up按键匹配
	KeySend(UP, DoubleMainMenu);

	// 双人Enter按键匹配
	KeySend(ENTER, DoubleMainMenu);
}

/* 向游戏发送虚拟按键信号 */
void SerialThread::KeySend(KeyType keyType, EndMenuButtonType buttonType)
{
	QString str;
	// 发送虚拟按键信号
	if (keyType == KeyType::UP)
	{
		m_KeyPress->sendUpKey();
		str = QString::fromLocal8Bit("UP按键发送");
	}
	if (keyType == KeyType::DOWN)
	{
		m_KeyPress->sendDownKey();
		str = QString::fromLocal8Bit("DOWN按键发送");
	}
	if (keyType == KeyType::ENTER)
	{
		m_KeyPress->sendEnterKey();
		str = QString::fromLocal8Bit("回车键发送");
	}
	if (keyType == KeyType::ESC)
	{
		m_KeyPress->sendEscapeKey();
		str = QString::fromLocal8Bit("ESC按键发送");
	}

	while (1)
	{
		setTopKOF();
		QThread::msleep(1000);
		EndMenuButtonType currButtonType = AgainCropMatch(buttonType);
		
		if (keyType == KeyType::ENTER)
		{
			if (currButtonType != buttonType)
			{
				qDebug() << str + QString::fromLocal8Bit("成功");
				break;
			}
			else
			{
				qDebug() << str + QString::fromLocal8Bit("失败");
			}
		}
		else
		{
			if (currButtonType == buttonType)
			{
				qDebug() << str + QString::fromLocal8Bit("成功");
				break;
			}
			else
			{
				qDebug() << str + QString::fromLocal8Bit("失败");
			}
		}
	}
}

void SerialThread::DisHasReward(GameWinType winType)
{
	qDebug() << QString::fromLocal8Bit("处理有奖励状态");
	qDebug() << QString::fromLocal8Bit("奖励剩余: ") << m_SignalRelay->CurrXuWanEnableCount;

	// 先判断是否还有奖励
	if (m_SignalRelay->CurrXuWanEnableCount == 0)
	{
		m_SignalRelay->m_rewardState = RewardState::NoRewards;
		qDebug() << QString::fromLocal8Bit("奖励已用完，进行失败跳转");
		FaliedGameJump();
		return;
	}
	m_SignalRelay->CurrXuWanEnableCount--;

	// 单人模式胜利
	if (winType == GameWinType::LeftPlayerWin_Single || winType == GameWinType::RightPlayerWin_Single)
	{
		SingleGameJump();
	}
}

void SerialThread::DisNoReward(GameWinType winType)
{
	qDebug() << QString::fromLocal8Bit("处理无奖励状态");

	// 设置奖励状态
	m_SignalRelay->m_rewardState = RewardState::HasRewards;
	m_SignalRelay->CurrXuWanEnableCount = m_SignalRelay->xuwanCount;
	qDebug() << QString::fromLocal8Bit("奖励剩余: ") << m_SignalRelay->CurrXuWanEnableCount;
	m_SignalRelay->CurrXuWanEnableCount--;

	// 单人模式胜利
	if (winType == GameWinType::LeftPlayerWin_Single || winType == GameWinType::RightPlayerWin_Single)
	{
		SingleGameJump();
	}
	// 双人模式胜利
	if (winType == GameWinType::LeftPlayerWin_Double || winType == GameWinType::RightPlayerWin_Double)
	{
		DoubleGameJump();
	}
}

void SerialThread::DisHasReward_EndLogic()
{
	qDebug() << "DisHasReward_EndLogic";

	// 确定游戏模式
	m_SignalRelay->m_gameFlowInfo->PlayerNums = 1;
	m_SignalRelay->m_gameFlowInfo->isPress1P2P = true;																				// 1P或者2P已按下
	emit m_SignalSend->write_single_register(SerialPort::m_charge, SerialPort::ChargeOpen);											// 打开充电灯	

	// 玩家对玩家，左边玩家胜利
	if (m_CurrGameWinType == GameWinType::LeftPlayerWin_Double)
	{
		qDebug() << QString::fromLocal8Bit("开启玩家对电脑模式");
		m_SignalRelay->m_gameFlowInfo->OneP_Single = 1;
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PlayerToPCMode;
	}

	// 玩家对玩家，右边玩家胜利
	if (m_CurrGameWinType == GameWinType::RightPlayerWin_Double)
	{
		qDebug() << QString::fromLocal8Bit("开启电脑对玩家模式");
		m_SignalRelay->m_gameFlowInfo->TwoP_Single = 1;
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PCToPlayerMode;
	}

	// 玩家对电脑，玩家胜利
	if (m_CurrGameWinType == GameWinType::LeftPlayerWin_Single)
	{
		emit m_SignalSend->write_joystick_1b2b(SerialPort::OpenYaoGan, SerialPort::OpenYaoGan, SerialPort::CloseB, SerialPort::OpenB);
		m_SignalRelay->m_gameFlowInfo->OneP_Single = 1;
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PlayerToPCMode;
	}

	// 电脑对玩家, 玩家胜利
	if (m_CurrGameWinType == GameWinType::RightPlayerWin_Single)
	{
		emit m_SignalSend->write_joystick_1b2b(SerialPort::OpenYaoGan, SerialPort::OpenYaoGan, SerialPort::CloseB, SerialPort::OpenB);
		m_SignalRelay->m_gameFlowInfo->TwoP_Single = 1;
		m_SignalRelay->m_gameFlowInfo->CurrGameMode = GameMode::PCToPlayerMode;
	}
	emit m_SignalSend->OpenSwitchMode();
}

void SerialThread::DisNoReward_EndLogic()
{
	qDebug() << "DisNoReward_EndLogic";

	// 收费
	if (m_SignalRelay->settingCoin != 0)
	{
		qDebug() << QString::fromLocal8Bit("收费模式");

		// 判断投币数
		if (m_SignalRelay->coin < m_SignalRelay->settingCoin)
		{
			qDebug() << QString::fromLocal8Bit("投币数不足");
			/* 发送视频 需要监听按键信号 */
			emit m_SignalSend->StartPlayMoive();
			emit m_SignalSend->write_single_register(SerialPort::m_charge, SerialPort::ChargeClose);			// 关闭充电灯
		}
		else
		{
			qDebug() << QString::fromLocal8Bit("投币数足够");
		}

		qDebug() << QString::fromLocal8Bit("收费模式，启动收费模式的投币轮询监听");
		CoinPollTimer->start(COIN_POLL_TIME);
	}
	// 免费
	else
	{
		qDebug() << QString::fromLocal8Bit("免费模式, 启动免费模式的投币轮询监听");
		CoinPollTimer_Zero->start(COIN_POLL_TIME);
		ModeMaskWidget::GetInstance()->showMask();																// 显示等待用户按下1p还是2p的遮罩
	}
}

/* 打印游戏胜利类型 */
void SerialThread::PrintWinType(GameWinType winType)
{
	switch (winType)
	{
	case GameWinType::LeftPlayerWin_Single:
		qDebug() << QString::fromLocal8Bit("单人模式: 左边玩家胜利");
		break;
	case GameWinType::RightPlayerWin_Single:
		qDebug() << QString::fromLocal8Bit("单人模式：右边玩家胜利");
		break;
	case GameWinType::LeftPlayerWin_Double:
		qDebug() << QString::fromLocal8Bit("双人模式：左边玩家胜利");
		break;
	case GameWinType::RightPlayerWin_Double:
		qDebug() << QString::fromLocal8Bit("双人模式：右边玩家胜利");
		break;
	case GameWinType::PlayerFalied:
		qDebug() << QString::fromLocal8Bit("失败");
		break;
	default:
		break;
	}
}