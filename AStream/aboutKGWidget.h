#pragma once
#include "tranWidget.h"

class QLabel;
class QPushButton;

class aboutKGWidget :public tranWidget
{
public:
	aboutKGWidget(int w, int h, QWidget * parent=0);
	virtual ~aboutKGWidget();
protected:
	QLabel *kgIcon;
	QLabel *describe;
	QLabel *officialWeb;
	QPushButton *submit;
};

