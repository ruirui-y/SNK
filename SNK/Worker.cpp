#include "Worker.h"

Worker::Worker(QObject *parent)
	: QObject(parent)
{}

Worker::~Worker()
{}

void Worker::AddTask(Task task)
{
	{
		QMutexLocker locker(&m_mutex);
		m_tasks.enqueue(task);
	}

	QMetaObject::invokeMethod(this, "ProcessTasks", Qt::QueuedConnection);
}

void Worker::ProcessTasks()
{
	QQueue<Task> tasks;

	{
		QMutexLocker locker(&m_mutex);
		tasks = std::move(m_tasks);
		m_tasks.clear();
	}

	while (!tasks.empty())
	{
		Task task = tasks.dequeue();
		task();
	}
}

int operator<<(Worker& worker, Task task)
{
	worker.AddTask(task);
	return worker.m_tasks.size();
}