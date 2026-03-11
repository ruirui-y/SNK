#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

class Interface  : public QObject
{
	Q_OBJECT

public:
	Interface(QObject *parent = 0);
	~Interface();

public slots:
	virtual void InitSelf() = 0;
};
#endif // INTERFACE_H