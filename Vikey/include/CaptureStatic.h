#ifndef CAPTURESTATIC_H
#define CAPTURESTATIC_H
#include <QObject>

// opencv
#include <opencv2/opencv.hpp>

// qt
#include <QImage>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QScreen>
#include <QApplication>
#include <QGuiApplication>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <QDateTime>

// c++
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using namespace std;

// json
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class CaptureStatic  : public QObject
{
	Q_OBJECT

public:
	CaptureStatic(); // 不创建父类对象
	~CaptureStatic();

	// 全屏截图
	static cv::Mat CaptureFullScreen();
	// 将QImage转换为cv::Mat
	static cv::Mat QImageToCvMat(const QImage& image);
	// 调整截取的图片的大小
	static cv::Mat resizeImage(const cv::Mat& inputImage, int width = 800);

	// 根据枚举类型加载图片到容器
	static void LoadImages(map<int, cv::Mat>& result);
    // 加载图像到容器中,该函数是根据指定的文件名来加载图像
	static void LoadImages(const string& directory, map<string, cv::Mat>& temp);
	// 不根据文件名加载文件
	static void NoFileNameLoadImages(const string& directory, map<string, cv::Mat>& temp);

	// 文件重命名
    static void SetCharacterName(const QString& dirPath);
	static void renameFiles(const QString& dirPath);

	// 打印容器Map，验证是否正确
    static void LoopMap(const map<string, cv::Mat>& temp);

	// 裁剪图片保存角色头像
    static void SaveCharacterAvatarByCore(); 

	// 人物角色匹配
	static string CharacterMatch(cv::Mat& croppedImage, const map<string, cv::Mat>& temp, double threshold);
	// 判断一张图片是不是白色的
	static bool isUniformWhite(const cv::Mat& image);

	// 保存图片
	static void SaveImage(QString leftImagePath, QString rightImagePath, cv::Rect leftRect, cv::Rect rightRect);
	static void SaveImage(map<string, cv::Mat>& temp, QString leftImagePath, QString rightImagePath, cv::Rect leftRect, cv::Rect rightRect);
	static void SaveImage(cv::Mat& resultImage, QString savePath);
	static void SaveImage(QString savePath);

	// 裁剪一个文件夹的图片
	static void readImageAndResize(const QString& path, cv::Rect & rect);
	// 裁剪单张图片
	static void cropSingleImage(const QString& imagePath, const cv::Rect & rect);

	// 显示截图
	static void showScreenshot();

	static void CaptureFullBloodImage(); // 截图满血血条
	
	// 计算血条的像素
	static int CalculateBloodPiexl(cv::Mat image);

	// 判断是否是战斗场景
	static bool IsFightScene();

	// 转换cv::Mat到QPixmap
	static QPixmap CVMatToQPixmap(cv::Mat& mat);

	// 避免游戏结束的误判
	static string Loopjudge(int loopNum, cv::Rect rect, map<string, cv::Mat>& temp,int threslods);
	static void Loopjudge(int loopNum, cv::Rect leftRect, cv::Rect rightRect, map<string, cv::Mat>& leftTemp, map<string, cv::Mat>& rightTemp, string & leftName, string & rightName, int threslods);

	// 找到文件名对应的下标
	static int FindIndex(string& fileName);
	// 获取QPixmap
	static QPixmap GetQPixmap(QString& fileName, int index);

	// 获取灰色头像序列帧
	static QVector<QPixmap> GetAvatarGrayPixmaps(const QString& path, int PixmapNum);

	// 获得头像名称
	static string GetAvatarName(unordered_set<string> & nameSet);

	// 插入唯一元素
	static void insertUnique(vector<string>& vec, const string& str);

	// 叠加图片
	static QPixmap OverlayImage(QPixmap& src, QPixmap& overlay);

	// 拼接路径
	static QString CombinePath(QString& path1, int num);

	// 读取图片
	static QPixmap ReadImage(QString& path,int num);

	// 比较图片，返回最相似的图片下标
	static int CompareImage(cv::Mat& img, map<int, cv::Mat>& _map, double threshold);

	/* 加载图片资源 */
	static int LoadAnimPixmaps(QVector<QPixmap>& AnimPixmaps, const QString Path, int PixmapNum);

	/* 比较两张图片 */
	static bool CompareImage(cv::Mat& CurrCropImg, cv::Mat& PreCropImg, double threshold);

	/* 判断血条是否被遮挡 */
	static bool IsBloodCovered(cv::Mat& MaskImg);

	/* 将指定路径下的所有图片重命名为1.png,2.png,3.png... */
	static void renameAllImages(const QString& dirPath, const QString fileName);

	/* 合并两张图片 */
	static QPixmap MergeImage(QPixmap& src, QPixmap& overlay);

	/* 返回合并的图片的数组 */
	static QVector<QPixmap> MergeImages(QPixmap& src, QString Path, int PixmapNum);

	/* 返回合并的图片的数组 */
	static QVector<QPixmap> MergeImages(QPixmap& src, QVector<QPixmap>& overlays);

	// 变量
	static vector<string> avatarNameList; // 头像名称列表
    static QString jsonString; // 头像名称列表
	static int fullBloodPixelNum; // 全血像素数
};
#endif // CAPTURESTATIC_H

