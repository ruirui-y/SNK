#include "PushButton.h"
#include <QMouseEvent>
#include <QTimer>

PushButton::PushButton(const QString& normalImagePath,
    const QString& hoverImagePath,
    const QString& pressedImagePath,
    QWidget* parent)
    : QPushButton(parent),
    m_isAnimating(false)  // 初始化为未在动画中
{
    // 加载三张图片
    m_normalImage = QPixmap(normalImagePath);
    m_hoverImage = QPixmap(hoverImagePath);
    m_pressedImage = QPixmap(pressedImagePath);

    // 设置按钮大小为普通状态图片的大小
    setFixedSize(m_normalImage.size());

    // 初始化动画对象
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(300); // 动画时长
    m_animation->setEasingCurve(QEasingCurve::OutBounce); // 设置动画效果为弹跳
}

PushButton::~PushButton() {
    delete m_animation;
}

void PushButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // 根据按钮的状态绘制不同的图片
    if (isDown()) {
        painter.drawPixmap(0, 0, m_pressedImage);
    }
    else if (underMouse()) {
        painter.drawPixmap(0, 0, m_hoverImage);
    }
    else {
        painter.drawPixmap(0, 0, m_normalImage);
    }

    // 调用父类的绘制方法，绘制文本等
    QPushButton::paintEvent(event);
}

void PushButton::enterEvent(QEvent* event)
{
    QPushButton::enterEvent(event);
    update();
}

void PushButton::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event);
    update();
}

void PushButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && !m_isAnimating) {
        m_isAnimating = true;
        startAnimation();
    }
    QPushButton::mousePressEvent(event);
}

void PushButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_isAnimating) {
        resetAnimation();
    }
    QPushButton::mouseReleaseEvent(event);
}

void PushButton::startAnimation()
{
    // 获取当前按钮的几何信息
    QRect originalGeometry = geometry();

    // 放大后的目标矩形（放大1.2倍）
    QRect enlargedGeometry = originalGeometry.adjusted(
        -originalGeometry.width() * 0.1,
        -originalGeometry.height() * 0.1,
        originalGeometry.width() * 0.1,
        originalGeometry.height() * 0.1
    );

    // 设置动画：从原始尺寸到放大后的尺寸
    m_animation->setStartValue(originalGeometry);
    m_animation->setEndValue(enlargedGeometry);
    m_animation->start();

    // 延迟触发缩小动画
    QTimer::singleShot(300, [this, originalGeometry]() {
        m_animation->setStartValue(geometry());
        m_animation->setEndValue(originalGeometry);  // 恢复到原始尺寸
        m_animation->start();

        // 动画结束后重置标志
        QTimer::singleShot(300, [this]() {
            m_isAnimating = false;
            });
        });
}

void PushButton::resetAnimation()
{
    // 获取当前按钮的几何信息
    QRect originalGeometry = geometry();

    // 设置动画：恢复原始尺寸
    m_animation->setStartValue(geometry());
    m_animation->setEndValue(originalGeometry);
    m_animation->start();
}
