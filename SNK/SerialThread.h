#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <windows.h>
#include <QObject>
#include <QWindow>
#include <QTimer>
#include "SignalRelay.h"
#include "SignalSend.h"
#include "KeyPress.h"
#include "ModeMaskWidget.h"
#include "Enum.h"

// 菜单按钮的位置
struct MenuButtonRect
{
	cv::Rect AgainFightRect;																				// 再次战斗
	cv::Rect SelectSequenceRect;																			// 选择顺序
	cv::Rect SelectMemberRect;																				// 选择成员
	cv::Rect FightSetRect;																					// 战斗设置
	cv::Rect MainMenuRect;																					// 主菜单

	MenuButtonRect() {}
};


class SerialThread  : public QObject
{
	Q_OBJECT

public slots:
	void onTimerSerial();
public:
	SerialThread(QObject *parent = nullptr);
	~SerialThread();

	// 切换遮罩
	void switchMask();

	// 当投币数为0时，需要监听1P2P信号
	void onCoinZero();

	// 将拳皇程序置顶
	void setTopKOF();

	// 处理发过来的信号
	void DisGameBegin();																					// 游戏开始界面
	void DisMainMenu();																						// 主菜单界面
	void DisModeSelectOne();																				// 选择模式界面1
	void DisModeSelectTwo();																				// 选择模式界面2 根据玩家按下来选择是玩家对玩家(发送向下按键)还是玩家对电脑(发送enter键)
	void DisModeSelectTwo_PlayerToPC();																		// 选择模式界面2P 玩家对电脑 按键Enter 或者 按键Down
	void DisModeSelectTwo_PCToPlayer();																		// 选择模式界面2P 电脑对玩家 按键Enter
	void DisEnterButton();																					// 处理按钮按下的界面
	void DisEnterButton2();																					// 处理按钮按下的界面2P
	void DisAvatarEnter();																					// 处理角色进入界面
	void DisGameOver();																						// 游戏结束界面,判断是否是左边玩家胜利
	void OnEndLeftChangeLight();																			// 左边赢了 win灯 + 爆闪
	void OnEndRightChangeLight();																			// 右边赢了 win灯 + 爆闪
	// 绑定槽函数
	void BindSlots();

	// 结束菜单按钮的类型确认
	void initMenuButtonRect();																				// 初始化菜单按钮的区域
	void initMenuButtonMap();																				// 初始化菜单按钮的图片
	void initMenuButtonRectMap();																			// 初始化菜单按钮的区域
	EndMenuButtonType AgainCropMatch(int ButtonType);														// 根据按键的类型再次匹配

	// 续玩游戏流程
	GameWinType CalcGameWinType();																			// 计算游戏结果
	void JudgeRewardStateAndJump();																			// 判断奖励状态并跳转

	// 游戏跳转
	void FaliedGameJump();																					// 失败游戏跳转
	void SingleGameJump();																					// 单人游戏跳转
	void DoubleGameJump();																					// 双人游戏跳转
	void KeySend(KeyType keyType, EndMenuButtonType buttonType);											// 按键发送

	// 处理奖励状态
	void DisHasReward(GameWinType winType);																	// 处理有奖励状态
	void DisNoReward(GameWinType winType);																	// 处理无奖励状态
	void DisHasReward_EndLogic();																			// 处理有奖励状态的结束逻辑
	void DisNoReward_EndLogic();																			// 处理无奖励状态的结束逻辑

	// 打印
	void PrintWinType(GameWinType winType);																	// 打印游戏结果

public:
	// 拳皇句柄
	HWND hWnd;

	// 信号发送对象
	QSharedPointer<KeyPress> m_KeyPress;																	// 模拟enter按键
	QSharedPointer<SignalRelay> m_SignalRelay;																// 信号中转类
	QSharedPointer<SignalSend> m_SignalSend;																// 信号发射器
	QTimer* CoinPollTimer;																					// 投币轮询定时器
	QTimer* CoinPollTimer_Zero;																				// 投币数为0的投币轮询定时器

	// 按钮类型
	map<int, cv::Mat>	MenuButtonImgMap;																	// 存放主菜单按钮的图片
	map<int, cv::Rect> MenuButtonRectMap;																	// 存放主菜单按钮的区域
	MenuButtonRect* m_SingleModeMenuButtonRect;																// 单人模式菜单按钮的区域
	MenuButtonRect* m_DoubleModeMenuButtonRect;																// 双人模式菜单按钮的区域

	// 游戏1P2P界面发送玩家选择信号
	map<string, cv::Mat> m_ImageMap;																		// 存放800 * 450 的 游戏流程的图片

	// 续玩游戏流程
	bool bIsLeftWin = false;																				// 左边玩家是否胜利

	// 当前胜利状态
	GameWinType m_CurrGameWinType = NoneGameWinType;
};
#endif // SERIALTHREAD_H