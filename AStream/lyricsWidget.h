#ifndef LYRICSWIDGET_H
#define LYRICSWIDGET_H

#include <QWidget>
#include <memory>
#include "lrcDecoder.h"
#include "desktopLyrics.h"

class QLabel;

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
protected:
	virtual void paintEvent(QPaintEvent *)override;
protected:
	QVector<lrc> allLyrics;
	std::shared_ptr<desktopLyrics> deskLyrics;
	QString first, second, third, fourth, fifth,sixth,seventh;
	QRect firstRect, secondRect, thirdRect, fourthRect, fifthRect, sixthRect, seventhRect;
	QFont font;
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
