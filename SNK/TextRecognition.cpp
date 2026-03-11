#include "TextRecognition.h"
#include "allheaders.h"
#include <QDebug>
#include <QDir>

TextRecognition::TextRecognition(QObject *parent)
	: QObject(parent)
{
    api = new tesseract::TessBaseAPI();
    bool ret = api->Init("./tessdata", "eng");
    if (ret) { qDebug() << "TextRecognition initialized failed"; }
}

Pix* TextRecognition::matToPix(cv::Mat& img)
{
    if (img.empty()) return nullptr;

    Pix* pix = nullptr;

    // 根据Mat类型进行转换
    if (img.type() == CV_8UC1)
    {
        // 创建8位灰度Pix图像
        pix = pixCreate(img.cols, img.rows, 8);
        if (!pix) {
            qWarning() << "Failed to create Pix for grayscale image";
            return nullptr;
        }

        // 获取每行的字数（words per line）
        l_uint32 wpl = pixGetWpl(pix);

        // 确保每行字节数匹配
        if (img.cols != pixGetWidth(pix)) {
            qWarning() << "Width mismatch between cv::Mat and Pix";
            pixDestroy(&pix);
            return nullptr;
        }

        // 复制图像数据
        for (int y = 0; y < img.rows; y++)
        {
            // 获取源和目标行指针
            uint8_t* src_line = img.ptr(y);
            uint8_t* dst_line = reinterpret_cast<uint8_t*>(pixGetData(pix) + y * wpl);

            // 复制整行数据
            memcpy(dst_line, src_line, img.cols);
        }
    }
    else if (img.type() == CV_8UC3)
    {
        // BGR彩色图像
        cv::Mat rgb;
        cv::cvtColor(img, rgb, cv::COLOR_BGR2RGB);

        pix = pixCreate(img.cols, img.rows, 32);                // 32位深度（RGBA）
        if (pix)
        {
            for (int y = 0; y < rgb.rows; y++)
            {
                uint32_t* dst_line = pixGetData(pix) + y * pixGetWpl(pix);
                uint8_t* src_line = rgb.ptr(y);

                for (int x = 0; x < rgb.cols; x++)
                {
                    // 将 RGB（3字节）转换为 32 位 Pix 格式（0xRRGGBB00）
                    dst_line[x] =
                        (src_line[3 * x] << 24) |       // R
                        (src_line[3 * x + 1] << 16) |   // G
                        (src_line[3 * x + 2] << 8);     // B
                    // Alpha 默认为 0
                }
            }
        }
    }

    return pix;
}

TextRecognition::~TextRecognition()
{}

bool TextRecognition::CompareImageText(cv::Mat & img, QString & text)
{

	return false;
}

bool TextRecognition::CompareImageText(cv::Mat& img1, cv::Mat& img2)
{
    Pix* pix1 = matToPix(img1);
    Pix* pix2 = matToPix(img2);
	
    if (!pix1 || !pix2) 
    {
        if (pix1) pixDestroy(&pix1);
        if (pix2) pixDestroy(&pix2);
        return false;
    }

    api->SetImage(pix1);
    QString text1 = api->GetUTF8Text();

    api->SetImage(pix2);
    QString text2 = api->GetUTF8Text();

    pixDestroy(&pix1);
    pixDestroy(&pix2);

    qDebug() << "text1: " << text1;
    qDebug() << "text2: " << text2;

    // 忽略大小写
    text1 = text1.toLower();
    text2 = text2.toLower();

    // 去除空格
    text1.remove(" ");
    text2.remove(" ");

    // 去除换行符
    text1.replace("\n", "");
    text2.replace("\n", "");

    qDebug() << "text1: " << text1;
    qDebug() << "text2: " << text2;

    if (text1 == text2)
    {
        qDebug() << QString::fromLocal8Bit("文本校验成功");
        return true;
    }
        
    qDebug() << QString::fromLocal8Bit("文本校验失败");
    return false;
}