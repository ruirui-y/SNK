#include "CaptureStatic.h"
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include "Enum.h"

#define WHITI_SCREEN_THRESHOLD                                    200   // 白色屏幕阈值
#define WHITI_SCREEN_ELEMENT_PERCENTAGE_THRESHOLD                 90   // 白色屏幕元素占比阈值


// 静态变量初始化

// 头像名称列表
vector<string> CaptureStatic::avatarNameList = { "吉斯·霍华德", "比利·凯恩", "山崎龙二", "洛克·霍华德",
        "矢吹真吾", "金家潘", "碧·珍妮特", "牙刀",
        "希尔薇·波拉·波拉", "娜吉德", "暴走七枷社", "暴走夏尔美",
        "堕珑", "四条雏子", "暴走克里斯", "霸王丸",
        "高尼茨", "娜可露露", "妲丽·刀伽", "究极卢卡尔",
        "哈迪兰", "德罗丽斯", "伊丝拉", "薇普",
        "马克西马", "凯", "神鸟千鹤", "八神庵",
        "草薙京", "库克里", "伊丽莎白·布兰克托什", "阿修·克里门森",
        "二阶堂红丸", "明天君", "瞬影", "特瑞·博加德",
        "安迪·博加德", "东丈", "坂崎良", "罗伯特·加西亚",
        "琼", "七枷社", "夏尔美", "克里斯",
        "麻宫雅典娜", "不知火舞", "坂崎百合", "莉安娜·哈迪兰",
        "拉尔夫·琼斯", "克拉克·史迪尔", "梁", "温妮莎",
        "布鲁·玛丽", "安琪尔", "库拉·戴尔蒙多", "柯隆",
        "暴龙王", "拉蒙", "安东诺夫","60","61"};

// 头像名称列表
QString CaptureStatic::jsonString = R"({
    "characters": [
        "吉斯·霍华德", "比利·凯恩", "山崎龙二", "洛克·霍华德",
        "矢吹真吾", "金家潘", "碧·珍妮特", "牙刀",
        "希尔薇·波拉·波拉", "娜吉德", "暴走七枷社", "暴走夏尔美",
        "堕珑", "四条雏子", "暴走克里斯", "霸王丸",
        "高尼茨", "娜可露露", "妲丽·刀伽", "究极卢卡尔",
        "哈迪兰", "德罗丽斯", "伊丝拉", "薇普",
        "马克西马", "凯", "神鸟千鹤", "八神庵",
        "草薙京", "库克里", "伊丽莎白·布兰克托什", "阿修·克里门森",
        "二阶堂红丸", "明天君", "瞬影", "特瑞·博加德",
        "安迪·博加德", "东丈", "坂崎良", "罗伯特·加西亚",
        "琼", "七枷社", "夏尔美", "克里斯",
        "麻宫雅典娜", "不知火舞", "坂崎百合", "莉安娜·哈迪兰",
        "拉尔夫·琼斯", "克拉克·史迪尔", "梁", "温妮莎",
        "布鲁·玛丽", "安琪尔", "库拉·戴尔蒙多", "柯隆",
        "暴龙王", "拉蒙", "安东诺夫"
    ]
})";

int CaptureStatic::fullBloodPixelNum = 3549;  // 全血像素数量

// ********************************************************************************************************************************************

// 静态函数定义
CaptureStatic::CaptureStatic()
{}

CaptureStatic::~CaptureStatic()
{
}

// 截取全屏
cv::Mat CaptureStatic::CaptureFullScreen()
{
    // 获取整个屏幕
    QScreen* screen = QGuiApplication::primaryScreen();
    // 截取整个屏幕
    QPixmap FullImage = screen->grabWindow(0);
    // 转换为QImage
    QImage screenshot = FullImage.toImage();

    // 然后将QImage转换为cv::Mat形式
    cv::Mat mat = QImageToCvMat(screenshot);

    // 调整图片的大小
    cv::Mat image = resizeImage(mat);
    return image;
}

// 调整图片的大小
cv::Mat CaptureStatic::resizeImage(const cv::Mat& inputImage, int width) {
    // 检查输入图像是否为空
    if (inputImage.empty()) {
        qDebug() << "captureStatic::resizeImage: Input image is empty";
        return cv::Mat(); // 返回空的 cv::Mat
    }

    // 获取图像的高度和宽度
    int height = inputImage.rows;
    int orignalWidth = inputImage.cols;

    // 计算新的高度,保持原先的纵横比
    double aspectRatio = static_cast<double>(height) / orignalWidth;
    int newHeight = static_cast<int>(width * aspectRatio);

    cv::Mat resizedImage;
    // 调整图像大小
    cv::resize(inputImage, resizedImage, cv::Size(width, newHeight));

    return resizedImage;
}

