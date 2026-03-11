#include "ShowLabel.h"

#include "QStyle"
#include <QVariant>
#include <QDebug>

ShowLabel::ShowLabel(QWidget* parent)
	: QLabel(parent)
{
}

ShowLabel::~ShowLabel()
{}

void ShowLabel::UpdateStyleSheet(QString Text)
{
	setText(Text);
	style()->unpolish(this);
	style()->polish(this);
	update();
}