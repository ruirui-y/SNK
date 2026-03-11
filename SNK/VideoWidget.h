#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QVideoWidget>
#include "ui_VideoWidget.h"

class VideoWidget : public QVideoWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *parent = nullptr);
	~VideoWidget();
	void init();

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::VideoWidgetClass ui;
	bool showText; // 是否展示闪烁的文本
};
#endif // VIDEOWIDGET_H