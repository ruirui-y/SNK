#include "SignalRelay.h"
#include <QDebug>


SignalRelay::SignalRelay(QObject *parent)
	: QObject(parent), m_gameFlowInfo(new GameFlowInfo)
{
}

SignalRelay::~SignalRelay()
{
}