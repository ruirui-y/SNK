#ifndef SWITCHGAME_H
#define SWITCHGAME_H

#include <QObject>

// qt
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QDir>

// c++
#include <string>
using namespace std;

class SwitchGame  : public QObject
{
	Q_OBJECT

public:
	SwitchGame(QObject *parent = nullptr);
	~SwitchGame();

	// 函数
	static void OpenApplication(); // 打开游戏
	static void CloseApplication(); // 关闭游戏
	static QString GetCurrentPath();
};
#endif // !SWITCHGAME_H