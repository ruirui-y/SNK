#include "SerialPort.h"
#include "SignalRelay.h"

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#define ADDR_HP1 12
#define ADDR_HP2 22
#define HP1_MODE 10
#define HP1_VAL 18
#define HP2_VAL 19
#define LED_OPEN_STATE 0
#define TIMEOUT 150
#define QUEUE_MAX_SIZE 256

int SerialPort::m_vs = 12;
int SerialPort::m_charge = 13;                  // 充电灯
int SerialPort::m_leftWin = 16;
int SerialPort::m_rightWin = 17;
int SerialPort::m_shake1 = 30;                  // 震动1
int SerialPort::m_shake2 = 31;                  // 震动2

int SerialPort::WinOpen = 0;
int SerialPort::WinClose = 1;
int SerialPort::VsOpen = 0;
int SerialPort::VsClose = 1;
int SerialPort::ChargeOpen = 0;					// 充电灯打开
int SerialPort::ChargeClose = 1;				// 充电灯关闭
int SerialPort::ShakeOpen = 0;					// 震动打开
int SerialPort::ShakeClose = 1;					// 震动关闭

int SerialPort::CloseLed = 0;
int SerialPort::RedLed = 1;
int SerialPort::GreenLed = 2;
int SerialPort::BlueLed = 3;
int SerialPort::OpenB = 1;
int SerialPort::CloseB = 0;
int SerialPort::OpenYaoGan = 1;
int SerialPort::CloseYaoGan = 0;

uint16_t SerialPort::SendAKey1P = 0x0001;       // 按键 A（1P）- 1
uint16_t SerialPort::SendBKey1P = 0x0002;       // 按键 B（1P）- 2
uint16_t SerialPort::SendUpKey1P = 0x0080;      // 按键 UP（1P）- 128
uint16_t SerialPort::SendDownKey1P = 0x0010;    // 按键 DOWN（1P）- 16
uint16_t SerialPort::SendStartKey1P = 0x0020;   // 按键 START（1P）- 32
uint16_t SerialPort::SendSelectKey1P = 0x0040;  // 按键 SELECT（1P）- 64
uint16_t SerialPort::SendAKey2P = 0x0100;       // 按键 A（2P）- 256
uint16_t SerialPort::SendBKey2P = 0x0200;       // 按键 B（2P）- 512
uint16_t SerialPort::SendDownKey2P = 0x1000;    // 按键 DOWN（2P）- 4096
uint16_t SerialPort::SendUpKey2P = 0x8000;      // 按键 UP（2P）- 32768
uint16_t SerialPort::SendStartKey2P = 0x2000;   // 按键 START（2P）- 8192
uint16_t SerialPort::SendSelectKey2P = 0x4000;  // 按键 SELECT（2P）- 16384

static const uint8_t _roger_table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };

/** Table of CRC values for low-order byte */
static const uint8_t _roger_table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };


SerialPort::SerialPort()
    :Interface(nullptr)
{

}

SerialPort::~SerialPort()
{
}

void SerialPort::GetAllSerialPort()
{
    // 列出所有可用串口
    QList<QSerialPortInfo> serialPortList = QSerialPortInfo::availablePorts();
    if (serialPortList.isEmpty())
    {
        qDebug() << QString::fromLocal8Bit("Serial::GetAllSerialPort(): 没有找到串口");
        return;
    }

    qDebug() << QString::fromLocal8Bit("Serial::GetAllSerialPort(): 打印所有串口");
    for (auto& info : serialPortList)
    {
        qDebug() << "Serial::GetAllSerialPort(): PortName: " << info.portName();
        qDebug() << "Serial::GetAllSerialPort(): Description" << info.description();
        qDebug() << "Serial::GetAllSerialPort(): Manufacturer" << info.manufacturer();
    }

    // 选择第一个串口并打开
    serial->setPortName("COM1");
    serial->setBaudRate(9600);
    serial->setDataBits((QSerialPort::DataBits)8);
    serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial->setParity(QSerialPort::NoParity);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << QString::fromLocal8Bit("成功打开串口") << serial->portName();
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("无法打开串口") << serial->errorString();
    }
}

// 应对串口失效
void SerialPort::clear_serial_port()
{   
    serial->close();
    serial = nullptr;
    init_serial("COM1");
}

