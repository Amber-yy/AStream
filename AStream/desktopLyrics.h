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
	void resetProgress(double);
	void setFontName(QString &name)
	{
		font.setFamily(name);
	}
	void setFontSize(int s)
	{
		font.setPixelSize(s);
	}
	void setFontStyle(int);
	void setUnplayColor(QColor *);
	void setPlayedColor(QColor *);
	int getMaxPix()
	{
		return maxPix/(font.pixelSize()/2);
	}
protected:
	virtual void paintEvent(QPaintEvent *)override;
	virtual bool event(QEvent *)override;
private:
	QString first, second;
	QColor firstColor, secondColor;
	QFont font;
	QLinearGradient normalGradient1,maskGradient1, normalGradient2, maskGradient2;
	QPixmap pix, temp,movePix;
	double progress;
	int maxPix;
	bool isUp;
	bool repaintAll;
};
