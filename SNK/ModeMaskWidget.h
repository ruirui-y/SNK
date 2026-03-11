#ifndef MODEMASKWIDGET_H
#define MODEMASKWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include "SignalSend.h"
#include "SignalRelay.h"
#include "PushButton.h"
#include "ui_ModeMaskWidget.h"
#include "Singletion.h"

class ModeMaskWidget : public QWidget, public Singletion<ModeMaskWidget>
{
	Q_OBJECT

public:
	friend class Singletion<ModeMaskWidget>;

public:
	~ModeMaskWidget();

	void BindSlot();
	
	void showMask();																									// 显示遮罩时允许监听1P2P信号
	void hideMask();																									// 撤销遮罩时不允许监听


public slots:
	/* 单人双人模式信号对应的槽函数(New) */
	void On1P2PButtonClicked();																							// 当1P2P按键按下时

signals:
	void OpenSwitchMode();
	void PressEnd();																									// 按键按下结束了，停止定时器的播放

protected:
	void paintEvent(QPaintEvent *event);

private:
	ModeMaskWidget(QWidget* parent = nullptr);
	/* 计算当前索引 */
	void CalcuCurrFrameIndex();
	
private:
	Ui::ModeMaskWidgetClass ui;

	QSharedPointer<SignalRelay> m_SignalRelay;													// 信号中转类
	QSharedPointer<SignalSend> m_SignalSend;													// 信号发射器

	/* 动画定时器 + 序列帧 + 当前帧索引 */
	QTimer* m_AnimTimer;
	QVector<QPixmap> m_ModePixmaps;
	int m_CurrentFrame;
	bool bMaskIsShow;																			// 遮罩是否显示
};

#endif // MODEMASKWIDGET_H
