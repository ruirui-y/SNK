#ifndef SHOWLABEL_H
#define SHOWLABEL_H

#include <QLabel>

class ShowLabel  : public QLabel
{
	Q_OBJECT

public:
	ShowLabel(QWidget *parent = 0);
	~ShowLabel();

	void UpdateStyleSheet(QString Text);
};

#endif // SHOWLABEL_H