// 将 QImage 转换为 cv::Mat
cv::Mat CaptureStatic::QImageToCvMat(const QImage& image) {
    // 确保 QImage 是有效的
    if (image.isNull()) {
        qDebug() << "captureStatic::QImageToCvMat: Input QImage is null";
        return cv::Mat();
    }

    cv::Mat mat;
    switch (image.format()) {
    case QImage::Format_RGB32:  // RGBA
        mat = cv::Mat(image.height(), image.width(), CV_8UC4,
            const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        break;

    case QImage::Format_RGB888:  // RGB
        mat = cv::Mat(image.height(), image.width(), CV_8UC3,
            const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;

    case QImage::Format_Indexed8:  // Grayscale
        mat = cv::Mat(image.height(), image.width(), CV_8UC1,
            const_cast<uchar*>(image.bits()), image.bytesPerLine());
        break;

    case QImage::Format_Grayscale8:  // Grayscale (Alpha not included)
        mat = cv::Mat(image.height(), image.width(), CV_8UC1,
            const_cast<uchar*>(image.bits()), image.bytesPerLine());
        break;

    default:
        qDebug() << "captureStatic::QImageToCvMat: Unsupported QImage format:" << image.format();
        break;
    }

    return mat;
}

void CaptureStatic::LoadImages(map<int, cv::Mat>& result)
{
    QString currDir = QDir::currentPath();
    currDir += "/MenuButton/Double";

    QDir dir(currDir);
    if (dir.exists())
    {
        QString imgPath1 = currDir + "/1.png";
        QString imgPath2 = currDir + "/2.png";
        QString imgPath3 = currDir + "/3.png";
        QString imgPath4 = currDir + "/4.png";
        QString imgPath5 = currDir + "/5.png";

        cv::Mat img1 = cv::imread(imgPath1.toStdString());
        cv::Mat img2 = cv::imread(imgPath2.toStdString());
        cv::Mat img3 = cv::imread(imgPath3.toStdString());
        cv::Mat img4 = cv::imread(imgPath4.toStdString());
        cv::Mat img5 = cv::imread(imgPath5.toStdString());

        result.insert({ EndMenuButtonType::DoubleAgainFight, img1 });
        result.insert({ EndMenuButtonType::DoubleSelectSequence, img2 });
        result.insert({ EndMenuButtonType::DoubleSelectMember, img3 });
        result.insert({ EndMenuButtonType::DoubleFightSet, img4 });
        result.insert({ EndMenuButtonType::DoubleMainMenu, img5 });
    }

    currDir = QDir::currentPath();
    currDir += "/MenuButton/Single";

    dir.setPath(currDir);
    if (dir.exists())
    {
        QString imgPath1 = currDir + "/1.png";
        QString imgPath2 = currDir + "/2.png";
        QString imgPath3 = currDir + "/3.png";
        QString imgPath4 = currDir + "/4.png";
        QString imgPath5 = currDir + "/5.png";

        cv::Mat img1 = cv::imread(imgPath1.toStdString());
        cv::Mat img2 = cv::imread(imgPath2.toStdString());
        cv::Mat img3 = cv::imread(imgPath3.toStdString());
        cv::Mat img4 = cv::imread(imgPath4.toStdString());
        cv::Mat img5 = cv::imread(imgPath5.toStdString());

        result.insert({ EndMenuButtonType::SingleAgainFight, img1 });
        result.insert({ EndMenuButtonType::SingleSelectSequence, img2 });
        result.insert({ EndMenuButtonType::SingleSelectMember, img3 });
        result.insert({ EndMenuButtonType::SingleFightSet, img4 });
        result.insert({ EndMenuButtonType::SingleMainMenu, img5 });
    }

    currDir = QDir::currentPath();
    currDir += "/MenuButton/ModeSelect";
    dir.setPath(currDir);
    if (dir.exists())
    {
        QString imgPath1 = currDir + "/1.png";
        QString imgPath2 = currDir + "/2.png";

        cv::Mat img1 = cv::imread(imgPath1.toStdString());
        cv::Mat img2 = cv::imread(imgPath2.toStdString());

        result.insert({ EndMenuButtonType::GameModeDouble, img1 });
        result.insert({ EndMenuButtonType::GameModeSingle, img2 });
    }
    qDebug() << "result.size() = " << result.size();
}

// 加载图像到容器中
void CaptureStatic::LoadImages(const string& directory, map<string, cv::Mat>& temp)
{
    // 获取绝对路径
    QString currentDir = QDir::currentPath();
    currentDir.replace("/", "\\");
    currentDir += "\\" + QString::fromUtf8(directory.c_str());
    string currentDirStr = currentDir.toStdString();
   /* qDebug() << "captureStatic::LoadImages currentDir = " << currentDir;*/

    // 必须使用绝对路径
    QDir dir(currentDir);  // 使用Qt的QDir
    if (dir.exists())  // 确保目录存在
    {
        for (auto& name : avatarNameList)
        {
            // 拼接目录
            string imagePath = currentDirStr + "\\" + name + ".png";
            // 读取图像文件
            cv::Mat image = cv::imread(imagePath);
            if (!image.empty())  // 确保图像文件存在
            {
                temp[name] = image;  // 加载图像到容器中
                // qDebug() << QString::fromLocal8Bit("captureStatic::LoadImages 加载图像成功: ") << QString::fromLocal8Bit(imagePath.c_str());
            }
            else
            {
                // qDebug() << QString::fromLocal8Bit("captureStatic::LoadImages 加载图像失败: ") << QString::fromLocal8Bit(imagePath.c_str());
            }
        }
    }
    else
    {
        qDebug() << "capture::LoadImages Directory not found: " << QString::fromLocal8Bit(directory.c_str());
    }

    /*qDebug() << "captureStatic::LoadImages temp.size() = " << temp.size();*/
}

// 加载图像到容器中（无文件名）
void CaptureStatic::NoFileNameLoadImages(const string& directory, map<string, cv::Mat>& temp)
{
    // 获取绝对路径
    QString currentDir = QDir::currentPath();
    currentDir.replace("/", "\\");
    currentDir += "\\" + QString::fromUtf8(directory.c_str());
    string currentDirStr = currentDir.toStdString();
    //qDebug() << "captureStatic::LoadImages currentDir = " << currentDir;
    // 必须使用绝对路径
    QDir dir(currentDir);                                                                       // 使用Qt的QDir
    if (!dir.exists()) {                                                                        // 检查目录是否存在
        qDebug() << "Directory does not exist: " << currentDir;
        return;
    }

    // 设置过滤器，只读取常见图片文件
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tiff";                           // 支持的图片格式
    dir.setNameFilters(filters);

    // 获取目录下所有文件名
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for (const QFileInfo& fileInfo : fileList) {
        std::string fileName = fileInfo.fileName().toStdString();                               // 文件名
        
        size_t dotIndex = fileName.find_last_of('.');                                           // 找到最后一个点的位置
        if (dotIndex != std::string::npos) {
            fileName = fileName.substr(0, dotIndex);  
            //qDebug() << "filename = " << QString(fileName.c_str());                           // 去掉后缀
        }

        std::string filePath = fileInfo.absoluteFilePath().toStdString();                       // 文件绝对路径
        
        // 使用OpenCV加载图像
        cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);                             // 加载原始格式图像
        if (image.empty()) {
            qDebug() << "Failed to load image: " << QString::fromStdString(filePath);
            continue;
        }

        // 存储到容器中
        temp[fileName] = image;
    }

   /* qDebug() << "Loaded " << temp.size() << " images from directory: " << currentDir;*/
}

// 给文件的角色重命名
void CaptureStatic::SetCharacterName(const QString& dirPath)
{
    // 解析 JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!jsonDoc.isObject()) {
        qDebug() << "无效的 JSON 数据";
        return;
    }

    // jsonDoc.object()提取QJsonObject类型的根对象
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray charactersArray = jsonObj["characters"].toArray();// charactersArray[0]："吉斯·霍华德"

    // 打开目录并读取所有图片文件
    QDir dir(dirPath);
    if (!dir.exists()) {
        qDebug() << "指定的目录不存在：" << dirPath;
        return;
    }

    dir.setNameFilters(QStringList() << "*.png" << "*.jpg" << "*.jpeg");  // 仅匹配图像文件
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);// 获取符合过滤条件的文件列表

    // 按照修改日期递增排序
    std::sort(fileList.begin(), fileList.end(), [](const QFileInfo& a, const QFileInfo& b) {
        return a.lastModified() < b.lastModified();
        });

    // 确保文件数量不超过角色数量
    int count = std::min(fileList.size(), charactersArray.size());
    for (int i = 0; i < count; ++i) {
        QFileInfo fileInfo = fileList.at(i);// 获取索引为i的文件的详细信息
        QString oldFilePath = fileInfo.absoluteFilePath();// 获取当前文件的绝对路径
        QString characterName = charactersArray.at(i).toString();// 获取对应的名字数组当前i的姓名
        QString newFileName = characterName + ".png";  // 新文件名（带扩展名）
        QString newFilePath = dirPath + "/" + newFileName;// 获取新的文件路径

        // 重命名文件
        if (QFile::rename(oldFilePath, newFilePath)) {
            qDebug() << "文件重命名成功：" << oldFilePath << " -> " << newFilePath;
        }
        else {
            qDebug() << "文件重命名失败：" << oldFilePath;
        }
    }
}

