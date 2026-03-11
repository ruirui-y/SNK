#include "StandbyAnimation.h"
#include <QApplication>
#include <QDir>
#include <QFont>

StandbyAnimation::StandbyAnimation(QObject* parent)
    :currentColorIndex(0)
{
    textItem = new QGraphicsPixmapItem(QPixmap(":/chuzhaobiao/CoinImage.png"));
    // 初始化颜色列表
    m_SignalSend = SignalSend::GetInstance();
    textTimer = new QTimer(this);
    playStandbyAnimation();                                                         // 在主线程中初始化所有资源
    BindSlot();
}

StandbyAnimation::~StandbyAnimation()
{
    if (textItem != nullptr)
    {
        delete textItem;
        textItem = nullptr;
    }
    if (videoItem!= nullptr)
    {
        delete videoItem;
        videoItem = nullptr;
    }
    if (scene != nullptr)
    {
        delete scene;
        scene = nullptr;
    }
    if (view != nullptr)
    {
        delete view;
        view = nullptr;
    }
    if (player!= nullptr)
    {
        delete player;
        player = nullptr;
    }
}

// 初始化视频资源加载
void StandbyAnimation::playStandbyAnimation()
{
    // 创建场景和视图
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 获取屏幕分辨率
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 设置视图大小为屏幕尺寸
    view->setFixedSize(screenWidth, screenHeight);
    view->setSceneRect(0, 0, screenWidth, screenHeight);

    // 创建视频项
    videoItem = new QGraphicsVideoItem();
    videoItem->setSize(QSizeF(screenWidth, screenHeight));                                  // 设置视频项大小为全屏
    scene->addItem(videoItem);

    // 创建文本图片
    textItem->setPos(screenWidth / 2 - 150, screenHeight / 2 + 450);                        // 设置图片位置
    textItem->setZValue(1);                                                                 // 确保图片在最上层显示
    scene->addItem(textItem);

    // 设置定时器控制文本闪烁
    QObject::connect(textTimer, &QTimer::timeout, [this]() {
        textItem->setVisible(!textItem->isVisible());
        });
    // 创建媒体播放器
    player = new QMediaPlayer();
    player->setVideoOutput(videoItem);

    // 设置视频文件路径（替换为你的路径）
    QString videoPath = QDir::currentPath() + "/Video/kof.mp4";
    player->setMedia(QUrl::fromLocalFile(videoPath));

    // 循环播放
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status)
        {
            // 如果播放结束
            if (status == QMediaPlayer::EndOfMedia)
            {
                player->setPosition(0);                                         // 重置播放位置
                player->play();                                                 // 重新播放
            }
        });
}

// 停止播放
void StandbyAnimation::stopStandbyAnimation()
{
    // 确保只会停止一次
    if (isPlaying)
    {
        if (player != nullptr)
        {
            player->stop();
        }
        if (view != nullptr)
        {
            view->close();
        }
        textTimer->stop();
        isPlaying = false;
        emit m_SignalSend->stopCoinFlash();                                      // 停止投币闪烁
        qDebug() << "Stop Standby Animation";
    }
}

// 绑定槽函数
void StandbyAnimation::BindSlot()
{
    // 当播放视频的信号到来时，播放视频
    connect(m_SignalSend.get(), &SignalSend::StartPlayMoive, this, &StandbyAnimation::StartStandbyAnimation, Qt::QueuedConnection);
    // 接受到投币信号，停止播放
    connect(m_SignalSend.get(), &SignalSend::stopStandbyAnimation, this, &StandbyAnimation::stopStandbyAnimation, Qt::QueuedConnection);
}

// 播放视频
void StandbyAnimation::StartStandbyAnimation()
{
    if (player != nullptr)
    {
        player->play();
        textTimer->start(500);                                                                  // 启动文本闪烁定时器
        isPlaying = true;                                                                       // 正在播放
        view->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        // 显示视图并进入全屏
        view->showFullScreen();
        qDebug() << "Start Standby Animation";
        emit m_SignalSend->SecondScreenShowText();                                              // 发送信号，通知副界面显示屏幕文字
    }
}