#ifndef ENUM_H
#define ENUM_H

/* 结束菜单的按钮类型  单人模式和双人模式的按钮类型 */
enum EndMenuButtonType : int
{
	None = -1,
	SingleAgainFight = 1,
	SingleSelectSequence,
	SingleSelectMember,
	SingleFightSet,
	SingleMainMenu,

	DoubleAgainFight = 10,
	DoubleSelectSequence,
	DoubleSelectMember,
	DoubleFightSet,
	DoubleMainMenu,

	GameModeDouble = 20,
	GameModeSingle,
};

/* 灰色动画 左边还是右边 */
enum GrayLabelType : int
{
	NoneGrayLabel = -1,
	LeftGrayLabel = 1,
	RightGrayLabel,
};

/* 游戏模式 */
enum GameMode : int
{
	NoneGameMode = -1,
	PlayerToPCMode = 1,																			// 玩家对电脑
	PCToPlayerMode = 2,																			// 电脑对玩家
	PlayerToPlayerMode = 3,																		// 玩家对玩家
};

/* 奖励状态 */
enum RewardState : int
{
	NoneRewardState = -1,
	NoRewards = 1,																				// 无奖励
	HasRewards = 2,																				// 有奖励
};

/* 游戏胜利状态 */
enum GameWinType : int
{
	NoneGameWinType = -1,
	LeftPlayerWin_Single = 1,																	// 左边玩家胜利_单人
	LeftPlayerWin_Double = 2,																	// 左边玩家胜利_双人
	RightPlayerWin_Single = 3,																	// 右边玩家胜利_单人
	RightPlayerWin_Double = 4,																	// 右边玩家胜利_双人
	PlayerFalied = 5,																			// 玩家失败
};

/* 按键类型 */
enum KeyType : int
{
	ESC = 1,
	UP,
	DOWN,
	ENTER,
};

#endif
