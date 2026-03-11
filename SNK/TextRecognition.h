#ifndef TEXTRECOGNITION_H
#define TEXTRECOGNITION_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "singletion.h"
#include "baseapi.h" 

class TextRecognition  : public QObject , public Singletion<TextRecognition>
{
	Q_OBJECT
	friend class Singletion<TextRecognition>;

public:
	~TextRecognition();

	bool CompareImageText(cv::Mat& img, QString& text);
	bool CompareImageText(cv::Mat& img1, cv::Mat& img2);

private:
	TextRecognition(QObject* parent = 0);
	Pix* matToPix(cv::Mat& img);

private:
	tesseract::TessBaseAPI* api;
};

#endif // TEXTRECOGNITION_H