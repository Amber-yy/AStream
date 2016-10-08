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
	void resetProgress(double);
	int getMaxPix()
	{
		return maxPix/16;
	}
protected:
	virtual void paintEvent(QPaintEvent *)override;
	virtual bool event(QEvent *)override;
private:
	QString first, second;
	QColor firstColor, secondColor;
	QFont font;
	QLinearGradient normalGradient1,maskGradient1, normalGradient2, maskGradient2;
	QPixmap pix, temp;
	double progress;
	int maxPix;
	bool isUp;
	bool repaintAll;
};
