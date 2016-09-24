#pragma once

#include "tranWidget.h"

class QPushButton;

class aboutPlayerWidget :public tranWidget
{
public:
	aboutPlayerWidget(int, int, QWidget *parent = 0);
	virtual ~aboutPlayerWidget();
protected:
	QLabel *promt;
	QLabel *email;
	QLabel *qq;
	QLabel *github;
	QLabel *version;
	QLabel *releaseDate;
	QLabel *thanks;
	QLabel *thanks1;
	QLabel *thanks2;
	QLabel *thanks3;
	QPushButton *submit;
};