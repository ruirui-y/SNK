#ifndef ZHANGDANWIDGET_H
#define ZHANGDANWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QVector>
#include<QLabel>
#include "ui_zhangdanWidget.h"

class SignalRelay;
class SignalSend;

class zhangdanWidget : public QWidget
{
	Q_OBJECT

public:
	zhangdanWidget(QWidget *parent = nullptr);
	~zhangdanWidget();

public slots:
	void writeConfig(bool isSingleMode);
	void resetConfig();

private:
	void Init();
	void initSignalRelay();
	void initSignalSend();
	void initConfig();
	void initUI();
	bool isCheckGroupExist(QString group);								// 检查某一个组是否存在
	void BindSlots();													// 绑定槽函数
	void updateDateUI();												// 更新日期UI
	
private:
	QLabel* labelDate;
	QLabel* labelDateStr;
	QLabel* labelDateEnd;
	QLabel* labelDateEndStr;
	QLabel* labelCoins;
	QLabel* labelCoinNums;
	QLabel* labelGames;
	QLabel* labelGameNums;
	
	QVector<int> readConfig(QString group);								// 读取配置文件
	QVector<QString> readConfigStr(QString group);						// 读取配置文件

	Ui::zhangdanWidgetClass ui;
	QString _Path;
	QScopedPointer<QSettings> settings;
	QSharedPointer<SignalRelay> signalRelay;
	QSharedPointer<SignalSend> signalSend;
	
};
#endif // ZHANGDANWIDGET_H
