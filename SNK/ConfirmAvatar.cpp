#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QScreen>
#include <QGridLayout>
#include <QGuiApplication>

#include "ConfirmAvatar.h"
#include "CaptureStatic.h"
#include "SwitchGame.h"
#include "SerialPort.h"

#define threslods 0.70
#define sleepNmus 5
#define VecSize 3

#define LeftScreenIndex 1
#define RightScreenIndex 2

#define oneScreen 3
#define twoScreen 2
#define minScreen 1

#define minScreenWidth 512
#define minScreenHeight 200
#define labelWidth 256
#define labelHeight 172

#define winNum 30

#define ONEP_IMAGE_INDEX 80
#define TWOP_IMAGE_INDEX 81
#define TWOP_IMAGE_INDEX_1 82
#define TWOP_IMAGE_INDEX_2 83

#define GRAY_TIMER_INTERVAL 100

#define GrayAnimationNums 31

ConfirmAvatar::ConfirmAvatar(QObject *parent)
	:showScreen1(new ShowScreen(nullptr, oneScreen)), 
	showScreen2(new ShowScreen(nullptr, twoScreen)),
	showMinScreen(new ShowMinScreen)
{
	Init();
}

ConfirmAvatar::~ConfirmAvatar()
{
	if(timer)
		delete timer;
	if(GrayTimer)
		delete GrayTimer;
}

/* 启动播放灰色动画的定时器 */
void ConfirmAvatar::StartGrayTimer()
{
	GrayTimer->start(GRAY_TIMER_INTERVAL);
}

// 初始化
void ConfirmAvatar::Init()
{
	leftRect = cv::Rect(14, 0, 51 - 14, 38);
	rightRect = cv::Rect(748, 0, 784 - 748, 38);

	string leftPath = "leftAvatar1";
	string rightPath = "rightAvatar1";

	CaptureStatic::LoadImages(leftPath, leftAvatarMap);															// 初始化用于匹配的左侧图像
	CaptureStatic::LoadImages(rightPath, rightAvatarMap);														// 初始化用于匹配的右侧图像

	CaptureStatic::LoadAnimPixmaps(GrayPixmapsOverlay, "./GrayAnimation/Failed_", GrayAnimationNums);			// 加载灰色动画的覆盖层

	lastLeftMatchingName = "";
	lastRightMatchingName = "";

	// 初始化图片路径
	leftMinScreenPath = ":leftMinScreenImage1";
	rightMinScreenPath = ":rightMinScreenImage1";
	leftMinBlackWhitePath = ":leftAvatarBlackWhite";
	rightMinBlackWhitePath = ":rightAvatarBlackWhite";
	
	// 获取唯一的指针
	m_SignalRelay = SignalRelay::GetInstance();
	m_SignalSend  = SignalSend::GetInstance();

	showMinScreen->setScreen(minScreen, minScreenWidth, minScreenHeight);

	GrayTimer = new QTimer(this);

	BindSlot();

	OnTimerConfirm();
}