// 重命名为0,1****10.png格式的文件
void CaptureStatic::renameFiles(const QString& dirPath)
{
    // 获取当前工作目录
    QString currentDir = QDir::currentPath();
    currentDir.replace("/", "\\");
    currentDir += "\\" + dirPath;
    std::string currentDirStr = currentDir.toStdString();
    qDebug() << "captureStatic::LoadImages currentDir = " << currentDir;

    // 使用绝对路径
    QDir dir(currentDir);  // 使用Qt的QDir
    if (dir.exists()) {  // 确保目录存在
        int index = 0;
        for (const auto& name : avatarNameList) {
            // 拼接完整路径
            std::string imagePath = currentDirStr + "\\" + name + ".png";
            QString imagePathQString = QString::fromLocal8Bit(imagePath.c_str());

            // 检查文件是否存在
            if (QFile::exists(imagePathQString)) {
                // 拼接新的文件名
                QString newFileName = QString::number(index) + ".png";
                QString newFilePath = currentDir + "\\" + newFileName;

                // 重命名文件
                bool renamed = QFile::rename(imagePathQString, newFilePath);
                if (renamed) {
                    qDebug() << "Renamed:" << imagePathQString << "to" << newFilePath;
                }
                else {
                    qDebug() << "Failed to rename:" << imagePathQString;
                }
            }
            else {
                qDebug() << "File does not exist:" << imagePathQString;
            }
            ++index;  // 增加下标
        }
    }
    else {
        qDebug() << "Directory does not exist:" << currentDir;
    }
}

