#include "ThreadPool.h"
#include "Worker.h"
#include "SignalSend.h"
#include <QDebug>

ThreadPool::ThreadPool(int maxThreadCount, QObject* parent)
    : QObject(parent), m_currentIndex(0)
{
    // 确保至少1个线程
    if (maxThreadCount < 1) {
        maxThreadCount = 1;
    }

    for (int i = 0; i < maxThreadCount; i++)
    {
        QThread* thread = new QThread(this);
        thread->setObjectName(QString("ThreadPool-%1").arg(i + 1));
        Worker* worker = new Worker;
        worker->moveToThread(thread);
        m_workers.append(worker);
        m_threads.enqueue(thread);
        thread->start();
    }

    m_signalSend = SignalSend::GetInstance();
    connect(m_signalSend.get(), &SignalSend::AddTaskToThreadPool, this, &ThreadPool::submit);
}

ThreadPool::~ThreadPool()
{
    shutdown();
}

QThread* ThreadPool::getThread()
{
    QMutexLocker locker(&m_mutex);

    if (m_shutdown || m_threads.isEmpty()) 
    {
        return nullptr;
    }

    // 使用原子操作实现无锁轮询
    int index = m_currentIndex.fetchAndAddRelaxed(1);

    // 计算实际索引
    index %= m_threads.size();

    return m_threads.at(index);
}

void ThreadPool::moveToThreadPool(Interface* obj)
{
    if (!obj || m_shutdown) {
        return;
    }

    QThread* thread = getThread();
    if (!thread) return;

    // 确保对象在线程结束时被删除
    connect(thread, &QThread::finished, obj, &QObject::deleteLater);
    connect(this, &ThreadPool::startInit, obj, &Interface::InitSelf);

    m_objects.append(obj);

    // 移动对象到新线程
    obj->moveToThread(thread);
}

void ThreadPool::shutdown()
{
    QList<QThread*> threadsToWait;
    {
        QMutexLocker locker(&m_mutex);
        if (m_shutdown) {
            return;
        }
        m_shutdown = true;
        threadsToWait = m_threads;
        m_threads.clear();                                          // 清空，防止 getThread() 再获取
    }

    for (QThread* thread : threadsToWait) {
        thread->quit();
        thread->wait();
    }
    qDeleteAll(threadsToWait);

    m_workers.clear();
}

void ThreadPool::submit(std::function<void()> task)
{
    if (m_shutdown)
    {
        qDebug() << "ThreadPool is shutting down";
        return;
    }

    int index = m_currentIndex.fetchAndAddRelaxed(1) % m_threads.size();
    *m_workers[index] << task;
}