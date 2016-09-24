#pragma once

#include "tranWidget.h"
#include <QWidget>

class desktopLyrics final:public tranWidget
{
	Q_OBJECT
public:
	desktopLyrics(int w, int h, QWidget *parent = 0);
	virtual ~desktopLyrics();
	void setCurrentText(const QString &);
	void setNextText(const QString &);
	void setLyricsFont(QFont);
	void setFirstColor(QColor);
	void setSecondColor(QColor);
protected:
	virtual bool event(QEvent *)override;
private:
	QLabel *first, *second;
};