// 遍历Map并打印
void CaptureStatic::LoopMap(const map<string, cv::Mat>& temp)
{
    qDebug() << "capture::LoopMap temp.size() = " << temp.size();
    for (auto& i : temp)
    {
        qDebug() << QString::fromLocal8Bit(i.first.c_str());
    }
}

// 裁剪图片保存角色头像
void CaptureStatic::SaveCharacterAvatarByCore()
{
    // 截图
    cv::Mat image = CaptureFullScreen();

    // 裁剪
    //cv::Rect firstCharactor(45, 356, 74 - 45, 386 - 356);
    /*cv::Rect firstCharactor(383, 21, 417 - 383, 54 - 21);*/
    cv::Rect firstCharactor(216, 196, 584 - 216, 20);

    // 保存图片到指定路径
    cv::Mat firstImage = image(firstCharactor);

    // 获取当前时间戳
    QString timeStamp = QDateTime::currentDateTime().toString("hhmmss");
    // 保存图片到指定路径
    //QString path = "D:/KofImageSave/Avatar";
    QString path = "D:/AAA";
    QDir().mkpath(path); // 创建保存目录
    cv::imwrite((path + "/" + timeStamp + ".png").toStdString(), firstImage);

    //qDebug() << "保存成功";
}

// 人物角色图片匹配
string CaptureStatic::CharacterMatch(cv::Mat& croppedImage, const map<string, cv::Mat>& temp, double threshold)
{
    string bestMatchKey;
    double bestMatchScore = 0.0;
    
    bool isWhite = isUniformWhite(croppedImage);
    if (isWhite)
    {
        return "white";
    }

    // 遍历模板匹配
    for (auto& pair : temp)
    {
        const string key = pair.first;
        const cv::Mat templateImage = pair.second;

        // 使用模板匹配
        cv::Mat result;
        cv::matchTemplate(croppedImage, templateImage, result, cv::TM_CCOEFF_NORMED);

        // 获取匹配的最大值
        double maxVal;
        cv::minMaxLoc(result, nullptr, &maxVal);

        // 检查是否是最佳匹配
        if (maxVal > bestMatchScore)
        {
            bestMatchScore = maxVal;
            bestMatchKey = key;
        }
    }

    // 设置匹配阈值
    if (bestMatchScore > threshold)
    {
        //cv::Mat bestMatchImage = temp.find(bestMatchKey)->second;
        //QDir().mkpath("D:/MatchSuccess");  // 创建保存目录
        //CaptureStatic::SaveImage(croppedImage, "D:/MatchSuccess");
        //CaptureStatic::SaveImage(bestMatchImage, "D:/MatchSuccess");
        qDebug() << QString::fromLocal8Bit("匹配成功的阈值 = %d", bestMatchScore);
        return bestMatchKey;
    }
    else
    {
        // 找不到的时候才去保存照片
        //CaptureStatic::SaveImage(croppedImage, "D:/MatchFailed");
    }
    qDebug() << QString::fromLocal8Bit("匹配失败的阈值 = %d", bestMatchScore);
    return "";// 如果没有超过匹配阈值就返回空字符串
}

bool CaptureStatic::isUniformWhite(const cv::Mat& image)
{
    // 检查尺寸是否符合要求
    if (image.rows != 450 || image.cols != 800) {
        std::cerr << "Image size is not 800x450." << std::endl;
        return false;
    }
    // 遍历每个像素
    int Elements = image.rows * image.cols;
    int WhiteElements = 0;
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(row, col);
            if (pixel[0] > WHITI_SCREEN_THRESHOLD ||
                pixel[1] > WHITI_SCREEN_THRESHOLD ||
                pixel[2] > WHITI_SCREEN_THRESHOLD)
            {
                WhiteElements++;
            }
        }
    }

    // 计算白色像素占比
    int WhiteElementsPercentage = WhiteElements / Elements * 100;
    if (WhiteElementsPercentage > WHITI_SCREEN_ELEMENT_PERCENTAGE_THRESHOLD)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 保存截图
void CaptureStatic::SaveImage(QString leftImagePath, QString rightImagePath, cv::Rect leftRect, cv::Rect rightRect)
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();

    // 获取当前时间戳
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    // 裁剪左边的人物头像并保存
    cv::Mat leftImage = image(leftRect);
    QDir().mkpath(leftImagePath);  // 创建保存目录
    cv::imwrite((leftImagePath + "/left_character_" + timestamp + ".png").toStdString(), leftImage);

    // 裁剪右边的人物头像并保存
    cv::Mat rightImage = image(rightRect);
    QDir().mkpath(rightImagePath);  // 创建保存目录
    cv::imwrite((rightImagePath + "/right_character_" + timestamp + ".png").toStdString(), rightImage);

    qDebug() << QString::fromLocal8Bit("保存成功：") << timestamp;
}

