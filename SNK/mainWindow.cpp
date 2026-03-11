#include "mainWindow.h"
#include <QBitmap>
#include <QTimer>
#include <QGridLayout>
#include <QDebug>
#include <QDir>
#include "SignalRelay.h"
#include "PushButton.h"

// 菜单停留时间
#define MENU_STAND_TIME 10000 

MainWindow::MainWindow(QWidget* parent)
    :QMainWindow(parent),
    tiaoshi(new tiaoshiWidget),
    toubi(new toubiWidget),
    zhangdan(new zhangdanWidget)
{
    Init();
    Listening();
}

MainWindow::~MainWindow()
{
    if (ui)
    {
        delete ui;
        ui = nullptr;
    }
}

// 显示主界面
void MainWindow::showMainWindow()
{
    if (toubi)
    {
        toubi->hide(); // 隐藏之前的窗口
    }
    show();
}

void MainWindow::Listening()
{
    connect(ui->toubiButton, &PushButton::pressed, this, [&]()
        {
            m_isPressed = true;
            qDebug() << QString::fromLocal8Bit("投币Button被按下");
        });
    QTimer::singleShot(MENU_STAND_TIME, [&]()
        {
            if (!m_isPressed)
            {
                ui->startButton->clicked();
            }
        });
}

void MainWindow::Init()
{
    ui = new Ui::SNKClass();
    ui->setupUi(this);
    setFixedSize(900, 600);
    ui->startButton->setFixedSize(120, 60);
    ui->exitButton->setFixedSize(120, 60);
    ui->tiaoshiButton->setFixedSize(120, 60);
    ui->toubiButton->setFixedSize(120, 60);
    ui->zhangdanButton->setFixedSize(120, 60);
    
    tiaoshi->hide();
    toubi->hide();
    zhangdan->hide();

    BindSlotsFunc();
    update();
}

// 绘制窗口
void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0,QPixmap());
    QWidget::paintEvent(event);
}

// 绑定主界面按钮对应的槽函数
void MainWindow::BindSlotsFunc()
{
    // 创建一个 lambda，用于切换窗口逻辑
    auto switchWindow = [this](const QString& widgetName) {
        if (widgetName == "tiaoshi") {
            tiaoshi->show();
        }
        else if (widgetName == "toubi") {
            toubi->show();
        }
        else if (widgetName == "zhangdan") {
            zhangdan->show();
        }
        };

    // 绑定按钮点击事件
    connect(ui->tiaoshiButton, &QPushButton::clicked, this, [this, switchWindow]() {
        switchWindow("tiaoshi");
        });
    connect(ui->toubiButton, &QPushButton::clicked, this, [this, switchWindow]() {
        switchWindow("toubi");
        });
    connect(ui->zhangdanButton, &QPushButton::clicked, this, [this, switchWindow]() {
        switchWindow("zhangdan");
        });

    // 退出按钮
    connect(ui->exitButton, &QPushButton::clicked, this, []() {
        QCoreApplication::quit();
        });

    // 绑定投币窗口中退出按钮点击发生的事件
    connect(toubi->ui->exitButton, &QPushButton::clicked, this, &MainWindow::showMainWindow);
}