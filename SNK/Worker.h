#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <functional>

using Task = std::function<void()>;

class Worker  : public QObject
{
	Q_OBJECT

public:
	Worker(QObject *parent = 0);
	~Worker();

public:
	friend int operator<<(Worker& worker, Task task);

public slots:
	void ProcessTasks();

private:
	void AddTask(Task task);

private:
	QMutex m_mutex;
	QQueue<Task> m_tasks;
};
#endif // WORKER_H