// 裁剪战斗中的人物图像并保存
void CaptureStatic::SaveImage(map<string, cv::Mat>& temp, QString leftImagePath, QString rightImagePath, cv::Rect leftRect, cv::Rect rightRect)
{
    for (auto& name : temp)
    {
        // 裁剪左边的人物头像并保存
        cv::Mat leftImage = name.second(leftRect);
        QDir().mkpath(leftImagePath);  // 创建保存目录
        cv::imwrite((leftImagePath.toStdString() + "/" + name.first + ".png"), leftImage);
    
        // 裁剪右边的人物头像并保存
        cv::Mat rightImage = name.second(rightRect);
        QDir().mkpath(rightImagePath);  // 创建保存目录
        cv::imwrite((rightImagePath.toStdString() + "/" + name.first + ".png"), rightImage);
    }
}

// 保存图片
void CaptureStatic::SaveImage(cv::Mat& resultImage, QString savePath)
{
    // 获取当前时间戳
    QString timestamp = QDateTime::currentDateTime().toString("MM_DD_hh_mm_ss_zzz");

    QDir().mkpath(savePath);  // 创建保存目录
    cv::imwrite((savePath + "/" + timestamp + ".png").toStdString(), resultImage);
}

// 简单的保存全屏截图
void CaptureStatic::SaveImage(QString savePath)
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();
    // 获取当前时间戳
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QDir().mkpath(savePath);  // 创建保存目录
    cv::imwrite((savePath + "/" + timestamp + ".png").toStdString(), image);
}

// 重新调整图片的大小
void CaptureStatic::readImageAndResize(const QString& path, cv::Rect& rect)
{
    qDebug() << "capture::readImageAndResize path = " << path;
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "指定的目录不存在：" << path;
        return;
    }
    // 获取目录下的所有图片文件
    QStringList filters = { "*.png"}; // 支持的图片格式
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
    qDebug() << "capture::readImageAndResize fileList.size() = " << fileList.size();

    // 遍历图片文件
    for (const QFileInfo& fileInfo : fileList) {
        QString filePath = fileInfo.absoluteFilePath();
        qDebug() << "capture::readImageAndResize filePath = " << filePath;
        // 读取图片
        cv::Mat image = cv::imread(filePath.toStdString());
        if (image.empty()) {
            qDebug() << "Failed to read image: " << filePath;
            return;
        }
        else
        {
            qDebug() << "capture::readImageAndResize: read image success  ";
            qDebug() << "capture::readImageAndResize: image.cols = " << image.cols;
            qDebug() << "capture::readImageAndResize: image.rows = " << image.rows;
        }

        // 裁剪图片
        cv::Mat croppedImage = image(rect);
        // 保存图片
        if (!cv::imwrite(filePath.toStdString(), croppedImage)) {
            std::cerr << "Failed to save image: " << filePath.toStdString() << std::endl;
        }
        else {
            std::cout << "Processed and saved image: " << filePath.toStdString() << std::endl;
        }
    }
}

// 裁剪单张图片
void CaptureStatic::cropSingleImage(const QString& imagePath, const cv::Rect& rect)
{
    // 读取单张图片
    cv::Mat image = cv::imread(imagePath.toStdString());
    if (image.empty()) {
        qDebug() << "Failed to read image: " << imagePath;
        return;
    }
    else
    {
        qDebug() << "capture::readImageAndResize: read image success  ";
    }

    // 裁剪图片
    cv::Mat croppedImage = image(rect);
    // 保存图片
    if (!cv::imwrite(imagePath.toStdString(), croppedImage)) {
        std::cerr << "Failed to save image: " << imagePath.toStdString() << std::endl;
    }
    else {
        std::cout << "Processed and saved image: " << imagePath.toStdString() << std::endl;
    }
}

void CaptureStatic::showScreenshot()
{
    // 获取屏幕截图
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qDebug() << "Screen is null";
        return;
    }
    // qDebug() << "Getting the screenshot...";

    QPixmap originalPixmap = screen->grabWindow(0);  // 截取整个屏幕
    if (originalPixmap.isNull()) {
        qDebug() << "Failed to grab screen: originalPixmap is null";
        return;
    }

    QImage screenshot = originalPixmap.toImage();     // 转换为 QImage
    if (screenshot.isNull()) {
        qDebug() << "Failed to convert to QImage: screenshot is null";
        return;
    }

    // 将 QImage 转换为 cv::Mat 格式
    cv::Mat mat = CaptureStatic::QImageToCvMat(screenshot);
    if (mat.empty()) {
        qDebug() << "Failed to convert QImage to cv::Mat: mat is empty";
        return;
    }

    // 调整图片大小
    mat = CaptureStatic::resizeImage(mat);

    // 使用 OpenCV 显示截图
    cv::imshow("Screenshot", mat);
    cv::waitKey(0);

    // qDebug() << "Screenshot displayed successfully";
}

// 截图满血血条
void CaptureStatic::CaptureFullBloodImage()
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();

    // 定义裁剪区域的坐标
    cv::Rect leftBlood(67, 24, 367 - 67, 37 - 24);
    cv::Rect rightBlood(432, 24, 732 - 432, 37 - 24);

    // 裁剪图片并显示
    cv::Mat leftImage = image(rightBlood);
    cv::imshow("Blood", leftImage);

    // cv::imshow("image",image);
}

