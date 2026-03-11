#include "ShowScreen.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QScreen>
#include <QPixmap>
#define HAI_BAO 59
#define BACKGROUND_X 194
#define BACKGROUND_Y 586
#define TEXT_X 829
#define TEXT_Y 573

ShowScreen::ShowScreen(QWidget* parent, int ScreenIndex)
	: label(new QLabel(this))
{
	chuzhaobiaoIndex = HAI_BAO;
	// 设置屏幕控制哪个屏幕
	setScreen(ScreenIndex);

	m_SignalSend = SignalSend::GetInstance();

	// 去除边框
	setAttribute(Qt::WA_TranslucentBackground);
	
	update();
}

ShowScreen::~ShowScreen()
{
	if (label) {
		delete label;
		label = nullptr;
	}
}

// 绑定槽函数
void ShowScreen::BintSlot()
{
}

// 设置图片
void ShowScreen::setImage(QString path)
{
	QPixmap pixmap(path);
	if (!pixmap.isNull()) {
		label->setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
	else {
		qDebug() << "image is null";
	}
}

// 设置显示器的索引
void ShowScreen::setScreen(int index)
{
	QList<QScreen*> screens = QGuiApplication::screens();

	if (index < screens.size()) {
		m_screenIndex = index;
		setGeometry(screens[index]->geometry());
		showFullScreen(); // 全屏显示

		m_width = screens[index]->geometry().width();
		m_height = screens[index]->geometry().height();
		
		m_x = screens[index]->geometry().x();
		m_y = screens[index]->geometry().y();
		
		qDebug() << "index = " << index << "width:" << width() << "height:" << height();
		qDebug() << "Screen Name: " << screens[index]->name();
	}
}

// 设置图片的索引
void ShowScreen::setImageIndex(int index)
{
	chuzhaobiaoIndex = index;
	update();
}

// 重绘事件
void ShowScreen::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QString num = QString::number(chuzhaobiaoIndex);

	QString path = QString(":/chuzhaobiao/%1.png").arg(num);
	QPixmap pixmap(path);
	// 拼接字符串
	if (!pixmap.isNull()) {
		// 获取窗口大小
		QSize windowSize = size();

		// 缩放图片，确保填充整个窗口
		QPixmap scaledPixmap = pixmap.scaled(windowSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

		// 绘制图片
		painter.drawPixmap(0, 0, scaledPixmap);
	}
	else {
		qDebug() << "Failed to load image: " << path;
	}
}
