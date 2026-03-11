#include "capture.h"
#include <QDebug>

// 定时器
#include <QTimer>

// 静态方法
#include "CaptureStatic.h"

#define MAX_MIS_JUDGE_TIME 4                                                    // 最大重复检测次数
#define HealthBarPercent 100                                                    // 血条百分比
#define Min_Difference 5                                                        // 最小血量差值
#define Max_Difference 15                                                       // 最大血量差值
#define Min_RefuseToSendNums 3                                                  // 最小拒绝发送次数


Capture::Capture(QObject* parent) : Interface(parent)
{
    switchGameEnd = false;                                                      // 记录游戏是否结束，如果本局游戏结束，就设置为true，战斗中就设置为false
    IsProcessFightScreenJudge = true;                                           // 判断是否进行战斗场景判断
    isSendHp = true;

    // 初始化中转变量
    m_pSignalRelay = SignalRelay::GetInstance();
    m_pSignalSend = SignalSend::GetInstance();

    bIsWhite = false;                                                            // 不是白屏

    initShakeSection();                                                          // 初始化震动区间 
    BindSlot();
}

// 加载满血血条,并计算捕获到的像素总数
int Capture::LoadFullBloodImage()
{
    string BloodPath =  ".\\ResourceFiles\\fullBlood.png";
    // 读取图像文件
    cv::Mat bloodImage = cv::imread(BloodPath, cv::IMREAD_COLOR);

    // 检查图像是否成功加载
    if (bloodImage.empty()) {
        qDebug() << "capture::LoadFullBloodImage 无法加载图像: " << QString::fromStdString(BloodPath);
    }
    else {
        qDebug() << "capture::LoadFullBloodImage 成功加载图像: " << QString::fromStdString(BloodPath);
        // 在此处可以继续处理加载的图像
    }

    cv::imshow("image2", bloodImage);

    // 定义绿色像素的上下限
    //cv::Scalar lowerBound(166, 222, 33);  // BGR 低限
    cv::Scalar lowerBound(65, 100, 0);      // BGR 低限
    cv::Scalar upperBound(180, 240, 40);    // BGR 高限

    // 创建掩码，仅保留绿色范围内的像素
    cv::Mat mask;
    cv::inRange(bloodImage, lowerBound, upperBound, mask);

    // cv::imshow("Mask",mask);

    // 计算绿色像素总数
    int greenPixelCount = cv::countNonZero(mask);
    return greenPixelCount;
}

// 加载残血血条,并计算捕获到的像素总数
void Capture::LoadResidualBloodImage()
{
    string BloodPath = ".\\ResourceFiles\\residualBlood.png";
    // 读取图像文件
    cv::Mat bloodImage = cv::imread(BloodPath, cv::IMREAD_COLOR);

    // 检查图像是否成功加载
    if (bloodImage.empty()) {
        qDebug() << "capture::LoadResidualBloodImage 无法加载图像: " << QString::fromStdString(BloodPath);
    }
    else {
        qDebug() << "capture::LoadResidualBloodImage 成功加载图像: " << QString::fromStdString(BloodPath);
        // 在此处可以继续处理加载的图像
    }

    cv::imshow("image2", bloodImage);

    // 定义黄色像素的上下限
    //cv::Scalar lowerBound(166, 222, 33);                                                                              // BGR 低限
    cv::Scalar lowerBound(0, 65, 137);                                                                                  // BGR 低限
    cv::Scalar upperBound(35, 140, 240);                                                                                // BGR 高限

    // 创建掩码，仅保留绿色范围内的像素
    cv::Mat mask;
    cv::inRange(bloodImage, lowerBound, upperBound, mask);

    cv::imshow("Mask", mask);

    // 计算绿色像素总数
    int greenPixelCount = cv::countNonZero(mask);
    qDebug() << "capture::LoadResidualBloodImage  残血: " << greenPixelCount;
}

// 绑定槽函数
void Capture::BindSlot()
{
    connect(m_pSignalSend.get(), &SignalSend::FirstGameEnd, [&]()
        {
            isSendHp = false;
            QTimer::singleShot(1000, [&]()
                {
                    isSendHp = true;
                });
        });

    /* 通过比较倒计时前后是否相等来判断一局战斗是否结束 */
    connect(m_pSignalSend.get(), &SignalSend::FightEnd, this, &Capture::JustFightIsEnd);
    connect(m_pSignalSend.get(), &SignalSend::WhiteScreen, [this]()
        {
            bIsWhite = true;
            JustFightIsEnd();
        });
}

// 判断是否发送血量
bool Capture::isSendBlood(int& lastBloodPiexlRate, int& currBloodPiexlRate)
{
    int difference = lastBloodPiexlRate - currBloodPiexlRate;
    if (difference > Max_Difference)
    {
        qDebug() << QString::fromLocal8Bit("capture::isSendBlood 血量差值过大: ") << difference;
        return false;
    }
    lastBloodPiexlRate = currBloodPiexlRate;
    return true;
}