// 裁剪图像并匹配
void ConfirmAvatar::CropAvatarAndMatch()
{
	// 如果角色选定完毕	
	if (m_SignalRelay->m_gameFlowInfo->isStartBloodCapture)
	{
		// 图片对比
		// 左侧匹配 *************************************************************************************************************************//
		static string matchLeftName = "";

		cv::Mat image = CaptureStatic::CaptureFullScreen();																																		// 循环截图
		cv::Mat leftAvatar = image(leftRect);																																					// 裁剪
		matchLeftName = CaptureStatic::CharacterMatch(leftAvatar, leftAvatarMap, threslods) != "" ? CaptureStatic::CharacterMatch(leftAvatar, leftAvatarMap, threslods) : matchLeftName;		// 匹配

		if (matchLeftName != "" && matchLeftName != "white")
		{
			// 插入唯一元素
			CaptureStatic::insertUnique(leftMatchingNames, matchLeftName);
		}

		//qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  leftMatchingNames.size()  =  " << leftMatchingNames.size();
		//qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  matchLeftName  =  " << QString::fromLocal8Bit(matchLeftName.c_str());

		// 右侧匹配 *************************************************************************************************************************//	
		static string matchRightName = "";

		cv::Mat rightAvatar = image(rightRect);																																					// 裁剪
		matchRightName = CaptureStatic::CharacterMatch(rightAvatar, rightAvatarMap, threslods) != "" ? CaptureStatic::CharacterMatch(rightAvatar, rightAvatarMap, threslods) : matchRightName;	// 匹配

		if (matchRightName != "" && matchRightName != "white")
		{
			// 插入唯一元素
			CaptureStatic::insertUnique(rightMatchingNames, matchRightName);
		}

		//qDebug() << "ConfirmAvatar::CropAvatarAndSave():  rightMatchingNames.size()  =  " << rightMatchingNames.size();
		//qDebug() << "ConfirmAvatar::CropAvatarAndSave():  matchRightName  =  " << QString::fromLocal8Bit(matchRightName.c_str());

		// 显示小屏幕 ***********************************************************************************************************************//
		// 刷新显示
		if (matchLeftName != "" && matchRightName != "")
		{
			int leftIndex = CaptureStatic::FindIndex(matchLeftName);
			int rightIndex = CaptureStatic::FindIndex(matchRightName);
			if (leftIndex == -1 || rightIndex == -1)
			{
				showDefaultBackground(); // 显示默认背景
				return;
			}
			// 最上面小屏幕需要播放的图片
			QPixmap leftPixmap = CaptureStatic::GetQPixmap(leftMinScreenPath, leftIndex);
			QPixmap rightPixmap = CaptureStatic::GetQPixmap(rightMinScreenPath, rightIndex);
			if (leftPixmap.isNull() || rightPixmap.isNull())
			{
				qDebug() << "ConfirmAvatar::CropAvatarAndSave():  QPixmap is Null";
			}
			showMinScreen->SetImage(leftPixmap, rightPixmap, labelWidth, labelHeight);

			// 显示在两个显示器上的图片
			showScreen1->setImageIndex(leftIndex);
			showScreen2->setImageIndex(rightIndex);
		}
		// 如果匹配结果为空，直接显示空白背景
		else if (matchLeftName == "" && matchRightName == "")
		{
			showDefaultBackground(); // 显示默认背景
		}
	}
}

// 显示右侧灰色头像
void ConfirmAvatar::ShowRightGray()
{
	// 灰色头像显示
	if (leftMatchingNames.size() > 0 && rightMatchingNames.size() > 0)
	{
		_GrayLabelType = RightGrayLabel;															// 右侧播放灰色头像的动画
		int leftIndex = CaptureStatic::FindIndex(leftMatchingNames.back());							// 最后一个匹配的角色，最后一个角色即时当前角色
		int rightIndex = CaptureStatic::FindIndex(rightMatchingNames.back());						// 最后一个匹配的角色，最后一个角色即时当前角色

		QPixmap leftPixmap = CaptureStatic::GetQPixmap(leftMinScreenPath, leftIndex);
		QPixmap rightPixmap = CaptureStatic::GetQPixmap(rightMinBlackWhitePath, rightIndex);

		// 拼接需要播放的灰色动画，播放灰色动画
		GrayPixmaps = showMinScreen->MergeImages(rightPixmap, GrayPixmapsOverlay);
		StartGrayTimer();

		showMinScreen->SetImage(leftPixmap, rightPixmap, labelWidth, labelHeight);
		timer->stop();
		QTimer::singleShot(5000, [=]()
			{
				timer->start(1000);
			});
		qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  show BlackWhite in right";
	}
}

