#ifndef CAPTURE_H
#define CAPTURE_H

// 睡觉
#undef ACCESS_MASK
#include <windows.h>
#include <QObject>

// OpenCV 头文件
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// qt头文件
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QImage>
//c++容器库
#include <vector>
#include <map>

// 头文件
// 信号中转
#include"SignalRelay.h"
#include "SignalSend.h"

#include "Interface.h"

using namespace cv;
using namespace std;

class Capture : public Interface
{
    Q_OBJECT
public:
    explicit Capture(QObject* parent = nullptr);

signals:

public slots:
    void OnTimerBlood();                                        // 定时器槽函数
    virtual void InitSelf() override;                           // 初始化函数

public:
    // 血条捕获，裁剪血条并计算血条占比
    void CropBloodCalculateRate();

private:
    // 函数
    int LoadFullBloodImage();                                   // 加载满血血条,并计算捕获到的像素总数
    void LoadResidualBloodImage();                              // 加载残血血条

    bool BloodIsObscured(cv::Mat mask);                         // 判断血条是否被遮挡
    bool AvoidMisJudge(cv::Rect Blood);                         // 避免误判
    void BindSlot();                                            // 绑定槽函数
    bool isSendBlood(int& lastBloodPiexlRate, 
        int& currBloodPiexlRate);                               // 判断是否发送血量信息
    void EndVairableReset(QString name);                        // 结束变量重置

    /* 判断是否游戏结束 */
    void JustFightIsEnd();                                      // 一局游戏结束
    /* 判断左边胜利还是右边胜利 */
    QString IsLeftOrRightIsEnd();                               // 左边或右边结束

    /* 判断当前震动是否都已经完成 */
    bool IsShakeSectionFinish(QMap<int, bool>& ShakeSection);

    /* 计算血条百分比 */
    void CalculateBloodRate(int& LeftBloodRate, int& RightBloodRate);

    /* 判断是否需要震动 */
    void IsShake(int CurrBloodNum, QMap<int, bool>& ShakeSection, bool isLeft);
    /* 发送震动指令 */
    void SendShakeCommand(QMap<int, bool>& ShakeSection, int sectionIndex, bool isLeft);

    /* 初始震动区域 */
    void initShakeSection();

    /* 每一局游戏结束后，清除已振动的标记 */
    void resetShakeSection(QMap<int, bool>& ShakeSection);

    /* 游戏结束时的处理 */
    void GameEndProcess();

public:
    // 变量
    string lastLeftName = "";                                   // 存储人物图像匹配成功的角色，避免重复打印,左边
    string lastRightName = "";                                  // 存储人物图像匹配成功的角色，避免重复打印,右边
    int lastLeftBloodRate = 100;                                // 存储上一帧左边血量百分比
    int lastRightBloodRate = 100;                               // 存储上一帧右边血量百分比
    int checkLoop = 0;                                          // 循环计数器,如果检测三次，发现当前的血条差值是正确的，就更新上一次的血量百分比
    int leftWinnerNum;                                          // 左边胜利的场数,有一方为3，就全部清空
    int rightWinnerNum;                                         // 右边胜利的场数,有一方为3，就全部清空
    bool switchGameEnd;                                         // 记录游戏是否结束，如果本局游戏结束，就设置为true，战斗中就设置为false
    bool IsProcessFightScreenJudge;                             // 判断是否进行战斗场景判断
    bool isSendHp;                                              // 判断是否发送血量信息，当游戏是最后一局的时候，白色和血条会同时使用一个串口会导致串口冲突
    int refuseToSendNums;                                       // 存储拒绝发送血量信息的次数

    bool isLeftBloodNumThanRight;                               // 左边血量是否大于右边血量
    QMap<int, bool> LeftShakeSection;                           // 记录左边震动的区域，key为区域的序号，value为是否已经震动过
    QMap<int, bool> RightShakeSection;                          // 记录右边震动的区域，key为区域的序号，value为是否已经震动过

    bool bIsWhite;                                              // 记录当前是否白屏
    
    QSharedPointer<SignalRelay> m_pSignalRelay;                 // 信号中转类
    QSharedPointer<SignalSend> m_pSignalSend;                   // 信号发射器
};
#endif // CAPTURE_H


// 注解
// 1.串口通信的时候，两次发送数据之间间隔为500ms，避免数据冲突。