// [0]
// 判断血条是否被遮挡
bool Capture::BloodIsObscured(cv::Mat mask)
{
    // 将血条区域分为多个小块
    int numRows = 4; // 4行
    int numCols = 8; // 8行

    int blockWidth = mask.cols / numCols; // 每一个小格子的宽度
    int blockHeight = mask.rows / numRows; // 每一个小格子的高度

    int totalPixelCount = blockWidth * blockHeight;

    for (int rows = 0; rows < numRows; rows++)
    {
        for (int cols = 0; cols < numCols; cols++)
        {
            // 计算每个小块的掩码像素数量
            cv::Rect block(cols * blockWidth, rows * blockHeight, blockWidth, blockHeight);
            cv::Mat blockMask = mask(block);

            // 计算这个掩码中的非0个数
            int pixelCount = cv::countNonZero(blockMask);

            // 如果这一块的像素远低于预期
            if (pixelCount < 0.2 * totalPixelCount)
                return true;
        }
    }
    return false;
}
// ![0]

// [1]
// 避免误判
bool Capture::AvoidMisJudge(cv::Rect Blood)
{
    // 重新截图
    cv::Mat image = CaptureStatic::CaptureFullScreen();

    cv::Mat Image = image(Blood);
    //// 保存截图
    //// 获取当前时间戳
    //QString times = QDateTime::currentDateTime().toString("hh_mm_ss_zzz");
    //QString Path = "D:\\KofImageSave\\terminationImage";
    //QDir().mkpath(Path);
    //bool ret = cv::imwrite((Path + "\\" + times + ".png").toStdString(), Image);
    //if (!ret)
    //{
    //    qDebug() << QString::fromLocal8Bit("capture::AvoidMisJudge 保存截图失败");
    //}

    // 计算血条的像素
    int BloodPiexlNum = CaptureStatic::CalculateBloodPiexl(Image);
    qDebug() << QString::fromLocal8Bit("capture::AvoidMisJudge : 避免误判检测： 检测到的像素数: ") << BloodPiexlNum;
    
    // 还剩下1/5的血量，判定为误判
    if (BloodPiexlNum > 500)
        return true; // 误判

    return false; // 没有误判
}
// ![1]

// [2]
// 血条捕获，裁剪血条并计算血条占比
void Capture::CropBloodCalculateRate()
{
    // 如果左边的角色被选中或者右边的角色被选中才进行战斗场景的判断
    if (m_pSignalRelay->m_gameFlowInfo->isStartBloodCapture)
    {
        // 是否进行战斗场景判断,只判断第一次
        if (IsProcessFightScreenJudge)
        {
            // 判断是否进入战斗场景
            if (CaptureStatic::IsFightScene())
            {
                // 如果是战斗场景,后续就不需要再进行战斗场景的检测了,直接执行血条计算
                IsProcessFightScreenJudge = false;
                QTimer::singleShot(7000, [this]()
                    {
                        emit m_pSignalSend->StartCountDownCrop();                                                // 开始倒计时裁剪
                    });
                qDebug() << QString::fromLocal8Bit("capture::CropBloodCalculateRate 进入战斗场景");
                resetShakeSection(LeftShakeSection);                                                            // 重置震动区间
                resetShakeSection(RightShakeSection);                                                           // 重置震动区间
            }
            else
            {
                // 如果不是战斗场景,就退出
                return;
            }
        }

        /* 计算血条百分比 */
        int leftBloodRate = 0;
        int rightBloodRate = 0;
        CalculateBloodRate(leftBloodRate, rightBloodRate);

        // 计算血条差值,判断是否需要震动
        IsShake(leftBloodRate, LeftShakeSection, true);
        IsShake(rightBloodRate, RightShakeSection, false);

        // 记录上一次的血量，当游戏结束时来判断是哪一方获胜了
        lastLeftBloodRate = leftBloodRate > 0 ? leftBloodRate : lastLeftBloodRate;
        lastRightBloodRate = rightBloodRate > 0 ? rightBloodRate : lastRightBloodRate;

        // 如果这次计算的血条小于上一次留存的血条
        if (leftBloodRate < 0)
        {
            emit m_pSignalSend->write_hp((uint16_t)lastLeftBloodRate, (uint16_t)rightBloodRate);
        }
        if (rightBloodRate < 0)
        {
            emit m_pSignalSend->write_hp((uint16_t)leftBloodRate, (uint16_t)lastRightBloodRate);
        }
        if (leftBloodRate > 0 && rightBloodRate > 0)
        {
            // 发送血量信息给灯板
            emit m_pSignalSend->write_hp((uint16_t)leftBloodRate, (uint16_t)rightBloodRate);
            qDebug() << "hp1 = " << leftBloodRate << "hp2 = " << rightBloodRate;
            // 当正在战斗时就置游戏结束标志为false
            switchGameEnd = false;
        }
    }
}
void Capture::InitSelf()
{
    OnTimerBlood();
}
// ![2]

