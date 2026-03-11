#include "showMovie.h"

ShowMovie::ShowMovie()
{}

ShowMovie::~ShowMovie()
{
    delete player;
    delete videoWidget;
}

void ShowMovie::setVideoFile(const QString& filePath)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    // 设置视频输出到 QVideoWidget
    player->setVideoOutput(videoWidget);

    // 获取显示器
    QList<QScreen*> screens = QApplication::screens();
    if (screens.size() > 1) {
        // 获取第二个显示器的几何位置
        QRect screenGeometry = screens[2]->geometry();\
            this->resize(screenGeometry.size());
        this->move(screenGeometry.topLeft());
    }

    if (filePath.isEmpty()) {
        qDebug() << "No video file path provided!";
        return;
    }

    // 设置视频文件并播放
    player->setMedia(QUrl::fromLocalFile(filePath));
    player->play();

    // 显示视频窗口
    layout->addWidget(videoWidget);
    setLayout(layout);
    show();
}

