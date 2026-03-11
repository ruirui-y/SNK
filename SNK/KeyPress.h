#ifndef KEYPRESSWIDGET_H
#define KEYPRESSWIDGET_H

#include "capture.h"
#include <QObject>
#include <QWindow>
#include <QWidget>
#include <QKeyEvent>

class KeyPress : public QObject
{
    Q_OBJECT
public:
    explicit KeyPress(QObject* parent = nullptr);

    void sendSpaceKey();                                        // 发送全局空格键
    void sendEnterKey();                                        // 发送全局Enter键
    void sendEscapeKey();                                       // 发送全局Escape键
    void sendUpKey();                                           // 发送全局向上键
    void sendDownKey();                                         // 发送全局向下键
    void sendEnterNoSignals();                                  // 发送全局Enter键，不发送信号
    void sendCombineKeys();

public slots:
    void startGameSlots();                                      // 游戏开始按钮点击对应的打开事件，前台程序
    void findAndSendSpaceKey();                                 // 找到拳皇这个程序并且发送空格信号
    void openVideSlots();                                       // 打开视频按钮按下对应的槽函数

signals:
    void keyPressed(const QString& key);
    void EnterSuccess();                                        // 发送Enter键成功信号
    void AnyKeyPressed();                                       // 发送任意键按下信号
private:
    // 函数
    static BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam);
    static void sendAddKey();                                   // 发送全局Escape键
    HWND FindActivateKOFVW();                                   // 发现这个窗口并返回句柄

    // 变量
    bool isSendPressKey;                                        // 是否发送空格按压信号
    bool isSendReleaseKey;                                      // 是否发送空格释放信
public:
    HWND hwnd;                                                  // 保存拳皇的句柄
};

#endif // KEYPRESSWIDGET_H