// 显示左侧灰色头像
void ConfirmAvatar::ShowLeftGray()
{
	// 灰色头像显示
	if (leftMatchingNames.size() > 0 && rightMatchingNames.size() > 0)
	{
		_GrayLabelType = LeftGrayLabel;																// 左侧播放灰色头像的动画
		int leftIndex = CaptureStatic::FindIndex(leftMatchingNames.back());
		int rightIndex = CaptureStatic::FindIndex(rightMatchingNames.back());
		
		QPixmap leftPixmap = CaptureStatic::GetQPixmap(leftMinBlackWhitePath, leftIndex);
		QPixmap rightPixmap = CaptureStatic::GetQPixmap(rightMinScreenPath, rightIndex);

		// 拼接需要播放的灰色动画，播放灰色动画
		GrayPixmaps = showMinScreen->MergeImages(leftPixmap, GrayPixmapsOverlay);
		StartGrayTimer();

		//showMinScreen->SetImage(leftPixmap, rightPixmap, labelWidth, labelHeight);
		timer->stop();
		QTimer::singleShot(5000, [=]()
			{
				timer->start(1000);
			});
		qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  show BlackWhite in left";
	}
}

// 显示默认背景
void ConfirmAvatar::showDefaultBackground()
{
	QPixmap temp; // 空白
	showMinScreen->SetImage(temp, temp, labelWidth, labelHeight);

	showScreen1->setImageIndex(59);
	showScreen2->setImageIndex(59);
	qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  matching is Null";
}

// 绑定槽函数
void ConfirmAvatar::BindSlot()
{
	connect(m_SignalSend.get(), &SignalSend::gameEndSuccess, this, [=]()
		{
			OnGameEnd(); // 当游戏结束时，重新定义变量
			qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  Blue is Winner !!!!!!!!!!!!!!!!!!!!!!";
		});
	connect(m_SignalSend.get(), &SignalSend::gameEndFailed, this, [=]()
		{
			OnGameEnd(); // 当游戏结束时，重新定义变量
			qDebug() << "ConfirmAvatar::CropAvatarAndMatch():  Red is Winner !!!!!!!!!!";
		});

	// 如果按下的是1p，则右边的屏幕显示请移步到左边开始玩
	connect(m_SignalSend.get(), &SignalSend::OnePKeyPress_Single, showScreen2.get(), [this]()
		{
			showScreen1->setImageIndex(ONEP_IMAGE_INDEX);
			showScreen2->setImageIndex(TWOP_IMAGE_INDEX_1);
		});
	// 如果按下的是2p，则右边的屏幕显示左边是1p，右边是2p
	connect(m_SignalSend.get(), &SignalSend::DoubleModeKeyPress, showScreen2.get(), [this]()
		{
			showScreen1->setImageIndex(ONEP_IMAGE_INDEX);
			showScreen2->setImageIndex(TWOP_IMAGE_INDEX_2);
		});

	// 如果左边胜利了
	connect(m_SignalSend.get(), &SignalSend::leftWin, this, &ConfirmAvatar::ShowRightGray);
	// 如果右边胜利了
	connect(m_SignalSend.get(), &SignalSend::rightWin, this, &ConfirmAvatar::ShowLeftGray);
	
	// 游戏结束，最后胜利的那个角色的索引
	connect(this, &ConfirmAvatar::endAvatrSuccess, m_SignalSend.get(), &SignalSend::endAvatrSuccess);

	// 播放灰色头像动画
	connect(GrayTimer, &QTimer::timeout, this, &ConfirmAvatar::showGrayAnimation);

	// 绑定小屏幕的待机动画
	connect(m_SignalSend.get(), &SignalSend::startMinScreenStandAnimation, showMinScreen, &ShowMinScreen::StartAnimation);
	connect(m_SignalSend.get(), &SignalSend::stopMinScreenStandAnimation, showMinScreen, &ShowMinScreen::StopAnimation);
}

// 当游戏结束时
void ConfirmAvatar::OnGameEnd()
{
	leftMatchingNames.clear();								// 清空角色列表
	rightMatchingNames.clear();								// 清空角色列表
	showDefaultBackground();								// 显示默认背景
}

