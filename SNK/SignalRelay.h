#ifndef SIGNALRELAY_H
#define SIGNALRELAY_H

#include <QObject>
#include <QDateTime>
#include <vector>
#include <string>

#include "Enum.h"
#include "Singletion.h"
#include <QScopedPointer>
#include <mutex>

using namespace std;


struct GameFlowInfo
{
	GameFlowInfo()
	{
		reset();
	}
	
	void reset()
	{
		isStartBloodCapture = false;
		isCoinFlipEnd = false;									// 一开始投币肯定没有结束
		isCloseLight = true;									// 决定是否关闭灯光
		globalLeftWin = false;
		globalRightWin = false;

		isPress1P2P = false;									// 一开始并没有按下1P2P按钮
		isAllowRecv1P2PSignal_ModeMask = false;					// 游戏投币界面是否允许监听信号
		isAllowRecv1P2PSignal_CoinMask = false;					// 投币界面是否允许监听信号

		isSheildX = false;
		isAvatarEnter = true;		
		isGameOver = true;
		CoinFlash = true;
		PlayerNums = -1;

		/* 关于1P单人双人和2P单人双人模式的变量 */
		OneP_Single = -1;
		TwoP_Single = -1;
		DoubleMode = -1;
		CurrGameMode = GameMode::NoneGameMode;
	}

	void resetKeyInfo()
	{
		OneP_Single = -1;
		TwoP_Single = -1;
		DoubleMode = -1;
		CurrGameMode = GameMode::NoneGameMode;
	}

	// 关于战斗流程的变量
	bool isStartBloodCapture;									// 决定是否开始捕获血条
	bool isCoinFlipEnd;											// 记录是否投币结束
	bool isCloseLight;											// 决定是否关闭灯光
	bool globalLeftWin;											// 全局左边是否胜利
	bool globalRightWin;										// 全局右边是否胜利
	bool CoinFlash;												// 决定是否闪烁请投币

	// 串口通信12P按键按下
	bool isPress1P2P;											// 是否按下了1P2P按钮
	bool isAllowRecv1P2PSignal_ModeMask;						// 游戏选择界面监听1P2P信号
	bool isAllowRecv1P2PSignal_CoinMask;						// 投币界面监听1P2P信号

	// 关于游戏流程的变量
	bool isSheildX;												// 是否屏蔽了X按钮
	bool isAvatarEnter;											// 是否是即将进入战斗界面,即已经确认了角色,战斗时为false
	bool isGameOver;											// 是否是游戏结束界面
	int PlayerNums;												// 玩家数量,1表示单人，2表示双人

	/* 关于1P单人双人和2P单人双人模式的变量 */
	int OneP_Single;											// 1P单人模式按下
	int TwoP_Single;											// 2P单人模式按下
	int DoubleMode;												// 1P,2P双人模式按下,不区分
	int CurrGameMode;											// 当前的游戏模式,1P单人(1)，2P单人(2), 双人(3)
};

struct myDateTime
{
	QDateTime m_dateTime;
	bool isRecord;

	myDateTime()
	{
		isRecord = false;
	}
};

class SignalRelay  : public QObject, public Singletion<SignalRelay>
{
	Q_OBJECT

public:
	friend class Singletion<SignalRelay>;

public:
	// 构造函数
	~SignalRelay();

private:
	SignalRelay(QObject* parent = nullptr);
	
public:
	QScopedPointer<GameFlowInfo> m_gameFlowInfo;				// 关于战斗流程的变量

	int settingCoin;											// 记录商家设定的投币数
	
	myDateTime m_beginTime;										// 记录截屏到游戏开始界面的时间
	myDateTime m_endTime;										// 记录截屏到游戏结束界面的时间

	/* 关于奖励机制的变量 */
	RewardState m_rewardState = RewardState::NoRewards;			// 奖励状态 无
	int xuwanCount;												// 支持续玩的局数

	int coin = 0;												// 该变量来记录投的币数,以防串口线程的崩溃带来的记忆币丢失
	int CurrXuWanEnableCount;									// 当前可续玩的局数
};
#endif // SIGNALRELAY_H