bool SerialPort::isQueueEmpty()
{
    if (m_queue.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SerialPort::InitSelf()
{
    Init();
}

// 初始化串口
void SerialPort::init_serial(QString portName)
{
    serial = QSharedPointer<QSerialPort>(new QSerialPort);

    // 选择第一个串口并打开
    serial->setPortName(portName);
    serial->setBaudRate(9600);
    serial->setDataBits((QSerialPort::DataBits)8);
    serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial->setParity(QSerialPort::NoParity);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << QString::fromLocal8Bit("成功打开串口") << serial->portName();
        qDebug() << QString::fromLocal8Bit("串口初始化成功 当前线程ID：") << QThread::currentThreadId();
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("无法打开串口") << serial->errorString();
    }
}

// 发送灯控指令
void SerialPort::write_single_register(uint16_t address, uint16_t value)
{
    uint8_t data[11] = { 0 };

    data[0] = 0x01;                                                                                                     // 从机地址
    data[1] = 0x10;                                                                                                     // 功能码
    data[2] = address >> 8;
    data[3] = address & 0xFF;                                                                                           // 地址
    data[4] = 0;
    data[5] = 1;                                                                                                        // 地址长度

    data[6] = 2;                                                                                                        // 数据长度
    data[7] = value >> 8;
    data[8] = value & 0xFF;                                                                                             // 数据

    uint16_t crc = roger_modbus_crc(data, 9);                                                                           // 校验

    data[9] = crc >> 8;
    data[10] = crc & 0xFF;

    // print data
    // 将 uint8_t 数组转换为 QByteArray
    QByteArray byteArray(reinterpret_cast<char*>(data), sizeof(data));
    qDebug() << "Serial::SEND data:" << byteArray.toHex();

    // 打印发送的数据
    Task task;
    task.taskName = "singleRegister";
    task.dataArray = byteArray;

    execute_task(task);
}

// 轮询监听投币和1p2p开关
void SerialPort::refresh_io_status(QByteArray buf)
{
    if (buf.isEmpty()) {
        qDebug() << "Read timeout or no data received!";
        return;
    }

    // 校验长度
    if (buf.size() != 11) {
        qDebug() << "Read data error!";
        return;
    }

    // 校验CRC
    // 打印发送的数据
    //qDebug() << "Serial::RECV data:" << buf.toHex();

    uint8_t* pArr = (uint8_t*)buf.data();

    uint16_t crc = roger_modbus_crc(pArr, 9) & 0xFFFF; // 校验
    uint16_t crc_recv = (pArr[9] << 8) | pArr[10];
    if (crc_recv != crc) {
        qDebug() << "CRC error!" << crc_recv << " " << crc;
        return;
    }

    // 解析数据
    // 数据模板：[开关] [投币1] [投币2]
    // 01 03 06 [00 00] [00 00] [00 00] 21 75
    uint8_t* data = (uint8_t*)buf.data();

    // 开关
    uint16_t switch_value = (data[3] << 8) | data[4];

    // 解析数据 
    // 0001 1P单人按下
    // 0010 1P双人按下
    // 0100 2P单人按下
    // 1000 2P双人按下

    // 游戏模式选择界面的按键监听
    if (SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask)
    {
        SignalRelay::GetInstance()->m_gameFlowInfo->resetKeyInfo();
        /* 1P位单人游戏按下 跳转到玩家对电脑 */
        if (switch_value & 0x01)
        {
            emit m_SignalSend->OnePKeyPress_Single();                                                                                   // 发送1p单人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 1;
            SignalRelay::GetInstance()->m_gameFlowInfo->OneP_Single = 1;                                                                // 1P单人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask = false;
            qDebug() << "OnePKey  SingeleMode Pressed ModeMask";
        }

        /* 1P位双人游戏按下 跳转到玩家对玩家 */
        if (switch_value & 0x02)
        {
            emit m_SignalSend->DoubleModeKeyPress();                                                                                    // 发送1P2P双人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 2;
            SignalRelay::GetInstance()->m_gameFlowInfo->DoubleMode = 1;                                                                 // 1P2P双人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask = false;
            qDebug() << "OnePKey  DoubleMode Pressed ModeMask";
        }

        /* 2P位单人游戏按下 跳转到电脑对玩家 */
        if (switch_value & 0x04)
        {
            emit m_SignalSend->TwoPKeyPress_Single();                                                                                   // 发送2p单人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 1;
            SignalRelay::GetInstance()->m_gameFlowInfo->TwoP_Single = 1;                                                                // 2P单人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask = false;
            qDebug() << "TwoPKey  SingeleMode Pressed ModeMask";
        }

        /* 2P位双人游戏按下 跳转到玩家对玩家 */
        if (switch_value & 0x08)
        {
            emit m_SignalSend->DoubleModeKeyPress();                                                                                    // 发送1P2P双人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 2;
            SignalRelay::GetInstance()->m_gameFlowInfo->DoubleMode = 1;                                                                 // 1P2P双人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_ModeMask = false;
            qDebug() << "TwoPKey  DoubleMode Pressed ModeMask";
        }
    }

    // 投币界面的按键监听
    if (SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_CoinMask)
    {
        /* 1P位单人游戏按下 跳转到玩家对电脑 */
        if (switch_value & 0x01)
        {
            SignalRelay::GetInstance()->m_gameFlowInfo->resetKeyInfo();
            emit m_SignalSend->OnePKeyPress_Single_CoinMask();                                                                          // 发送1p单人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 1;
            SignalRelay::GetInstance()->m_gameFlowInfo->OneP_Single = 1;                                                                // 1P单人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_CoinMask = false;
            qDebug() << "OnePKey  SingeleMode Pressed CoinMask";
        }

        /* 2P位单人游戏按下 跳转到电脑对玩家 */
        if (switch_value & 0x04)
        {
            SignalRelay::GetInstance()->m_gameFlowInfo->resetKeyInfo();
            emit m_SignalSend->TwoPKeyPress_Single_CoinMask();                                                                          // 发送2p单人模式按下信号
            SignalRelay::GetInstance()->m_gameFlowInfo->PlayerNums = 1;
            SignalRelay::GetInstance()->m_gameFlowInfo->TwoP_Single = 1;                                                                // 2P单人按下
            SignalRelay::GetInstance()->m_gameFlowInfo->isAllowRecv1P2PSignal_CoinMask = false;
            qDebug() << "TwoPKey  SingeleMode Pressed CoinMask";
        }
    }

    // 投币1
    oneCoin = (data[5] << 8) | data[6];
    // 投币2
    twoCoin = (data[7] << 8) | data[8];
    static bool isFirst = true;                                                                                         // 第一次读取数据，清空数据
    if (isFirst) 
    {
        oneCoin = 0;
        twoCoin = 0;
        isFirst = false;
        qDebug() << QString::fromLocal8Bit("第一次读币，将数据清空");
    }

    // 打印数据
    qDebug() << "oneCoin:" << oneCoin << " twoCoin:" << twoCoin;
}

// 通用控制接口
QByteArray SerialPort::write_registers(uint16_t addr, uint16_t len, uint8_t* data)
{
    uint8_t send_data[100] = { 0 };

    send_data[0] = 0x01;                                                                            // 从机地址
    send_data[1] = 0x10;                                                                            // 功能码
    send_data[2] = addr >> 8;
    send_data[3] = addr & 0xFF;                                                                     // 地址
    send_data[4] = 0;
    send_data[5] = len;                                                                             // 地址长度
    send_data[6] = len * 2;                                                                         // 数据长度

    // 组装数据段
    for (int i = 0; i < len *2; i++) {
        send_data[7 + i] = data[i];
    }

    // 校验
    uint16_t crc = roger_modbus_crc(send_data, 7 + len * 2);                                        // 校验

    send_data[7 + len * 2] = crc >> 8;
    send_data[8 + len * 2] = crc & 0xFF;

    // print data
    // 将 uint8_t 数组转换为 QByteArray
    QByteArray byteArray(reinterpret_cast<char*>(send_data), 9 + len * 2);

    // 打印发送的数据
    return byteArray;
}

// 控制两个Win灯
void SerialPort::write_win_led(uint16_t leftWin, uint16_t rightWin)
{
    uint8_t data[4] = { 0 };

    data[0] = leftWin >> 8;
    data[1] = leftWin & 0xFF;                                                                       // 左边灯亮度
    data[2] = rightWin >> 8;
    data[3] = rightWin & 0xFF;                                                                      // 右边灯亮度

    QByteArray byteArray = write_registers(MB_REG_LED_WIN1, 2, data);

    // 01 10 00 10 00 01 00 0c
    // 校验长度
    Task task;
    task.taskName = "winLed";
    task.dataArray = byteArray;
    execute_task(task);
}

// 控制两个Win灯和两个HP灯
void SerialPort::write_win_hp_led(uint16_t leftWin, uint16_t rightWin, uint16_t hp1, uint16_t hp2)
{
    uint8_t data[8] = { 0 };
    data[0] = leftWin >> 8;
    data[1] = leftWin & 0xFF;                                                                       // 左边灯亮度
    data[2] = rightWin >> 8;
    data[3] = rightWin & 0xFF;                                                                      // 右边灯亮度
    data[4] = hp1 >> 8;
    data[5] = hp1 & 0xFF;                                                                           // 左边灯亮度
    data[6] = hp2 >> 8;
    data[7] = hp2 & 0xFF;                                                                           // 右边灯亮度

    QByteArray byteArray = write_registers(MB_REG_LED_WIN2, 4, data);
    Task task;
    task.taskName = "winHpLed";
    task.dataArray = byteArray;
    execute_task(task);
}

// 控制两个氛围灯  // 0灭 1 红色 2 绿色 3 蓝色
void SerialPort::write_color_led(uint16_t left_color, uint16_t right_color)
{
    uint8_t data[4] = { 0 };

    data[0] = left_color >> 8;
    data[1] = left_color & 0xFF; // 左边灯颜色
    data[2] = right_color >> 8;
    data[3] = right_color & 0xFF; // 右边灯颜色

    QByteArray byteArray = write_registers(MB_REG_LED_COLOR_LEFT, 2, data);

    // 01 10 00 0E 00 01 60 0A
    Task task;
    task.taskName = "colorLed";
    task.dataArray = byteArray;
    execute_task(task);
}

// 屏蔽1B和2B
void SerialPort::write_1b_2b(uint16_t one_b, uint16_t two_b)
{
    uint8_t data[4] = { 0 };
    data[0] = one_b >> 8;
    data[1] = one_b & 0xFF; // 屏蔽1B
    data[2] = two_b >> 8;
    data[3] = two_b & 0xFF; // 屏蔽2B

    QByteArray byteArray = write_registers(MB_REG_1B, 2, data);
    Task task;
    task.taskName = "1b2b";
    task.dataArray = byteArray;
    execute_task(task);
}

// 屏蔽摇杆的信号
void SerialPort::write_joystick(uint16_t one, uint16_t two)
{
    uint8_t data[4] = { 0 };
    data[0] = one >> 8;
    data[1] = one & 0xFF; // 屏蔽1B
    data[2] = two >> 8;
    data[3] = two & 0xFF; // 屏蔽2B

    QByteArray byteArray = write_registers(MB_REG_YAOGAN1, 2, data);
    Task task;
    task.taskName = "joystick";
    task.dataArray = byteArray;
    execute_task(task);
}

// 改变HP1的颜色
void SerialPort::write_One_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue)
{
    uint8_t data[6] = { 0 };
    data[0] = Red >> 8;
    data[1] = Red & 0xFF; // 红色值
    data[2] = Green >> 8;
    data[3] = Green & 0xFF; // 绿色值
    data[4] = Blue >> 8;
    data[5] = Blue & 0xFF; // 蓝色值

    QByteArray byteArray = write_registers(MB_REG_ONE_HP1_COLOR, 3, data);
    Task task;
    task.taskName = "oneHpColor";
    task.dataArray = byteArray;
    execute_task(task);
}