/* 播放灰色头像动画 */
void ConfirmAvatar::showGrayAnimation()
{
	switch (_GrayLabelType)
	{
	case LeftGrayLabel:
		showMinScreen->SetLeftLabelImage(GrayPixmaps[GrayIndex], labelWidth, labelHeight);
		break;
	case RightGrayLabel:
		showMinScreen->SetRightLabelImage(GrayPixmaps[GrayIndex], labelWidth, labelHeight);
		break;
	default:
		break;
	}

	GrayIndex = (GrayIndex + 1) % GrayPixmaps.size();
	if (GrayIndex == 0)
	{
		_GrayLabelType = NoneGrayLabel;														// 没有标签需要展示灰色头像
		GrayTimer->stop();
		GrayPixmaps.clear();																// 清空动画
	}
}

// 线程对应的槽函数
void ConfirmAvatar::OnTimerConfirm()
{
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &ConfirmAvatar::CropAvatarAndMatch);
	timer->start(1000); // 0.4秒一次*/

	// 开始投币的时候的闪烁
	QTimer* timer2 = new QTimer(this);
	connect(timer2, &QTimer::timeout, this, [=]()
		{
			static int index = 59;
			index = 59 + (index + 1 - 59) % 2;  // 在 59 和 60 之间循环
			if (showScreen1) {
				showScreen1->setImageIndex(index);
			}
			if (showScreen2) {
				showScreen2->setImageIndex(index);
			}
		},Qt::QueuedConnection);

	// 游戏的CG动画开始的时候，进行闪烁
	connect(m_SignalSend.get(), &SignalSend::SecondScreenShowText, this, [=]()
		{
			if (!timer2 || !showScreen1 || !showScreen2) {
				qWarning() << "Timer or screen objects are null!";
				return;
			}
			timer2->start(500);
			qDebug() << QString::fromLocal8Bit("ConfirmAvatar::OnTimerConfirm(): 闪烁请投币");
		},Qt::QueuedConnection);

	// 开始投币的时候，结束副屏的投币文本闪烁,副屏显示1p和2p的图片
	connect(m_SignalSend.get(), &SignalSend::stopCoinFlash, this, [=]()
		{
			timer2->stop();
			showScreen1->setImageIndex(ONEP_IMAGE_INDEX);
			showScreen2->setImageIndex(TWOP_IMAGE_INDEX);
		}, Qt::QueuedConnection);

	// 游戏结束的时候开始闪烁,创建两个定时器
	// 1. 闪烁1p赢了
	QTimer *timer3 = new QTimer(this);
	connect(timer3, &QTimer::timeout, this, [=]()
		{
			static int index = 88;
			index = 88 + (index + 1 - 88) % 2;  // 在 88 和 89 之间循环
			if (showScreen1)
			{
				showScreen1->setImageIndex(index);
			}
			if (showScreen2)
			{
				showScreen2->setImageIndex(index);
			}
		}, Qt::QueuedConnection);

	// 游戏一结束就触发,左边赢了
	connect(m_SignalSend.get(), &SignalSend::gameEndSuccess, this, [=]()
		{
			timer3->start(250);
			QTimer::singleShot(9000, [=]()
				{
					timer3->stop();
					showDefaultBackground();
				});
		}, Qt::QueuedConnection);

	// 2. 闪烁2p赢了
	QTimer *timer4 = new QTimer(this);
	connect(timer4, &QTimer::timeout, this, [=]()
		{
			static int index = 98;
			index = 98 + (index + 1 - 98) % 2;  // 在 98 和 99 之间循环
			if (showScreen1)
			{
				showScreen1->setImageIndex(index);
			}
			if (showScreen2)
			{
				showScreen2->setImageIndex(index);
			}
		}, Qt::QueuedConnection);

	// 游戏一结束就触发,右边赢了
	connect(m_SignalSend.get(), &SignalSend::gameEndFailed, this, [=]()
		{
			timer4->start(250);
			QTimer::singleShot(9000, [=]()
				{
					timer4->stop();
					showDefaultBackground();
				});
		}, Qt::QueuedConnection);
	qDebug() << QString::fromLocal8Bit("确认线程ID: ") << QThread::currentThreadId();
}