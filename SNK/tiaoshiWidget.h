#ifndef TIAOSHIWIDGET_H
#define TIAOSHIWIDGET_H

#include <QWidget>
#include "ui_tiaoshiWidget.h"

class tiaoshiWidget : public QWidget
{
	Q_OBJECT

public:
	tiaoshiWidget(QWidget *parent = nullptr);
	~tiaoshiWidget();
private:
	Ui::tiaoshiWidgetClass ui;
};
#endif // TIAOSHIWIDGET_H