// 改变HP2的颜色
void SerialPort::write_Two_hp_color(uint16_t Red, uint16_t Green, uint16_t Blue)
{
    uint8_t data[6] = { 0 };
    data[0] = Red >> 8;
    data[1] = Red & 0xFF; // 红色值
    data[2] = Green >> 8;
    data[3] = Green & 0xFF; // 绿色值
    data[4] = Blue >> 8;
    data[5] = Blue & 0xFF; // 蓝色值

    QByteArray byteArray = write_registers(MB_REG_TWO_HP1_COLOR, 3, data);
    Task task;
    task.taskName = "twoHpColor";
    task.dataArray = byteArray;
    execute_task(task);
}

// 写入数据
void SerialPort::write_hp(uint16_t hp1_value, uint16_t hp2_value)
{
    uint8_t data[20] = { 0 };

    data[0] = 0x01; // 从机地址
    data[1] = 0x10; // 功能码
    data[2] = HP1_VAL >> 8;
    data[3] = HP1_VAL & 0xFF; // 地址
    data[4] = 0;
    data[5] = 2; // 地址长度
    data[6] = 4; // 数据长度

    // HP1
    data[7] = hp1_value >> 8;
    data[8] = hp1_value & 0xFF;

    // HP2
    data[9] = hp2_value >> 8;
    data[10] = hp2_value & 0xFF;

    uint16_t crc = roger_modbus_crc(data, 11); // 校验

    data[11] = crc >> 8;
    data[12] = crc & 0xFF;

    // print data
    // 将 uint8_t 数组转换为 QByteArray
    QByteArray byteArray(reinterpret_cast<char*>(data), 13);

    // 打印发送的数据
    // qDebug() << "Serial::write_hp: send data:" << byteArray.toHex();
    Task task;
    task.taskName = "hp";
    task.dataArray = byteArray;
    execute_task(task);
}

