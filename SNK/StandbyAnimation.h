#ifndef STANDBYANIMATION_H
#define STANDBYANIMATION_H

#include <windows.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QTimer>
#include <QColor>
#include <QVector>
#include <QScreen>
#include <QPixmap>
#include "SignalSend.h"

class StandbyAnimation: public QObject
{
    Q_OBJECT

public:
    StandbyAnimation(QObject* parent = nullptr);
    ~StandbyAnimation();
    // 开始播放动画
    void playStandbyAnimation();
    // 绑定槽函数
    void BindSlot();

public slots:
    void StartStandbyAnimation();                                                       // 开始播放动画
    void stopStandbyAnimation();                                                        // 停止播放动画

private:
    QGraphicsPixmapItem* textItem;
    int currentColorIndex;
    QGraphicsView* view;                                                                // 摄像机
    QGraphicsScene* scene;                                                              // 场景
    QMediaPlayer* player;                                                               // 播放器
    QGraphicsVideoItem* videoItem;
    bool isPlaying = false;                                                             // 刚开始，没有播放动画
    QSharedPointer<SignalSend> m_SignalSend;

    QTimer* textTimer;                                                                  // 控制文本闪烁
};

#endif // STANDBYANIMATION_H