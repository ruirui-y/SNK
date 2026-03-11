#ifndef SINGLETONE_H
#define SINGLETONE_H

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <mutex>

template<typename T>
class Singletion
{
public: 
    ~Singletion() {
        qDebug() << "Singletion is deleted";
    }

    static QSharedPointer<T> GetInstance()
    {
        static std::once_flag flag;
        std::call_once(flag, [&]() {
            m_instance = QSharedPointer<T>(new T);
            }
        );

        return m_instance;
    }

protected:
    Singletion() = default;
    Singletion(const Singletion<T>&) = delete;
    Singletion& operator=(const Singletion<T>&) = delete;

private:
    static QSharedPointer<T> m_instance;
};

template<typename T>
QSharedPointer<T> Singletion<T>::m_instance = nullptr;

#endif // SINGLETONE_H