// 0.2s一条信息
// 读取投币信号
void SerialPort::read_registers(uint16_t addr, uint16_t len)
{
    uint8_t data[9] = { 0 };

    data[0] = 01; // 从机地址
    data[1] = 03; // 功能码

    data[2] = addr >> 8;
    data[3] = addr & 0xFF; // 地址

    data[4] = len >> 8;
    data[5] = len & 0xFF; // 地址长度

    uint16_t crc = roger_modbus_crc(data, 6); // 校验

    data[6] = crc >> 8;
    data[7] = crc & 0xFF;

    // todo send data
    // print data
    // 将 uint8_t 数组转换为 QByteArray

    QByteArray byteArray(reinterpret_cast<char*>(data), 8);
    // 打印发送的数据
    /*qDebug() << "Serial::write_single_register: send data:" << byteArray.toHex();*/
    Task task;
    task.taskName = "ReadCoin";
    task.dataArray = byteArray;
    execute_task(task);
}

// 执行任务
void SerialPort::execute_task(Task task)
{
    int timeout = 150;
    int sizeLen = 8;

    if (serial->isOpen())
    {
        // 写入数据
        serial->write(task.dataArray);
        QByteArray receiveData;
        qint64 startTime = QDateTime::currentMSecsSinceEpoch();
        if (task.taskName == "joystick")
        {
            timeout = 300;
        }
        // 修改等待逻辑，增加超时退出
        while (QDateTime::currentMSecsSinceEpoch() - startTime < timeout)
        {
            if (serial->waitForReadyRead(50))
            {
                receiveData.append(serial->readAll());
            }
        }
        // 投币的数据校验
        if (task.taskName == "ReadCoin")
        {
            qDebug() << QString::fromLocal8Bit("执行串口读币任务");
            refresh_io_status(receiveData);
            SignalRelay::GetInstance()->coin += oneCoin;
            SignalRelay::GetInstance()->coin += twoCoin;
            return;
        }
        // 不是投币的数据校验
        else if (receiveData.size() != sizeLen)
        {
            qDebug() << "Receive Bytedata.size() = " << receiveData.size();
            qDebug() << "Receive Bytedate = " << receiveData.toHex();
            qDebug() << task.taskName << "Write data error!";
            return;
        }
        //qDebug() << receiveData.toHex();
        qDebug() << "Serial::" << task.taskName << "Write data success!";
    }
}

