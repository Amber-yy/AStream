#pragma once
#include "tranWidget.h"

class QLabel;
class QPushButton;

class aboutFFMPEGWidget :public tranWidget
{
public:
	aboutFFMPEGWidget(int,int,QWidget* parent=0);
	virtual ~aboutFFMPEGWidget();
protected:
	QLabel *ffmpegIcon;
	QLabel *describe;
	QLabel *officialWeb;
	QPushButton *submit;
};

