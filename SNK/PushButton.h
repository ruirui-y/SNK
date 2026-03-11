#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QPropertyAnimation>

class PushButton : public QPushButton
{
    Q_OBJECT

public:
    // 构造函数，接收三张图片的路径作为参数
    PushButton(const QString& normalImagePath,
        const QString& hoverImagePath,
        const QString& pressedImagePath,
        QWidget* parent = nullptr);
    ~PushButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QPixmap m_normalImage;                                  // 普通状态图片
    QPixmap m_hoverImage;                                   // 悬停状态图片
    QPixmap m_pressedImage;                                 // 按下状态图片
    QPropertyAnimation* m_animation;                        // 动画对象
    bool m_isAnimating;                                     // 是否正在执行动画

    void startAnimation();                                  // 启动放大动画
    void resetAnimation();                                  // 启动缩小动画
};

#endif // PUSHBUTTON_H
