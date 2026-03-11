#ifndef TOUBIWIDGET_H
#define TOUBIWIDGET_H

#include <QSettings>
#include <QWidget>
#include "ui_toubiWidget.h"
#include <QSharedPointer>

class toubiWidget : public QWidget
{
	Q_OBJECT

public:
	toubiWidget(QWidget *parent = nullptr);
	~toubiWidget();
	// 初始化
	void init();
	// 绑定槽函数
	void BindSlot();

	// 静态变量
	static int coinCount;							// 记录多少个硬币数开始一局游戏
	static int juCount;								// 记录coinCount个硬币可以玩几局
	static int xuwanCount;							// 记录可以续玩多少局
	static bool isGameMode;							// 是否是比赛模式
	
	Ui::toubiWidgetClass* ui;
	QSharedPointer<QSettings> settings;

public slots:
	// 设置硬币数
	void setCoinCount(int count);
	void setJuCount(int count);
	void setXuwanCount(int count);
	void setIsGameMode(int modeIndex);

	void Destroy();									// 销毁指针变量

signals:
	void showMainWindows();							// 显示主界面
};
#endif // TOUBIWIDGET_H