// 定时器槽函数
void Capture::OnTimerBlood()
{
    // 创建定时器,用于血条检测
    QTimer* timeBloodDetection = new QTimer(this);
    connect(timeBloodDetection, &QTimer::timeout, this, &Capture::CropBloodCalculateRate);
    timeBloodDetection->start(1050);
    qDebug() << QString::fromLocal8Bit("捕获线程ID: ") << QThread::currentThreadId();
}

// 结束时变量重置
void Capture::EndVairableReset(QString name)
{
    if (!switchGameEnd)
    {
        if (name == "left")
        {
            qDebug() << QString::fromLocal8Bit("capture::CropBloodCalculateRate 红方胜利");
            leftWinnerNum++;                                                                    // 蓝方胜利次数
            emit m_pSignalSend->leftWin();                                                       // 红方胜利信号转发
        }
        else if (name == "right")
        {
            qDebug() << QString::fromLocal8Bit("capture::CropBloodCalculateRate 蓝方胜利");
            rightWinnerNum++;                                                                   // 红方胜利次数
            emit m_pSignalSend->rightWin();                                                      // 蓝方胜利信号转发
        }
        switchGameEnd = true;
        lastLeftBloodRate = 100;
        lastRightBloodRate = 100;
        IsProcessFightScreenJudge = true;                                                       // 有一方胜利就必须再重新进行血量检测
        QThread::msleep(7000);                                                                  // 睡7s，等待下一场游戏的开始
    }
}

// 判断是否游戏结束
void Capture::JustFightIsEnd()
{
    // 如果是白屏
    if (bIsWhite)
    {
        qDebug() << QString::fromLocal8Bit("capture::DisFightEnd 战斗结束，白屏");
        GameEndProcess();
        bIsWhite = false;
        emit m_pSignalSend->StopCountDownCrop();                                                 // 停止倒计时裁剪
    }
    else
    {
        if (IsShakeSectionFinish(LeftShakeSection))
        {
            qDebug() << QString::fromLocal8Bit("capture::DisFightEnd 一局战斗结束，不是白屏，倒计时检测, 左边已经走完全流程");
            GameEndProcess();
        }
        else if (IsShakeSectionFinish(RightShakeSection))
        {
            qDebug() << QString::fromLocal8Bit("capture::DisFightEnd 一局战斗结束，不是白屏，倒计时检测, 右边已经走完全流程");
            GameEndProcess();
        }
        else
        {
            qDebug() << QString::fromLocal8Bit("capture::DisFightEnd 一局战斗结束，不是白屏，倒计时检测, 左边和右边都未走完全流程");
            emit m_pSignalSend->StartCountDownCrop();                                            // 重新开始倒计时裁剪
        }
    }
}

/* 判断左边胜利还是右边胜利 */
QString Capture::IsLeftOrRightIsEnd()
{
    if (IsShakeSectionFinish(LeftShakeSection))                                                 // 如果左边角色走完了流程，但是右边的角色没有走完，就返回右边胜利
    {
        if (!IsShakeSectionFinish(RightShakeSection))
            return "right";
    }

    if (IsShakeSectionFinish(RightShakeSection))                                                // 如果右边角色走完了流程，但是左边的角色没有走完，就返回左边胜利
    {
        if (!IsShakeSectionFinish(LeftShakeSection))
            return "left";
    }
    
    if (IsShakeSectionFinish(LeftShakeSection) && IsShakeSectionFinish(RightShakeSection))      // 如果左右两边都走完了流程，重新截图比较双方血量
    {
        int leftBloodRate = 0;
        int rightBloodRate = 0;
        CalculateBloodRate(leftBloodRate, rightBloodRate);

        if (leftBloodRate > rightBloodRate)
            return "left";
        else if (rightBloodRate > leftBloodRate)
            return "right";
    }

    return "NULL";
}

/* 判断当前震动是否完成 */
bool Capture::IsShakeSectionFinish(QMap<int, bool>& ShakeSection)
{
    if(ShakeSection[10] == true)
        return true;

    return false;
}

