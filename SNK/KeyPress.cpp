#include "KeyPress.h"
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>
#include <cstdlib>

KeyPress::KeyPress(QObject* parent)
 : QObject(parent), isSendPressKey(true)
    , isSendReleaseKey(true)
{
    // 获取当前工作路径
    QString currentPath = QDir::currentPath();
    // 拼接路径
    QString dllPath = currentPath + "/DllFile/DllKof.dll";
    QString startBatPath = currentPath + "/Bat/start.bat";
}

// 按钮点击对应的打开事件
void KeyPress::startGameSlots()
{
    // 检查句柄
    if (hwnd != nullptr)
    {
        SetForegroundWindow(hwnd);// 将游戏程序设置为前台程序
        LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, style & ~WS_EX_TOPMOST);
        for (int i = 0; i < 3; ++i)
        {
            Sleep(3000);
            sendEnterKey();
        }
        Sleep(1000);
        sendAddKey();
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("KeyPress::startGameSlots() 句柄为空");
        return;
    }
}

// 发送全局空格键
void KeyPress::sendSpaceKey() {
    // 模拟按下空格键
    Sleep(3000); // 等待界面稳定下来
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_SPACE; // 空格键虚拟键码
    inputPress.ki.dwFlags = 0; // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT)); // 发送按下事件
    //SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2); // 关闭显示器
    Sleep(4000);
    // 模拟释放空格键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_SPACE; // 空格键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT)); // 发送释放事件*/

    qDebug() << QString::fromLocal8Bit("KeyPress::sendSpaceKey() 发送空格键成功");
    emit AnyKeyPressed();                                                                           // 发送任意键按下信号
}

// 发送全局Enter键
void KeyPress::sendEnterKey()
{
    // 模拟按下Enter键
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_RETURN;                      // enter键虚拟键码
    inputPress.ki.dwFlags = 0;                          // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT));           // 发送按下事件
    Sleep(100);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_RETURN;                    // 空格键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP;          // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT));         // 发送释放事件*/
    qDebug() << QString::fromLocal8Bit("keyPress::sendEnterKey() 发送回车键");
}

void KeyPress::sendEscapeKey()
{
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_ESCAPE;                      // escape键虚拟键码
    inputPress.ki.dwFlags = 0;                          // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT));           // 发送按下事件
    Sleep(100);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_ESCAPE;                    // escape键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP;          // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT));         // 发送释放事件*/
    qDebug() << QString::fromLocal8Bit("keyPress::sendEscapeKey() 发送Escape键");
}

void KeyPress::sendUpKey()
{
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_UP; // up键虚拟键码
    inputPress.ki.dwFlags = 0; // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT)); // 发送按下事件
    Sleep(100);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_UP; // up键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT)); // 发送释放事件*/
    qDebug() << QString::fromLocal8Bit("keyPress::sendUpKey() 发送Up键");
}

void KeyPress::sendDownKey()
{
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_DOWN; // up键虚拟键码
    inputPress.ki.dwFlags = 0; // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT)); // 发送按下事件
    Sleep(100);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_DOWN; // up键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT)); // 发送释放事件*/
}

void KeyPress::sendEnterNoSignals()
{
    // 模拟按下Enter键
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_RETURN; // enter键虚拟键码
    inputPress.ki.dwFlags = 0; // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT)); // 发送按下事件
    Sleep(2000);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_RETURN; // 空格键虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT)); // 发送释放事件*/
    qDebug() << QString::fromLocal8Bit("keyPress::sendEnterKey() 发送回车键成功");
}

void KeyPress::sendCombineKeys()
{
    INPUT inputs[6] = { 0 };

    // 1. ESC键按下并释放
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_ESCAPE;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_ESCAPE;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    // 2. 上方向键按下并释放
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_UP;
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_UP;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    // 3. 回车键按下并释放
    inputs[4].type = INPUT_KEYBOARD;
    inputs[4].ki.wVk = VK_RETURN;
    inputs[5].type = INPUT_KEYBOARD;
    inputs[5].ki.wVk = VK_RETURN;
    inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;

    // 发送并检查返回值
    UINT result = SendInput(6, inputs, sizeof(INPUT));
    if (result != 6) {
        qDebug() << QString::fromLocal8Bit("发送失败! 成功发送的事件数: ") << result << "/6";
    }
    else {
        qDebug() << QString::fromLocal8Bit("所有按键已成功提交到系统输入队列!");
    }
}

