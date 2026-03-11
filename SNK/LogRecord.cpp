#include "LogRecord.h"
#include <QTime>
#include <QFileInfo>

QFile LogRecord::file("log.txt"); // 静态成员变量类外初始化，变量类型 + 类作用域 + 变量名 + 初始值

LogRecord::LogRecord(QObject *parent)
	: Interface(parent)
{

}

void LogRecord::InitSelf()
{
    // 初始化日志记录
    qInstallMessageHandler(customMessageHandler);
}

LogRecord::~LogRecord()
{}

// 自定义消息处理函数，将Qt日志输出到文件
void LogRecord::customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    if (!file.isOpen()) {
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    // 改进空消息检测：移除双引号并修剪空白
    QString trimmedMsg = msg;
    trimmedMsg.remove('"');
    trimmedMsg = trimmedMsg.trimmed();
    if (trimmedMsg.isEmpty()) {
        return;
    }

    QTextStream stream(&file);
    QString logMessage;

    // 格式化日志信息
    QString timeStamp = QDateTime::currentDateTime().toString("MM:dd:hh:mm:ss.zzz"); // 修改时间格式
    QString logType;
    switch (type) {
    case QtDebugMsg:    logType = "Debug"; break;
    case QtInfoMsg:     logType = "Info"; break;
    case QtWarningMsg:  logType = "Warning"; break;
    case QtCriticalMsg: logType = "Critical"; break;
    case QtFatalMsg:    logType = "Fatal"; break;
    default:            logType = "Unknown"; break;
    }

    // 构建带详细信息的日志消息
    logMessage = QString("%1  [%2]:  %3")
        .arg(timeStamp)
        .arg(logType)
        .arg(msg);

    stream << logMessage << "\n";
    file.flush();
}