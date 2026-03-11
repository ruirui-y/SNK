#ifndef SHOWMINSCREEN_H
#define SHOWMINSCREEN_H
// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Qt
#include <QWidget>
#include <QLabel>
#include <QScreen>
#include <QGridLayout>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include <QPaintEvent>

class ShowMinScreen : public QWidget
{
	Q_OBJECT

public:
	ShowMinScreen();
	~ShowMinScreen();

	void SetImage(cv::Mat leftImg, cv::Mat rightImg, int width, int height);
	void setScreen(int screenIndex,int width, int height);
	void SetImage(const QPixmap& pixmap1, const QPixmap& pixmap2, int width, int height);

	// 设置标签的背景图片
	void SetLeftLabelImage(const QPixmap& pixmap1, int width, int height);
	void SetRightLabelImage(const QPixmap& pixmap2, int width, int height);

	/* 合并两张图片 */
	QPixmap MergeImage(QPixmap& src, QPixmap& overlay);

	/* 返回合并的图片的数组 */
	QVector<QPixmap> MergeImages(QPixmap& src, QVector<QPixmap>& overlays);

public slots:
	void StartAnimation();																	// 开始动画
	void StopAnimation();																	// 停止动画
	
protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void Init();
	void UpdateBackground();																// 更新背景图片

private:
	QGridLayout* layout;
	QLabel* leftLabel;
	QLabel* rightLabel;
	
	QPixmap currBackground;																	// 当前背景图片
	int currBackgroundIndex;																// 当前背景图片索引
	QPixmap backgroundPixmap;																// 背景图片
	QVector<QPixmap> backgroundAnimation;													// 背景动画

	QTimer* AnimTimer;																		// 动画定时器
};
#endif // SHOWMINSCREEN_H