#include "mainWindow.h"
#include "capture.h"
#include "LogRecord.h"
#include <QtWidgets/QApplication>
#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include <QByteArray>
#include "SerialPort.h"
#include "SwitchGame.h"
#include "ConfirmAvatar.h"
#include "CaptureStatic.h"
#include "ShowScreen.h"
#include "SerialThread.h"
#include "MaskWidget.h"
#include "SignalRelay.h"
#include "StandbyAnimation.h"
#include "GlobaScreenShot.h"
#include "Encryption.h"
#include "ShowLabel.h"
#include "ThreadPool.h"
#include "TextRecognition.h"

// 分配线程
void AllocationThread()
{
    ThreadPool* pool = new ThreadPool;

    // 创建对象（无父对象）
    // Capture* c = new Capture;
    SerialPort* s = new SerialPort;
    GlobaScreenShot* g = new GlobaScreenShot;
    LogRecord* l = new LogRecord;

    // 使用安全方式移动对象并初始化
    // pool->moveToThreadPool(c);
    pool->moveToThreadPool(s);
    pool->moveToThreadPool(g);
    pool->moveToThreadPool(l);
    emit pool->startInit();
}

// 测试合并图片功能
void TestMergeImage()
{
    // 测试绘制图片功能
    QLabel* _Label = new QLabel;
    // 加载图片
    QPixmap Src("./a/1/2.png");
    QPixmap Overlay("./a/2/Failed_31.png");
    QPixmap Combined = CaptureStatic::MergeImage(Src, Overlay);

    _Label->setPixmap(Combined);
    _Label->show();
}

// 测试最小屏幕的动画的功能
void TestMinScreenAnimation()
{
    QPixmap Src("./a/1/2.png");
    QVector<QPixmap> Overlays;
    CaptureStatic::LoadAnimPixmaps(Overlays, "./a/2/Failed_", 31);
    QVector<QPixmap> AnimationImages = CaptureStatic::MergeImages(Src, Overlays);
    ShowMinScreen* showMinScreen = new ShowMinScreen;
    showMinScreen->setScreen(1, 512, 200);

    QTimer* timer = new QTimer;
    int i = 0;
    QObject::connect(timer, &QTimer::timeout, [showMinScreen, AnimationImages, timer, &i]()
        {
            showMinScreen->SetLeftLabelImage(AnimationImages[i], 256, 172);
            i++;
            qDebug() << "i = " << i;
            if (i >= AnimationImages.size())
            {
                timer->stop();
                delete timer;
            }
        });
    timer->start(100);
}

/* 登录验证 */
bool LogVerify()
{
    // 直接赋值，QByteArray 会在内部自动分配并管理内存
    QByteArray userPassword = "K7#pL2!m";
    QByteArray adminPassword = "K7#pL2!m";

    return LoginVerify(userPassword.data(), adminPassword.data());
}

/* 加载样式表 */
void LoadStyle(QApplication* app)
{
    QFile file(":/StyleSheet/stylesheet.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(file.readAll());
        app->setStyleSheet(style);
        file.close();
        qDebug() << "Application stylesheet:" << app->styleSheet().left(300);
        qDebug() << "Load Style Success";
    }
    else
    {
        qDebug() << "Load Style Failed";
    }
}

/* 标签测试 */
void LabelTest()
{
    ShowLabel* showLabel = new ShowLabel;
    showLabel->setObjectName("TextLabel");
    showLabel->UpdateStyleSheet("Hello World!");
    showLabel->show();
    // 在创建m_CoinLabel后输出
    qDebug() << "Widget stylesheet:" << showLabel->styleSheet();
}

int index = 0;
/* 裁剪图片 */
void CropImage(const QString& imagePath, const cv::Rect& rect)
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();
    // 裁剪图片
    cv::Mat croppedImage = image(rect);
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QString name = timestamp + "_" + QString::number(index) + ".png";
    index++;

    QDir().mkpath(imagePath);  // 创建保存目录
    cv::imwrite((imagePath + "/" + name).toStdString(), croppedImage);
}

/* 裁剪图片 

    cv::Rect rect1 = cv::Rect(296, 156, 206, 18);
    cv::Rect rect2 = cv::Rect(296, 181, 206, 18);
    cv::Rect rect3 = cv::Rect(296, 205, 206, 18);
    cv::Rect rect4 = cv::Rect(296, 229, 206, 18);
    cv::Rect rect5 = cv::Rect(296, 254, 206, 18);
    QTimer::singleShot(3000, [&]()
        {
            CropImage("D:/SnkOptimize/Chinese/10.15/SNK/SNK/temp", rect1);
            CropImage("D:/SnkOptimize/Chinese/10.15/SNK/SNK/temp", rect2);
            CropImage("D:/SnkOptimize/Chinese/10.15/SNK/SNK/temp", rect3);
            CropImage("D:/SnkOptimize/Chinese/10.15/SNK/SNK/temp", rect4);
            CropImage("D:/SnkOptimize/Chinese/10.15/SNK/SNK/temp", rect5);
            QMessageBox::information(nullptr, u8"提示", u8"截图成功!");
        });
*/


/* 文本校验 */
void TextVerify()
{
    QSharedPointer<TextRecognition> Instance = TextRecognition::GetInstance();
    QString currDir = QDir::currentPath();
    currDir += "/MenuButton/Single/5.png";
    cv::Mat img1 = cv::imread(currDir.toStdString());

    bool ret = Instance->CompareImageText(img1, img1);
    if (ret)
    {
        qDebug() << QString::fromLocal8Bit("文本校验成功");
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // 加载样式表
    LoadStyle(&a);

    // 密码狗验证
    //if (!LogVerify())
    //    return -1;

    TextVerify();

    MainWindow w;
    w.show();

    // 所有的窗口全都在主线程中创建
    ModeMaskWidget::GetInstance();
    MaskWidget::GetInstance();

    // 创建视频播放对象,在主线程中创建
    StandbyAnimation* standby = new StandbyAnimation;

    // 启动游戏
    QObject::connect(w.ui->startButton, &QPushButton::clicked, [&]()
        {
            static bool isStart = true;
            if (isStart)
            {             
                w.hide();                                                                                       // 同一时刻只能有一个窗口显示
                AllocationThread();                                                                             // 分配线程
                // ConfirmAvatar* c = new ConfirmAvatar;                                                           // 创建确认对象
                SerialThread* s = new SerialThread;                                                             // 创建串口线程对象
                isStart = false;
                qDebug() << "MainThread = " << QThread::currentThreadId();
            }
        });
    return a.exec();
}