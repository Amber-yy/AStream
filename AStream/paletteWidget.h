#pragma once
#include "tranWidget.h"

class QPushButton;
class QLabel;

class paletteWidget :public tranWidget
{
	Q_OBJECT
public:
	paletteWidget(int ,int,QWidget *parent=0);
	virtual ~paletteWidget();
	virtual void mousePressEvent(QMouseEvent *)override;
	virtual void mouseMoveEvent(QMouseEvent *)override;
signals:
	void closed();
	void colorSetted(QColor);
protected:
	QPushButton *submit;
	QPushButton *cancel;
	QPushButton *closeBt;
	QImage *palImage;
	QLabel *palette;
	QLabel *cursor;
};