/* 游戏结束时的处理 */
void Capture::GameEndProcess()
{
    QString IsLeftOrRightIsEndStr = IsLeftOrRightIsEnd();
    qDebug() << QString::fromLocal8Bit("capture::GameEndProcess 战斗结束，结果: ") << IsLeftOrRightIsEndStr;

    if (IsLeftOrRightIsEndStr == "left")
    {
        /* 红方胜利时向蓝方发送震动 */
        //emit m_pSignalSend->write_single_register(SerialPort::m_shake2, SerialPort::ShakeOpen);
        //QThread::msleep(1000);
        //emit m_pSignalSend->write_single_register(SerialPort::m_shake2, SerialPort::ShakeClose);

        EndVairableReset("left");
        resetShakeSection(LeftShakeSection);                                                     // 重置震动区间
        resetShakeSection(RightShakeSection);                                                    // 重置震动区间
    }
    else
    {
        /* 蓝方胜利时向红方发送震动 */
        //emit m_pSignalSend->write_single_register(SerialPort::m_shake1, SerialPort::ShakeOpen);
        //QThread::msleep(1000);
        //emit m_pSignalSend->write_single_register(SerialPort::m_shake1, SerialPort::ShakeClose);

        EndVairableReset("right");
        resetShakeSection(LeftShakeSection);                                                     // 重置震动区间
        resetShakeSection(RightShakeSection);                                                    // 重置震动区间
    }
}

/* 计算血条百分比 */
void Capture::CalculateBloodRate(int& LeftBloodRate, int& RightBloodRate)
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();

    // 定义裁剪区域的坐标
    cv::Rect leftBlood(67, 24, 367 - 67, 37 - 24);
    cv::Rect rightBlood(432, 24, 732 - 432, 37 - 24);

    cv::Mat leftImage = image(leftBlood);                                                       // 左边的血条
    cv::Mat rightImage = image(rightBlood);                                                     // 右边的血条

    // 计算血条的像素
    int leftBloodPixelNum = CaptureStatic::CalculateBloodPiexl(leftImage);
    int rightBloodPixelNum = CaptureStatic::CalculateBloodPiexl(rightImage);

    // 计算血条的百分比
    LeftBloodRate = (leftBloodPixelNum * HealthBarPercent) / CaptureStatic::fullBloodPixelNum;
    RightBloodRate = (rightBloodPixelNum * HealthBarPercent) / CaptureStatic::fullBloodPixelNum;
}

/* 是否需要震动 */
void Capture::IsShake(int CurrBloodNum, QMap<int, bool>& ShakeSection, bool isLeft)
{
    if (CurrBloodNum > 0)
    {
        if (CurrBloodNum > 75 && CurrBloodNum < 85)
        {
            if (ShakeSection[80] == false)
            {
                SendShakeCommand(ShakeSection, 80, isLeft);
                qDebug() << "80%";
            }
        }
        
        if (CurrBloodNum > 40 && CurrBloodNum < 55)
        {
            if (ShakeSection[50] == false)
            {
                SendShakeCommand(ShakeSection, 50, isLeft);
                qDebug() << "50%";
            }
        }
        
        if (CurrBloodNum > 0 && CurrBloodNum < 25)
        {
            if (ShakeSection[10] == false)
            {
                SendShakeCommand(ShakeSection, 10, isLeft);
                qDebug() << "10%";
            }
        }
    }
}

/* 发送震动命令 */
void Capture::SendShakeCommand(QMap<int, bool>& ShakeSection, int sectionIndex, bool isLeft)
{
    if (ShakeSection[sectionIndex] == false)
    {
        if (isLeft)
        {
            //m_pSignalSend->write_single_register(SerialPort::m_shake1, SerialPort::ShakeOpen);
            //QThread::msleep(1000);
            //m_pSignalSend->write_single_register(SerialPort::m_shake1, SerialPort::ShakeClose);
            ShakeSection[sectionIndex] = true;
            qDebug() << QString::fromLocal8Bit("capture::SendShakeCommand ") << "Left" << sectionIndex << " %";
        }
        else if (!isLeft)
        {
            //m_pSignalSend->write_single_register(SerialPort::m_shake2, SerialPort::ShakeOpen);
            //QThread::msleep(1000);
            //m_pSignalSend->write_single_register(SerialPort::m_shake2, SerialPort::ShakeClose);
            ShakeSection[sectionIndex] = true;
            qDebug() << QString::fromLocal8Bit("capture::SendShakeCommand ") << "Left" << sectionIndex << " %";
        }
    }
}

void Capture::initShakeSection()
{
    LeftShakeSection.insert(80, false);
    LeftShakeSection.insert(50, false);
    LeftShakeSection.insert(10, false);

    RightShakeSection.insert(80, false);
    RightShakeSection.insert(50, false);
    RightShakeSection.insert(10, false);
}

void Capture::resetShakeSection(QMap<int, bool>& ShakeSection)
{
    for (auto it = ShakeSection.begin(); it != ShakeSection.end(); ++it)
    {
        it.value() = false;
    }
}