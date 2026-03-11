#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QQueue>
#include <QList>
#include <QAtomicInteger>
#include <QCoreApplication>
#include <functional>
#include "Interface.h"
#include "SignalSend.h"

class Worker;
class SignalSend;

class ThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit ThreadPool(int maxThreadCount = QThread::idealThreadCount(),
        QObject* parent = nullptr);
    ~ThreadPool();

    // 获取线程对象（轮询方式）
    QThread* getThread();

    // 将对象移动到线程池线程
    void moveToThreadPool(Interface* obj);

    // 安全关闭线程池
    void shutdown();

signals:
    void startInit();

public slots:
    // 提交任务到线程池
    void submit(std::function<void()> task);

private:
    QQueue<QThread*> m_threads;                                                 // 线程队列
    QMutex m_mutex;                                                             // 互斥锁
    QAtomicInteger<int> m_currentIndex;                                         // 当前线程索引（原子操作）
    bool m_shutdown = false;                                                    // 关闭标志
    QList<Interface*> m_objects;                                                // 对象队列
    QList<Worker*> m_workers;                                                   // 工作者队列
    QSharedPointer<SignalSend> m_signalSend;                                    // 信号发送器
};
#endif // THREADPOOL_H