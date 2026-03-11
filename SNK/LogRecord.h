#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "Interface.h"

class LogRecord  : public Interface
{
	Q_OBJECT

public:
	LogRecord(QObject *parent = nullptr);
	~LogRecord();

public:
	static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg); // 自定义消息处理函数，将Qt日志输出到文件
	static QFile file;

public slots:
	virtual void InitSelf() override;
};

#endif // LOGRECORD_H