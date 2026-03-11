#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMainWindow>
#include <QCoreApplication>
#include "tiaoshiWidget.h"
#include "toubiWidget.h"
#include "zhangdanWidget.h"
#include "ui_SNK.h"
#include <memory>

using namespace std;
#include <QScopedPointer>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showMainWindow();                          // 显示主窗口
    Ui::SNKClass* ui = nullptr;                     // 界面类
    void Listening();                               // 监听

protected:
    void paintEvent(QPaintEvent* event) override;   // 绘制窗口
    void BindSlotsFunc();                           // 绑定槽函数

private:
    void Init();                                    // 初始化界面

private:
    QScopedPointer<tiaoshiWidget> tiaoshi;
    QScopedPointer<toubiWidget> toubi;
    QScopedPointer<zhangdanWidget> zhangdan;
    
    bool m_isPressed = false;
};

#endif // MAINWINDOW_H
