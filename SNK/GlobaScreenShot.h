#ifndef GLOBASCREENSHOT_H
#define GLOBASCREENSHOT_H
#include <opencv2/opencv.hpp>
#include <QObject>
#include <map>
using namespace std;
#include "SignalSend.h"
#include "CaptureStatic.h"
#include "SignalRelay.h"
#include "Interface.h"

class GlobaScreenShot  : public Interface
{
	Q_OBJECT

public:
	GlobaScreenShot(QObject *parent = nullptr);
	~GlobaScreenShot();

	void mainScreenShot();																			// 主界面截图
	void init();																					// 初始化,加载所需要比对的图片
	// 处理游戏截图
	void DisGameScreenShot(cv::Mat img,cv::Rect rect, map<string, cv::Mat> &Map);					// 游戏截图

	void sendSignal(string str);																	// 发送信号
	void BindSlot();																				// 绑定槽函数
	void startGame();																				// 开始游戏
	
	void JudeFightIsEnd();																			// 判断一局战斗是否结束

signals:
	void gameBegin();																				// 游戏开始
	void mainMenu();																				// 主菜单
	void selectedModeOne();																			// 选择模式1
	void selectedModeTwo();																			// 选择模式2
	void EnterButton();																				// 开始游戏
	void ShieldXButton();																			// 屏蔽按键X
	void wuchubujiu();																				// 误触补救
	void AvatarEnter();																				// 角色确认
	void gameEndSuccess();																			// 游戏结束，左边胜利
	void gameEndFail();																				// 游戏结束，右边胜利
	void FirstGameEnd();																			// 三局游戏结束，依据白屏来判断，然后屏蔽手柄按键

public slots:
	void onTimeDisScreenShot();																		// 定时处理截取的图片
	void OpenSwitchMode();																			// 打开模式切换开关
	void CloseSwitchMode();
	virtual void InitSelf() override;
	void StartSendHandleSignal();																	// 开始发送手柄信号

private:
	map<string, cv::Mat> m_ImageMap;																// 存放800 * 450 的 游戏流程的图片
	map<string, cv::Mat> m_AvatarEnterMap;															// 存放角色确认的图片(394 * 67)
	map<string, cv::Mat> m_GameEndMap;																// 存放游戏结束的图片(71 * 42)
	map<string, cv::Mat> m_ShieldXMap;																// 屏蔽X按键
	cv::Rect m_AvatarEnterRect;																		// 角色确认区域
	cv::Rect m_GameEndRect;																			// 游戏结束，左边胜利区域
	cv::Rect m_ShieldeRect;																			// 屏蔽X区域

	QSharedPointer<SignalRelay> m_SignalRelay;														// 信号中转类
	QSharedPointer<SignalSend> m_SignalSend;														// 信号发射器

	cv::Mat m_PreCaptureImg;																		// 上一次的游戏截图
	cv::Rect m_CountDownRect;																		// 倒计时区域
	QTimer* m_CountDownTimer;																		// 倒计时定时器

	bool m_SwitchMode;																				// 模式开关
	bool m_IsWhiteScreen;																			// 是否是白屏

	QTimer* m_handleTimer;																			// 实时发送手柄开启信号(打开手柄并屏蔽按键B)
};
#endif // GLOBASCREENSHOT_H