void KeyPress::sendAddKey()
{
    // 模拟按下Enter键
    INPUT inputPress;
    inputPress.type = INPUT_KEYBOARD;
    inputPress.ki.wVk = VK_ADD; // ADD键虚拟键码
    inputPress.ki.dwFlags = 0; // 按下键盘的标志
    SendInput(1, &inputPress, sizeof(INPUT)); // 发送按下事件
    Sleep(2000);
    // 模拟按下释放键
    INPUT inputRelease;
    inputRelease.type = INPUT_KEYBOARD;
    inputRelease.ki.wVk = VK_ADD; // ADD虚拟键码
    inputRelease.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键盘的标志
    SendInput(1, &inputRelease, sizeof(INPUT)); // 发送释放事件*/
    qDebug() << QString::fromLocal8Bit("keyPress::sendEnterKey() 发送Add键成功");
}

// 列出所有标题的窗口
/*void KeyPressWidget::ListOpenWindows() {
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        wchar_t title[256];
                wchar_t className[256];

                GetWindowTextW(hwnd, title, sizeof(title) / sizeof(wchar_t));
                GetClassNameW(hwnd, className, sizeof(className) / sizeof(wchar_t));

                // 输出窗口标题和类名
                if (IsWindowVisible(hwnd)) {
                    qDebug() << "窗口标题:" << QString::fromWCharArray(title);
                    qDebug() << "窗口类名:" << QString::fromWCharArray(className);
                }
                return TRUE; // 继续枚举
            }, 0);
}*/


BOOL CALLBACK KeyPress::EnumWindowProc(HWND hwnd, LPARAM lParam) {
    char title[256];
    GetWindowTextA(hwnd, title, sizeof(title));

    // 检查窗口是否可见
    if (IsWindowVisible(hwnd)) {
        qDebug() << QString::fromLocal8Bit("keyPress::EnumWindowProc() 窗口标题:") << title; // 输出窗口标题

        // 如果找到 KOF XV 窗口，推到前面并发送空格键
        if (strcmp(title, "KOFXV  ") == 0) {
            qDebug() << QString::fromLocal8Bit("keyPress::EnumWindowProc() 找到 KOF XV 窗口，准备将其推到前面");

            ShowWindow(hwnd, SW_RESTORE); // 恢复窗口
            SetForegroundWindow(hwnd); // 设置为前景窗口

            // 发送空格键
            // sendSpaceKey();
            return FALSE; // 找到窗口后停止枚举
        }
    }
    return TRUE; // 继续枚举
}

// 槽函数 找到拳皇这个程序并且发送空格信号
void KeyPress::findAndSendSpaceKey() {
    EnumWindows(EnumWindowProc, reinterpret_cast<LPARAM>(this));
}

// 发现这个应用程序
HWND KeyPress::FindActivateKOFVW()
{
    qDebug() << QString::fromLocal8Bit("keyPress::FindActivateKOFVW(): 查找拳皇窗口句柄");
    HWND hWnd = nullptr;
    while (hWnd == nullptr)
    {
        hWnd = FindWindowA("UnrealWindow", "KOFXV  ");
    }
    if (hWnd)
    {   
        hwnd = hWnd;
        qDebug() << QString::fromLocal8Bit("keyPress::FindActivateKOFVW() 找到了这个窗口");
        return hWnd;
    }
    else
        qDebug() << QString::fromLocal8Bit("keyPress::FindActivateKOFVW() 没找到这个窗口");
    return nullptr;
}

// 打开视频按钮按下对应的槽函数
void KeyPress::openVideSlots()
{
    // 发送句柄
    if (hwnd == nullptr)
    {
        qDebug() << QString::fromLocal8Bit("keyPress::openVideSlots() 句柄为空");
        return;
    }
}