// 判断是否在战斗场景
bool CaptureStatic::IsFightScene()
{
    // 获取全屏
    cv::Mat image = CaptureStatic::CaptureFullScreen();

    // 定义裁剪区域的坐标
    cv::Rect leftBlood(67, 24, 367 - 67, 37 - 24);
    cv::Rect rightBlood(432, 24, 732 - 432, 37 - 24);

    cv::Mat leftImage = image(leftBlood); // 左边的血条
    cv::Mat rightImage = image(rightBlood); // 右边的血条

    // 计算血条的像素
    int leftBloodPiexlNum = CaptureStatic::CalculateBloodPiexl(leftImage);
    int rightBloodPiexlNum = CaptureStatic::CalculateBloodPiexl(rightImage);

    /*qDebug() << "CaptureStatic::IsFightScene leftBloodPiexlNum = " << leftBloodPiexlNum;
    qDebug() << "CaptureStatic::IsFightScene rightBloodPiexlNum = " << rightBloodPiexlNum;
    qDebug() << "CaptureStatic::IsFightScene fullBloodPiexlNum = " << fullBloodPixelNum;*/

    // 计算血条的百分比
    int leftBloodRate = 0;
    int rightBloodRate = 0;
    if (fullBloodPixelNum != 0)
    {
        leftBloodRate = (leftBloodPiexlNum * 25) / fullBloodPixelNum;
        // qDebug() << "CaptureStatic::IsFightScene leftBloodRate = " << leftBloodRate;
        rightBloodRate = (rightBloodPiexlNum * 25) / fullBloodPixelNum;
        // qDebug() << "CaptureStatic::IsFightScene rightBloodRate = " << rightBloodRate;
        if (leftBloodRate > 20 || rightBloodRate > 20)
            return true;
    }
    return false;
}

// 将 cv::Mat 转换为 QPixmap
QPixmap CaptureStatic::CVMatToQPixmap(cv::Mat& mat)
{
    // 检查 Mat 是否为空
    if (mat.empty())
    {
        return QPixmap();
    }

    // 转换 Mat 数据到 QImage
    cv::Mat outputMat;

    // 将 BGR 图像转换为 BGRA 图像（添加透明通道）
    if (mat.type() == CV_8UC3) {
        // 创建一个新的 BGRA 图像
        cv::Mat bgraMat(mat.rows, mat.cols, CV_8UC4);
        cv::cvtColor(mat, bgraMat, cv::COLOR_BGR2RGBA);  // 转换为 BGRA 格式
        // 遍历图像并将背景颜色设为透明
        for (int y = 0; y < bgraMat.rows; ++y) {
            for (int x = 0; x < bgraMat.cols; ++x) {
                cv::Vec4b& pixel = bgraMat.at<cv::Vec4b>(y, x);
                if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0) {
                    pixel[3] = 0;  // 将背景的 Alpha 通道设置为 0（透明）
                }
            }
        }
        // 保存输出图像
        outputMat = bgraMat;
    }
    else
    {
        qWarning("CVMatToQPixmap: Unsupported cv::Mat format");
        return QPixmap();
    }

    // 将处理后的图像转换为 QImage
    QImage image(outputMat.data, outputMat.cols, outputMat.rows, outputMat.step, QImage::Format_RGBA8888);


    // 将 QImage 转换为 QPixmap
    return QPixmap::fromImage(image);
}

// 避免误判
string CaptureStatic::Loopjudge(int loopNum, cv::Rect rect, map<string, cv::Mat>& temp, int threslods)
{
    int i = 0;
    string name;
    while (i < loopNum)
    {
        cv::Mat image = CaptureStatic::CaptureFullScreen();
        cv::Mat cropImage = image(rect);
        name = CaptureStatic::CharacterMatch(cropImage, temp, threslods);
        i++;
    }
    return name;
}

// 避免误判
void CaptureStatic::Loopjudge(int loopNum, cv::Rect leftRect, cv::Rect rightRect, map<string, cv::Mat>& leftTemp, map<string, cv::Mat>& rightTemp, string& leftName, string& rightName, int threslods)
{
    int i = 0;
    while (i < loopNum)
    {
        cv::Mat image = CaptureStatic::CaptureFullScreen();
        cv::Mat leftImage = image(leftRect);
        cv::Mat rightImage = image(rightRect);
        leftName = CaptureStatic::CharacterMatch(leftImage, leftTemp, threslods);
        rightName = CaptureStatic::CharacterMatch(rightImage, rightTemp, threslods);
        QThread::msleep(500); // 延时1秒
        i++;
    }
}

// 找到人物角色对应的下标
int CaptureStatic::FindIndex(string& fileName)
{
    int index = 0;
    for (index = 0; index < avatarNameList.size(); index++)
    {
        if (fileName == avatarNameList[index])
        {
            return index;
        }
    }
    return -1;                                                      // 没找到返回-1
}

// 获取头像的 QPixmap
QPixmap CaptureStatic::GetQPixmap(QString& fileName, int index)
{
    // 拼接路径
    QString path = fileName + "/" + QString::number(index) + ".png";
    QPixmap pixmap(path);
    if (pixmap.isNull())
    {
        qDebug() << "Failed to load image: " << path;
        return QPixmap();
    }
    return pixmap;
}

