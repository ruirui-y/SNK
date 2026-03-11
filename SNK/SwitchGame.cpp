#include "SwitchGame.h"

SwitchGame::SwitchGame(QObject *parent)
	: QObject(parent)
{}

SwitchGame::~SwitchGame()
{}


//获取当前路径
QString SwitchGame::GetCurrentPath()
{
    QString currentPath = QDir::currentPath();
    qDebug() << QString::fromLocal8Bit("SwitchGame::Init() : currentPath: ") << currentPath;
    return currentPath;
}

// 打开应用程序
void SwitchGame::OpenApplication()
{
    // 获取当前路径
    QString currentPath = GetCurrentPath();
    // 拼接路径
    QString startBatPath = currentPath + "/Bat/start.bat";

    const string appPath = startBatPath.toStdString();

    int ret = system(appPath.c_str());
    if (ret == 0)
    {
        qDebug() << QString::fromLocal8Bit("keyPress::OpenApplication() 打开应用程序成功");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("keyPress::OpenApplication() 打开应用程序失败");
    }
}

// 关闭应用程序
void SwitchGame::CloseApplication()
{
    // 获取当前路径
    QString currentPath = GetCurrentPath();
    // 拼接路径
    QString startBatPath = currentPath + "/Bat/end.bat";

    const string appPath = startBatPath.toStdString();
    
    int ret = system(appPath.c_str());
    if (ret == 0)
    {
        qDebug() << QString::fromLocal8Bit("SwitchGame::CloseApplication()： 关闭应用程序失败");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("SwitchGame::CloseApplication()： 关闭应用程序失败");
    }
}
