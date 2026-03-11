#include "VideoWidget.h"

VideoWidget::VideoWidget(QWidget *parent)
	: QVideoWidget(parent),showText(true)
{
	ui.setupUi(this);
	init();
}

VideoWidget::~VideoWidget()
{}

// 绘制闪烁文本
void VideoWidget::paintEvent(QPaintEvent* event)
{
	// 调用基类的绘制事件
	QVideoWidget::paintEvent(event);
	if (showText) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);

		// 设置字体为支持中文的字体
		QFont font("微软雅黑", 36, QFont::Bold); // 微软雅黑支持中文
		painter.setFont(font);
		painter.setPen(Qt::red);

		// 绘制中文文本
		QString chineseText = "请投币";
		painter.drawText(rect(), Qt::AlignCenter, chineseText);
	}
}

void VideoWidget::init()
{
	// 设置定时器，用于切换文本的显示状态
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this]() {
		showText = !showText; // 切换显示状态
		update();            // 触发重绘
		});
	timer->start(500); // 每500毫秒闪烁一次
}