/* 获取左边角色的灰色头像序列帧 */
QVector<QPixmap> CaptureStatic::GetAvatarGrayPixmaps(const QString& path, int PixmapNum)
{
    QString imagePath = path + "/" + QString::number(PixmapNum) + "/";
    qDebug() << "imagePath " << imagePath;
    QVector<QPixmap> pixmaps;

    QDir dir(imagePath);
    if (dir.exists())
    {
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg";
        dir.setNameFilters(filters);

        for (auto file : dir.entryInfoList())
        {
            pixmaps.append(QPixmap(file.absoluteFilePath()));
        }
    }

    qDebug() << "Loading avatar gray pixmaps... PixmapNum = " << pixmaps.size();
    return pixmaps;
}


// 获取头像的名字
string CaptureStatic::GetAvatarName(unordered_set<string>& nameSet)
{
    auto it = nameSet.end();
    
    return string();
}

// 插入唯一元素
void CaptureStatic::insertUnique(vector<string>& vec, const string& str)
{
    if (find(vec.begin(), vec.end(), str) == vec.end())
    {
        vec.push_back(str);
    }
}

// 叠加图片
QPixmap CaptureStatic::OverlayImage(QPixmap& src, QPixmap& overlay)
{
    if (src.isNull())
    {
        return QPixmap();
    }
    QPixmap combinedPixmap = src;
    if (!overlay.isNull())
    {
        QPainter painter(&combinedPixmap);
        // 将 overlay 叠加到 src 上
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawPixmap(0, 0, overlay);
        painter.end();
    }
    return combinedPixmap;
}

// 合并路径
QString CaptureStatic::CombinePath(QString& path1, int num)
{
    QString path = path1  + QString::number(num) + ".png";
    return path;
}

// 读取图片
QPixmap CaptureStatic::ReadImage(QString& path, int num)
{
    QString imagePath = CaptureStatic::CombinePath(path, num);
    return QPixmap(imagePath);
}

