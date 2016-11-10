#ifndef LYRICSWIDGET_H
#define LYRICSWIDGET_H

#include <QWidget>
#include <memory>
#include "lrcDecoder.h"
#include "desktopLyrics.h"

class QLabel;

#include <QDebug>

class lyricsWidget : public QWidget
{
	Q_OBJECT
public:
	lyricsWidget(QWidget *parent=0);
	~lyricsWidget();
	void resetLyrics(QVector<lrc> &);
	void updateLyrics(size_t);
	void restartLyrics();
	void setMaxDuration(size_t);
	void resetProgroess(size_t);
	void showDesktop(bool);
	void setLock(bool);
	bool getLock();
	void setFontName(QString &name)
	{
		font.setFamily(name);
	}
	void setFontSize(int s)
	{
		font.setPixelSize(s);
	}
	void setUnplay(QColor c)
	{
		unplay = c;
	}
	void setPlayed(QColor c)
	{
		played = c;
	}
	void setDeskFontName(QString &name)
	{
		deskLyrics->setFontName(name);
	}
	void setDeskFontSize(int s)
	{
		deskLyrics->setFontSize(s);
	}
	void setDeskFontStyle(int flag)
	{
		deskLyrics->setFontStyle(flag);
	}
	void setDeskPlayedColor(QColor *c)
	{
		deskLyrics->setPlayedColor(c);
	}
	void setDeskUnplayColor(QColor *c)
	{
		deskLyrics->setUnplayColor(c);
	}
	void setFontStyle(int);
protected:
	virtual void paintEvent(QPaintEvent *)override;
protected:
	QVector<lrc> allLyrics;
	std::shared_ptr<desktopLyrics> deskLyrics;
	QString first, second, third, fourth, fifth,sixth,seventh;
	QRect firstRect, secondRect, thirdRect, fourthRect, fifthRect, sixthRect, seventhRect;
	QFont font;
	QColor unplay;
	QColor played;
	double playProgress;
	size_t maxDuration;
	QPoint maskStartPoint;
	QPixmap pix,temp,movePix;
	int moveRest;
	int maxPix;
	int currentIndex;
	bool repaintAll;
};

#endif // LYRICSWIDGET_H
