#include "ShowMinScreen.h"
#include <QDebug>

#include "CaptureStatic.h"

#define BackGround "/background.png"
#define BackGroundWidth 512
#define BackGroundHeight 128

#define BackGroundAnimationNums 30
#define BackGroundAnimationInterval 2000

ShowMinScreen::ShowMinScreen()
:leftLabel(new QLabel(this)), rightLabel(new QLabel(this)),layout(new QGridLayout(this))
{
    Init();
}

ShowMinScreen::~ShowMinScreen()
{
    backgroundAnimation.clear();
    qDebug() << "~ShowMinScreen()";
}

void ShowMinScreen::Init()
{
    // 1.去除边框
    setWindowFlags(Qt::FramelessWindowHint);

    // 2.加载背景图片
    QString backGroundPath = QDir::currentPath();
    backGroundPath += BackGround;
    QPixmap background(backGroundPath);
    backgroundPixmap = background;
    currBackground = backgroundPixmap;
    currBackgroundIndex = 0;

    // 3.加载背景动画图片
    CaptureStatic::LoadAnimPixmaps(backgroundAnimation, "./BackGroundAnimation/", BackGroundAnimationNums);

    // 4.初始化布局和 QLabel
    layout->setContentsMargins(0, 0, 0, 0);                                                                                             // 设置边距为 0
    layout->setSpacing(0);                                                                                                              // 设置控件间间距为 0
    // 设置 QLabel 的对齐方式为左上角和右上角
    layout->addWidget(leftLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);                                                                   // 左上角
    layout->addWidget(rightLabel, 0, 1, Qt::AlignRight | Qt::AlignTop);                                                                 // 右上角
    setLayout(layout);

    // 5.设置 QLabel 的大小策略
    leftLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 6.初始化定时器
    AnimTimer = new QTimer(this);
    connect(AnimTimer, &QTimer::timeout, this, &ShowMinScreen::UpdateBackground);

    update(); // 更新显示
}

/* 更新背景图片 */
void ShowMinScreen::UpdateBackground()
{
    currBackground = backgroundAnimation[currBackgroundIndex];
    currBackgroundIndex = (currBackgroundIndex + 1) % BackGroundAnimationNums;
    update(); // 更新显示
}

/* 开始动画 */
void ShowMinScreen::StartAnimation()
{
    AnimTimer->start(BackGroundAnimationInterval);
}

/* 停止动画 */
void ShowMinScreen::StopAnimation()
{
    AnimTimer->stop();
    currBackgroundIndex = 0;
    currBackground = backgroundPixmap;
    update();
}

void ShowMinScreen::SetImage(cv::Mat leftImg, cv::Mat rightImg, int width, int height)
{
    leftLabel->setPixmap(QPixmap::fromImage(QImage(leftImg.data, width, height, leftImg.step, QImage::Format_RGB888)));
    rightLabel->setPixmap(QPixmap::fromImage(QImage(rightImg.data, width, height, rightImg.step, QImage::Format_RGB888)));
}

void ShowMinScreen::setScreen(int index, int width, int height)
{
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.size() == 1) {
        qWarning() << "Only one screen available, cannot set geometry";
        return;
    }
    auto screen = screens.at(index);
    setGeometry(screen->geometry().x(), screen->geometry().y(), width, height);
    show();
}

void ShowMinScreen::SetImage(const QPixmap& pixmap1, const QPixmap& pixmap2, int width, int height)
{
    // 更新显示图片
    if (!pixmap1.isNull()) {
        leftLabel->setPixmap(pixmap1.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        leftLabel->clear();
    }
    if (!pixmap2.isNull()) {
        rightLabel->setPixmap(pixmap2.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        rightLabel->clear();
    }

    update(); // 更新显示
}

void ShowMinScreen::SetLeftLabelImage(const QPixmap& pixmap1, int width, int height)
{
    if (!pixmap1.isNull()) {
        leftLabel->setPixmap(pixmap1.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        leftLabel->clear();
    }

    update(); // 更新显示
}

void ShowMinScreen::SetRightLabelImage(const QPixmap& pixmap2, int width, int height)
{
    if (!pixmap2.isNull()) {
        rightLabel->setPixmap(pixmap2.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        rightLabel->clear();
    }

    update(); // 更新显示
}

/* 合并图片 */
QPixmap ShowMinScreen::MergeImage(QPixmap& src, QPixmap& overlay)
{
    if (src.isNull() && overlay.isNull())
    {
        return QPixmap();
    }
    else if (src.isNull())
    {
        return overlay;
    }
    else if (overlay.isNull())
    {
        return src;
    }

    // 创建一个空白的 QPixmap
    QPixmap combinedPixmap(src.size());
    combinedPixmap.fill(Qt::transparent);

    QPainter painter(&combinedPixmap);

    // 将 overlay 叠加到 src 上
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, src);
    painter.drawPixmap(0, 0, overlay);
    painter.end();
    return combinedPixmap;
}

/* 返回合并后的图片的数组 */
QVector<QPixmap> ShowMinScreen::MergeImages(QPixmap& src, QVector<QPixmap>& overlays)
{
    QVector<QPixmap> results;
    if (src.isNull() && overlays.empty())
    {
        return results;
    }

    for (auto& overlay : overlays)
    {
        results.append(MergeImage(src, overlay));
    }
    return results;
}

// 绘制背景图片
void ShowMinScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    // 检查图片是否加载成功
    if (!currBackground.isNull()) {
        // 绘制背景图，使用窗口的大小
        painter.drawPixmap(0, 0, BackGroundWidth, BackGroundHeight, currBackground);
    }
}