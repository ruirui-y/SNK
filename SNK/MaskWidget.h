#ifndef MASKWIDGET_H
#define MASKWIDGET_H

#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QWidget>

#include <QTimer>
#include "ui_MaskWidget.h"
#include "Singletion.h"

class SignalRelay;
class SignalSend;
class ShowLabel;

class MaskWidget : public QWidget, public Singletion<MaskWidget>
{
	Q_OBJECT

public:
	friend class Singletion<MaskWidget>;

public:
	~MaskWidget();
	bool showMask(bool isSingle = true, bool bIsCoinReduce = false, bool isAllowListen = false);
	bool hideMask();
	void StartListen(bool isSingle = true, bool bIsCoinReduce = true);

	void setLabelText();

	// 变量
	Ui::MaskWidgetClass ui;

protected:
	void paintEvent(QPaintEvent *event);

private:
	// 函数
	MaskWidget(QWidget* parent = 0);
	void init();
	void InitTotalCoin();
	void InitItemLayout();
	void BindSlots();

private slots:
	void UpdataLabelText();
	void On1P2PKeyPress();

public:
	bool _bIsCoinReduce = false;																// 是否应该扣币 

private:
	QPixmap m_CurrMask;

	int totalCoin_single;																		// 单人游戏所需投币数
	int totalCoin_Double;																		// 双人游戏所需投币数
	bool m_isSingle = false;																	// 是否为单人游戏

	ShowLabel * m_CoinLabel;																	// 显示投币数的标签
	QTimer *m_UpdataLabelTextTimer;																// 用于刷新标签文本的定时器

	QSharedPointer<SignalRelay> m_SignalRelay;													// 信号中转类
	QSharedPointer<SignalSend> m_SignalSend;													// 信号发射器
};

#endif // MASKWIDGET_H
