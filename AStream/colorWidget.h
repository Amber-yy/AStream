#pragma once
#include "tranWidget.h"

class QPushButton;

class colorWidget :public tranWidget
{
	Q_OBJECT
public:
	colorWidget(int, int, QColor *,QWidget *parent = 0);
	virtual ~colorWidget();
	void resetColor()
	{
		repaint();
	}
	QPushButton *getFirst()
	{
		return firstBt;
	}
	QPushButton *getSecond()
	{
		return secondBt;
	}
	QPushButton *getThird()
	{
		return thirdBt;
	}
	QColor &getColor(int index)
	{
		return colors[index];
	}
signals:
	void colorApply();
protected:
	void apply();
	virtual void showEvent(QShowEvent *)override;
	virtual bool eventFilter(QObject *, QEvent *)override;
	QColor colors[3];
	QPushButton *firstBt;
	QPushButton *secondBt;
	QPushButton *thirdBt;
	QPushButton *submit;
	QPushButton *cancel;
	QPushButton *closeBt;
	QColor &first;
	QColor &second;
	QColor &third;
};

