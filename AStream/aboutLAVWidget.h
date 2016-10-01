#pragma once
#include "tranWidget.h"

class QLabel;
class QPushButton;

class aboutLAVWidget :public tranWidget
{
public:
	aboutLAVWidget(int,int,QWidget *parents=0);
	virtual ~aboutLAVWidget();
protected:
	QLabel *describe1;
	QLabel *describe2;
	QLabel *officialWeb;
	QPushButton *submit;
};
 