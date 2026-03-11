#ifndef SHOWSCREEN_H
#define SHOWSCREEN_H
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include "SignalSend.h"
#include "Singletion.h"

class ShowScreen : public QWidget
{
	Q_OBJECT

public:
	ShowScreen(QWidget* parent = nullptr, int ScreenIndex = 0);
	~ShowScreen();
	void BintSlot(); // °ó¶¨²Ûº¯Êý

	void setImage(QString imagePath);
	void setScreen(int index);
	void setImageIndex(int index);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QLabel *label;
	int chuzhaobiaoIndex;
	int m_width = 0;
	int m_height = 0;
	int m_screenIndex = 0;
	int m_x = 0;
	int m_y = 0;
	QSharedPointer<SignalSend> m_SignalSend;
};
#endif // SHOWSCREEN_H