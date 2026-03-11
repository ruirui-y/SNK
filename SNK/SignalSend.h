#ifndef SIGNALSEND_H
#define SIGNALSEND_H

#include <QObject>
#include <QString>
#include <functional>
#include "Singletion.h"

using Task = std::function<void()>;

// 创建对象专门绑定信号槽
class SignalSend  : public QObject, public Singletion<SignalSend>
{
	Q_OBJECT

public:
	friend class Singletion<SignalSend>;

public:
	~SignalSend();

private:
	SignalSend(QObject* parent = nullptr);

// 发送信号
signals:
	void gameBegin();						// 游戏开始
	void mainMenu();						// 主菜单
	void selectedModeOne();					// 选择模式1
	void selectedModeTwo();					// 选择模式2 玩家对玩家
	void selectedModeTwo_PlayerToPC();		// 选择模式2，玩家对电脑
	void selectedModeTwo_PCToPlayer();		// 选择模式2，电脑对玩家

	void EnterButton();						// 发送1P
	void EnterButton2();					// 发送2P
	void StartSelectPlayerCheck();			// 避免选择玩家界面，手柄无法及时打开而导致游戏卡死，开启定时器，实时发送手柄打开信号,直到上一帧图片和当前图片不一致

	void ShieldXButton();					// 屏蔽按键X
	void wuchubujiu();						// 误触补救
	void AvatarEnter();						// 角色确认
	void FirstGameEnd();					// 三局游戏结束，依据白屏来判断，此时屏蔽游戏按键
	void gameEndSuccess();					// 游戏结束，左边胜利
	void gameEndFailed();					// 游戏结束，右边胜利
	void EnterSuccess();					// enter按键发送成功
	
	/* 关于1P单人双人和2P单人双人模式的信号 */
	void OnePKeyPress_Single();				// 1p单人模式按下
	void TwoPKeyPress_Single();				// 2p单人模式按下
	void DoubleModeKeyPress();				// 1P2P双人模式按下

	void OnePKeyPress_Single_CoinMask();	// 1p单人模式按下 投币遮罩
	void TwoPKeyPress_Single_CoinMask();	// 2p单人模式按下 投币遮罩

	/* 关于 开启或关闭投币轮询的信号*/
	void StartCoinPolling();				// 开始投币轮询
	void StopCoinPolling();					// 停止投币轮询
	void toubiEnd(bool isSingleMode);		// 投币结束,是否是单人模式

	void OpenSwitchMode();					// 打开全局截屏
	void CloseSwitchMode();					// 关闭全局截屏
	void StartPlayMoive();					// 开始播放动画
	void SecondScreenShowText();			// 副屏显示文本
	void leftWin();							// 左边赢了
	void rightWin();						// 右边赢了
	void endAvatrSuccess(int index);		// 最后胜利的角色的索引，用来播放胜利动画
	void endMoive();						// 结束动画播放完毕
	void SerialPortError(int totalCoin);	// 串口通信错误，totalCoin为当前拥有的总金币
	void stopStandbyAnimation();			// 停止待机动画
	void stopCoinFlash();                   // 停止投币闪烁
	
	/* 一局游戏战斗结束 */
	void FightEnd();
	void StartCountDownCrop();				// 开始倒计时裁剪
	void StopCountDownCrop();				// 停止倒计时裁剪

	/* 最上方的小屏幕待机动画 */
	void startMinScreenStandAnimation();	// 开始最上方的屏幕待机动画
	void stopMinScreenStandAnimation();		// 停止最上方的屏幕待机动画

	/* 全局白屏信号 */
	void WhiteScreen();

	/* 窗口显示 / 隐藏 */
	void ShowMaskWidget(bool);				// 显示遮罩层
	void HideMaskWidget();					// 隐藏遮罩层

	/* 添加任务到线程池的任务队列 */
	void AddTaskToThreadPool(Task);

	//*************************************   串口信号绑定 *********************************************//

	// 将串口单独封装到一个线程中，触发方式为信号，其他的线程只需要绑定信号，其余的全部交给串口线程处理
	void write_win_led(uint16_t leftWin, uint16_t rightWin);								// WIN灯
	void write_win_hp_led(uint16_t leftWin, uint16_t rightWin, uint16_t hp1, uint16_t hp2); // 血条灯+WIN灯
	void write_color_led(uint16_t left_color, uint16_t right_color);						// 氛围灯 addr:14~15
	void write_1b_2b(uint16_t one_b, uint16_t two_b);										// 1B2B灯 addr:8~9
	void write_joystick(uint16_t one, uint16_t two);										// 摇杆 addr:6~7
	void write_One_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue);					// 控制血条的颜色
	void write_Two_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue);					// 控制血条的颜色
	void refresh_io_status(QByteArray buf);													// 刷新状态: 开关+投币，刷新完成后读取状态
	void write_hp(uint16_t hp1_value, uint16_t hp2_value);									// 血条 addr:0~1
	void read_registers(uint16_t addr, uint16_t len);
	void write_single_register(uint16_t address, uint16_t value);
	void write_joystick_1b2b(uint16_t j1, uint16_t j2, uint16_t one_b, uint16_t two_b);		// 打开手柄并屏蔽1b2b
};
#endif // SIGNALSEND_H