// 比较图片，返回最相似的图片下标
int CaptureStatic::CompareImage(cv::Mat& img, map<int, cv::Mat>& _map, double threshold)
{
    int bestMatchKey = EndMenuButtonType::None;
    double bestMatchScore = 0.0;
    int cols = img.cols;
    int rows = img.rows;

    // 遍历模板匹配
    for (auto& pair : _map)
    {
        const int key = pair.first;
        const cv::Mat templateImage = pair.second;
        if (cols != templateImage.cols || rows != templateImage.rows)
        {
            continue;
        }

        // 转换为灰度图
        cv::Mat gray1, gray2;
        cv::cvtColor(img, gray1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(templateImage, gray2, cv::COLOR_BGR2GRAY);

        // 二值化处理（增强黑白对比）
        cv::threshold(gray1, gray1, 127, 255, cv::THRESH_BINARY);
        cv::threshold(gray2, gray2, 127, 255, cv::THRESH_BINARY);

        // 计算SSIM
        double ssim_value = ComputeSSIM(gray1, gray2);

        // 检查是否是最佳匹配
        if (ssim_value > bestMatchScore)
        {
            bestMatchScore = ssim_value;
            bestMatchKey = key;
        }
    }

    // 设置匹配阈值
    if (bestMatchScore > threshold)
    {
        qDebug() << QString::fromLocal8Bit("匹配成功，最佳匹配阈值 ") << bestMatchScore;
        return bestMatchKey;
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("匹配失败，最佳匹配阈值 ") << bestMatchScore;
        return EndMenuButtonType::None;
    }
}

/* 加载动画图片 */
int CaptureStatic::LoadAnimPixmaps(QVector<QPixmap>& AnimPixmaps, const QString Path, int PixmapNum)
{
    for (int i = 1; i <= PixmapNum; i++)
    {
        QString imagePath = Path + QString::number(i) + ".png";
        QPixmap pixmap(imagePath);
        if (pixmap.isNull())
        {
            qDebug() << "Failed to load image: " << imagePath;
            return -1;
        }
        else
        {
            AnimPixmaps.append(pixmap);
        }
    }
    qDebug() << "Loading anim pixmaps... PixmapNum = " << PixmapNum;
    return AnimPixmaps.size();
}

double CaptureStatic::ComputeSSIM(const cv::Mat& i1, const cv::Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;

    cv::Mat I1, I2;
    i1.convertTo(I1, CV_32F);
    i2.convertTo(I2, CV_32F);

    cv::Mat I2_2 = I2.mul(I2);                                                                          // I2^2
    cv::Mat I1_2 = I1.mul(I1);                                                                          // I1^2
    cv::Mat I1_I2 = I1.mul(I2);                                                                         // I1 * I2

    cv::Mat mu1, mu2;
    cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2 = mu1.mul(mu1);
    cv::Mat mu2_2 = mu2.mul(mu2);
    cv::Mat mu1_mu2 = mu1.mul(mu2);

    cv::Mat sigma1_2, sigma2_2, sigma12;
    cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    cv::Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                                                                                    // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                                                                                    // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    cv::divide(t3, t1, ssim_map);                                                                       // ssim_map =  t3./t1;

    cv::Scalar mssim = cv::mean(ssim_map);                                                              // mssim = average of ssim map
    return (mssim[0] + mssim[1] + mssim[2]) / 3;
}

/* 比较两张图片 */
bool CaptureStatic::CompareImage(cv::Mat& CurrCropImg, cv::Mat& PreCropImg, double threshold)
{
    //cv::Mat ResultImg;
    //cv::matchTemplate(CurrCropImg, PreCropImg, ResultImg, cv::TM_CCOEFF_NORMED);

    //// 获取匹配的最大值
    //double maxVal;
    //cv::minMaxLoc(ResultImg, nullptr, &maxVal);

    //if (maxVal > threshold)
    //{
    //    qDebug() << QString::fromLocal8Bit("匹配成功，最佳匹配阈值 ") << maxVal;
    //    return true;
    //}
    //return false;

    // 转换为灰度图
    cv::Mat gray1, gray2;
    cv::cvtColor(CurrCropImg, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(PreCropImg, gray2, cv::COLOR_BGR2GRAY);

    // 二值化处理（增强黑白对比）
    cv::threshold(gray1, gray1, 127, 255, cv::THRESH_BINARY);
    cv::threshold(gray2, gray2, 127, 255, cv::THRESH_BINARY);

    // 计算SSIM
    double ssim_value = ComputeSSIM(gray1, gray2);

    return ssim_value > threshold;
}

/* 通过连接的通道数来判断是否有遮挡，如果该区域的像素数量少于 100，说明只是零星散散的 */
bool CaptureStatic::IsBloodCovered(cv::Mat& MaskImg)
{
    int counts = cv::countNonZero(MaskImg);
    if (counts <= 100)
    {
        return false;
    }

    vector<vector<cv::Point>> contours;
    cv::findContours(MaskImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    double TotalArea = 0;
    int size = contours.size();
    for (auto contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area < 10.0)
        {
            size--;
        }
        TotalArea += area;
    }

    if (size > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* 将指定路径下的所有图片重命名为1.png,2.png,3.png... */
void CaptureStatic::renameAllImages(const QString& dirPath, const QString fileName)
{
    QDir dir(dirPath);
    if (!dir.exists())
    {
        qDebug() << QString::fromLocal8Bit("目录不存在: ") << dirPath;
        return;
    }

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";
    dir.setNameFilters(filters);

    int count = 1;
    for (auto file : dir.entryInfoList())
    {
        QString oldName = file.fileName();
        QString newName = fileName + QString::number(count) + ".png";
        if (oldName!= newName)
        {
            QString newPath = dirPath + "/" + newName;
            QFile::rename(file.absoluteFilePath(), newPath);
        }
        count++;
    }
}

// 计算血条的像素
int CaptureStatic::CalculateBloodPiexl(cv::Mat image)
{
    // 定义绿色和黄色的 BGR 范围
    cv::Scalar greenLowerBound(65, 100, 0);                                                     // BGR 低限
    cv::Scalar greenUpperBound(180, 240, 40);                                                   // BGR 高限
    cv::Scalar yellowLowerBound(0, 65, 137);                                                    // BGR 低限
    cv::Scalar yellowUpperBound(35, 140, 240);                                                  // BGR 高限

    // 计算绿色像素数量
    // 创建掩码，仅保留绿色范围内的像素
    cv::Mat greenMask;
    cv::inRange(image, greenLowerBound, greenUpperBound, greenMask);
    // 计算绿色像素总数
    int greenPixelCount = cv::countNonZero(greenMask);

    // 判断绿色血条是否被遮挡
    bool green = IsBloodCovered(greenMask);

    // 计算黄色像素数量
    // 创建掩码，仅保留黄色范围内的像素
    cv::Mat yellowMask;
    cv::inRange(image, yellowLowerBound, yellowUpperBound, yellowMask);
    // 计算黄色像素总数
    int yellowPixelCount = cv::countNonZero(yellowMask);
    //qDebug() << "黄色像素总数: " << yellowPixelCount;
    // 判断黄色血条是否被遮挡
    bool yellow = IsBloodCovered(yellowMask);

    // 如果被遮挡了，返回 -1
    if (green || yellow)
        return -1;

    int totalPixelNum = yellowPixelCount + greenPixelCount;
    // qDebug() << "像素总数为 " << totalPixelNum;
    return totalPixelNum;
}

/* 合并图片 */
QPixmap CaptureStatic::MergeImage(QPixmap& src, QPixmap& overlay)
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

/* 返回合并的图片的数组 */
QVector<QPixmap> CaptureStatic::MergeImages(QPixmap& src, QString Path, int PixmapNum)
{
    QVector<QPixmap> results;
    QVector<QPixmap> overlay = GetAvatarGrayPixmaps(Path, PixmapNum);
    qDebug() << "overlay size = " << overlay.size();
    if (src.isNull() && overlay.empty())
    {
        return results;
    }
    
    for (auto& pixmap : overlay)
    {
        qDebug() << QString::fromLocal8Bit("正在合并图片...");
        results.append(CaptureStatic::MergeImage(src, pixmap));
    }
    return results;
}

QVector<QPixmap> CaptureStatic::MergeImages(QPixmap& src, QVector<QPixmap>& overlays)
{
    QVector<QPixmap> results;
    if (src.isNull() && overlays.empty())
    {
        return results;
    }

    for (auto& overlay : overlays)
    {
        results.append(CaptureStatic::MergeImage(src, overlay));
    }
    return results;
}