void SerialPort::Init()
{
    init_serial("COM1");
    m_SignalSend = SignalSend::GetInstance();
    BindSlot();
}

uint16_t SerialPort::roger_modbus_crc(uint8_t* buffer, uint16_t buffer_length)
{
    uint8_t      crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t      crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i;             /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ _roger_table_crc_hi[i];
        crc_lo = _roger_table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

// 绑定槽函数
void SerialPort::BindSlot()
{
    connect(this, &SerialPort::SerialPortError, m_SignalSend.get(), &SignalSend::SerialPortError);

    connect(m_SignalSend.get(), &SignalSend::write_win_led, this, &SerialPort::write_win_led, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_win_hp_led, this, &SerialPort::write_win_hp_led, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_color_led, this, &SerialPort::write_color_led, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_1b_2b, this, &SerialPort::write_1b_2b, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_joystick, this, &SerialPort::write_joystick,Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_One_hp_color, this, &SerialPort::write_One_hp_color, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_Two_hp_color, this, &SerialPort::write_Two_hp_color, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_hp, this, &SerialPort::write_hp, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::read_registers, this, &SerialPort::read_registers, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_single_register, this, &SerialPort::write_single_register, Qt::QueuedConnection);
    connect(m_SignalSend.get(), &SignalSend::write_joystick_1b2b, this, &SerialPort::write_j1_j2_1b_2b, Qt::QueuedConnection);
}

bool SerialPort::errorCheck(int times, int sizeLen)
{
    // 加锁和变量，防止两个线程同时接受数据
    QMutexLocker locker(&m_mutex);
    QByteArray receiveData;
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    while (QDateTime::currentMSecsSinceEpoch() - startTime < times)
    {
        // 等待串口数据到达
        if (serial->waitForReadyRead(50))
        {
            receiveData = serial->readAll();
        }
    }

    if (receiveData.size() != sizeLen)
    {
        qDebug() << "bytedata.size() = " << receiveData.size();
        qDebug() << "bytedate = " << receiveData.toHex();
        qDebug() << "Write data error!";
        // 清理串口
        clear_serial_port();
        return false;
    }
    return true;
}

void SerialPort::writeMutex(QByteArray data)
{
    QMutexLocker locker(&m_mutex);
    if (serial->isOpen())
    {
        serial->write(data);
    }
}

bool SerialPort::writeAndRead(QByteArray data, int timeout, int sizeLen)
{
    QMutexLocker locker(&m_mutex);
    if (!serial->isOpen() || !serial)
    {
        qDebug() << "Serial port is not open!";
        return false;
    }

    if (serial->isOpen())
    {
        serial->write(data);

        QByteArray receiveData;
        qint64 startTime = QDateTime::currentMSecsSinceEpoch();
        while (QDateTime::currentMSecsSinceEpoch() - startTime < timeout)
        {
            // 等待串口数据到达
            if (serial->waitForReadyRead(50))
            {
                receiveData = serial->readAll();
            }
        }

        if (receiveData.size() != sizeLen)
        {
            qDebug() << "bytedata.size() = " << receiveData.size();
            qDebug() << "bytedate = " << receiveData.toHex();
            qDebug() << "Write data error!";
            return false;
        }
        return true;
    }
}

void SerialPort::AddHp()
{
    if (hp1 < 100 && hp2 < 100)
    {
        hp1 += 10;
        hp2 += 10;
        qDebug() << "hp1 = " << hp1;
        qDebug() << "hp2 = " << hp2;
    }
    qDebug() << "hp1 = " << hp1;
    qDebug() << "hp2 = " << hp2;


    static int i = 0;

    if (i % 2) {

        //write_single_register(7, 1); // VS开
        //write_single_register(9, 1); // 左灯开
        write_single_register(8, 1); // 右灯开
    }
    else {
        //write_single_register(7, 0); // VS关
        //write_single_register(9, 0); // 左灯关
        write_single_register(8, 0); // 右灯关
    }
    i++;
}

// !在这里添加地址信息
enum hold_reg_addr
{
    ADDR_LED_VS = 12, // VS灯地址
    ADDR_LED_CHARGE = 13, // 充电灯地址
    ADDR_LED1_COLOR = 14, // 氛围灯LED1颜色地址
    ADDR_LED2_COLOR = 15, // 氛围灯LED2颜色地址
    // ... 这里添加其他需要控制的地址
};

enum led_color
{
    LED_COLOR_RED = 1,
    LED_COLOR_GREEN = 2,
    LED_COLOR_BLUE = 3,
};

//write_single_register(ADDR_LED_VS, LED_OPEN_STATE); // 打开VS灯
// write_single_register(ADDR_LED_CHARGE, LED_OPEN_STATE); // 打开充电灯
// write_single_register(ADDR_LED1_COLOR, LED_COLOR_RED); // 设置LED1颜色为红色
// write_single_register(ADDR_LED2_COLOR, LED_COLOR_BLUE); // 设置LED2颜色为蓝色


void SerialPort::write_j1_j2_1b_2b(uint16_t j1, uint16_t j2, uint16_t one_b, uint16_t two_b)
{
    uint8_t data[8] = { 0 };
    data[0] = j1 >> 8;
    data[1] = j1 & 0xFF;// 屏蔽摇杆1
    data[2] = j2 >> 8;
    data[3] = j2 & 0xFF;// 屏蔽摇杆2
    data[4] = one_b >> 8;
    data[5] = one_b & 0xFF;// 屏蔽1B
    data[6] = two_b >> 8;
    data[7] = two_b & 0xFF;// 屏蔽2B
    QByteArray byteArray = write_registers(MB_REG_YAOGAN1, 4, data);
    Task task;
    task.taskName = "j1j2_1b2b";
    task.dataArray = byteArray;
    execute_task(task);
}