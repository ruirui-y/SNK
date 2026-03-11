#ifndef SHOWMOVIE_H
#define SHOWMOVIE_H

#include <QWidget>
#include <QScreen>
#include <QLayout>
#include <QVBoxLayout>
#include <QList>
#include <QApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>

class ShowMovie : public QWidget
{
    Q_OBJECT

public:
    // 构造函数和析构函数
    explicit ShowMovie();
    ~ShowMovie();

    // 设置并播放视频文件
    void setVideoFile(const QString& filePath);

private:
    QMediaPlayer* player;  // 视频播放器对象
    QVideoWidget* videoWidget;  // 视频显示窗口
};
#endif // SHOWMOVIE_H