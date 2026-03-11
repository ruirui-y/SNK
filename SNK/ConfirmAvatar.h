#ifndef CONFIRMAVATAR_H
#define CONFIRMAVATAR_H
// qt
#include <QObject>
#include <QDebug>
#include <QWidget>

// opencv
#include <opencv2/opencv.hpp>

// c++
#include <string>
#include <map>
#include <unordered_set>

// 头文件
#include "ShowMinScreen.h"
#include "ShowScreen.h"
#include "SignalRelay.h"
#include "SignalSend.h"

#include "Enum.h"
#include <QScopedPointer>

using namespace std;

class ConfirmAvatar  : public QObject
{
	Q_OBJECT
public:
	ConfirmAvatar(QObject *parent = nullptr);
	~ConfirmAvatar();

	void StartGrayTimer();										// 开始灰色动画定时器
private:
	// 函数
	void Init();

	// 截图
	void CropAvatarAndMatch();
	// 右边显示灰色图
	void ShowRightGray();
	// 左边显示灰色图
	void ShowLeftGray();

	void showDefaultBackground(); // 显示默认背景

	// 绑定槽函数
	void BindSlot();

public slots:
	void OnTimerConfirm();
	// 当游戏结束时
	void OnGameEnd();

	void showGrayAnimation();									// 播放灰色动画

signals:
	void endGames();											// 游戏结束,再来一局
	void endAvatrSuccess(int index);							// 游戏结束最后胜利的角色

public:
	QVector<QPixmap> GrayPixmaps;								// 灰色动画图片
	QVector<QPixmap> GrayPixmapsOverlay;						// 灰色动画图片叠加层
	int GrayIndex = 0;											// 灰色动画图片索引
	GrayLabelType _GrayLabelType = NoneGrayLabel;				// 哪个标签显示灰色动画

private:
	// 变量
	map<string, cv::Mat> leftAvatarMap;							// 用来匹配的左边角色资源
	map<string, cv::Mat> rightAvatarMap;						// 用来匹配的右边角色资源
																
	cv::Rect leftRect;											// 左边角色的位置
	cv::Rect rightRect;											// 右边角色的位置
																
	vector<string> leftMatchingNames;							// 已经参赛的角色列表，左边
	vector<string> rightMatchingNames;							// 已经参赛的角色列表，右边
																
	// 显示器													 
	QScopedPointer<ShowScreen> showScreen1;						// 显示屏幕类
	QScopedPointer<ShowScreen> showScreen2;						// 显示屏幕类
	ShowMinScreen* showMinScreen;								// 最小屏幕类
																
	string lastLeftMatchingName;								// 左边角色名称
	string lastRightMatchingName;								// 右边角色名称
																
	// 图片的路径							`						
	QString leftMinScreenPath;									
	QString rightMinScreenPath;									
	QString leftMinBlackWhitePath;								
	QString rightMinBlackWhitePath;								
																
	// 游戏中转变量指针											 
	QSharedPointer<SignalRelay> m_SignalRelay;					// 变量中转类
	// 游戏中转信号指针											 
	QSharedPointer<SignalSend> m_SignalSend;					// 信号发送类
																
	QTimer* timer;												// 定时器
	QTimer* GrayTimer;											// 灰色动画定时器
};
#endif // CONFIRMAVATAR_H