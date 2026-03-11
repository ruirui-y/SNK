#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include "Interface.h"
#include "SignalSend.h"

class SerialPort  : public Interface
{
	Q_OBJECT

public:
	SerialPort();// 不创建父类对象
	~SerialPort();
	void clear_serial_port();// 如果串口失效了，做的应对处理
	bool isQueueEmpty();

	// 用于实现串口的互斥访问所需的变量
	struct Task {
		QString taskName;
		QByteArray dataArray;
	};

	QQueue<Task> m_queue;
	QMutex m_mutex;
	QWaitCondition queueNotEmpty;		// 队列不空
	QWaitCondition queueNotFull;		// 队列不满

public slots:
	void AddHp();
	virtual void InitSelf() override;

private:
	// 函数
	void GetAllSerialPort();						
	QByteArray write_registers(uint16_t addr, uint16_t len, uint8_t* data);
	static uint16_t roger_modbus_crc(uint8_t* buffer, uint16_t buffer_length);
	void BindSlot();

	bool errorCheck(int times, int sizeLen);								// 错误检查
	void writeMutex(QByteArray data);										// 为写操作加锁
	// 统一的收发接口
	bool writeAndRead(QByteArray data,int timeout, int sizeLen);


signals:
	// 当1p按下时发送信号
	void OnePKeyPressed();
	// 当2p按下时发送信号
	void TwoPKeyPressed();
	// 串口失效
	void SerialPortError(int totalCoin);

public:
	enum mb_register {
		MB_REG_KEY = 2,													// 按键
		MB_REG_SWITCH = 3,												// 开关
		MB_REG_COIN1 = 4,												// 投币1
		MB_REG_COIN2 = 5,												// 投币2
		MB_REG_YAOGAN1 = 6,												// 摇杆1
		MB_REG_YAOGAN2 = 7,												// 摇杆2
		MB_REG_1B = 8,													// 1B
		MB_REG_2B = 9,													// 2B

		// 氛围灯
		MB_REG_LED_COLOR_LEFT = 14,										// 左侧颜色灯
		MB_REG_LED_COLOR_RIGHT = 15,									// 右侧颜色灯

		MB_REG_LED_WIN1 = 16,											// 左WIN灯
		MB_REG_LED_WIN2 = 17,											// 右WIN灯

		MB_REG_ONE_HP1_COLOR = 23,										// 1号血条颜色
		MB_REG_TWO_HP1_COLOR = 27										// 2号血条颜色
	};

	// 变量
	QByteArray qdata;
	int hp1 = 0;
	int hp2 = 0;

	QSharedPointer<QSerialPort> serial;									// 串口对象

	static int m_vs;
	static int m_charge;												// 充电
	static int m_leftWin;
	static int m_rightWin;
	static int m_shake1;												// 震动1
	static int m_shake2;												// 震动2

	static int WinOpen;
	static int WinClose;
	static int VsOpen;
	static int VsClose;
	static int ChargeOpen;												// 充电灯打开
	static int ChargeClose;												// 充电灯关闭
	static int ShakeOpen;												// 震动打开
	static int ShakeClose;												// 震动关闭
	static int CloseLed;
	static int RedLed;
	static int GreenLed;
	static int BlueLed;
	static int OpenB;
	static int CloseB;
	static int OpenYaoGan;
	static int CloseYaoGan;

	// 按键类型
	static uint16_t SendAKey1P;
	static uint16_t SendBKey1P;
	static uint16_t SendUpKey1P;
	static uint16_t SendDownKey1P;
	static uint16_t SendStartKey1P;
	static uint16_t SendSelectKey1P;
	static uint16_t SendAKey2P;
	static uint16_t SendBKey2P;
	static uint16_t SendUpKey2P;
	static uint16_t SendDownKey2P;
	static uint16_t SendStartKey2P;
	static uint16_t SendSelectKey2P;

	quint16 oneCoin; // 1号投币口
	quint16 twoCoin; // 2号投币口

	// 信号指针，用来绑定整个代码的信号槽
	QSharedPointer<SignalSend> m_SignalSend;												// 信号发射器

	// 函数
	void init_serial(QString portName);
	void execute_task(Task task);															// 执行任务
	void Init();																			// 初始化

public slots:
	void write_single_register(uint16_t address, uint16_t value);
	void write_hp(uint16_t hp1_value, uint16_t hp2_value);
	void read_registers(uint16_t addr, uint16_t len);

	// write_single_register(MB_REG_KEY, KEY_TYPE_1P_A | KEY_TYPE_1P_B);
	// todo
	void write_win_led(uint16_t leftWin, uint16_t rightWin);								// WIN灯
	void write_win_hp_led(uint16_t leftWin, uint16_t rightWin, uint16_t hp1, uint16_t hp2); // 血条灯+WIN灯
	void write_color_led(uint16_t left_color, uint16_t right_color);						// 氛围灯 addr:14~15
	void write_1b_2b(uint16_t one_b, uint16_t two_b);										// 1B2B灯 addr:8~9
	void write_joystick(uint16_t one, uint16_t two);										// 摇杆 addr:6~7
	void write_One_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue);					// 控制血条的颜色
	void write_Two_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue);					// 控制血条的颜色
	void refresh_io_status(QByteArray buf);													// 刷新状态: 开关+投币，刷新完成后读取状态	
	void write_j1_j2_1b_2b(uint16_t j1, uint16_t j2, uint16_t one_b, uint16_t two_b);		// 控制摇杆和1b2b

};
#endif // SERIALPORT_H