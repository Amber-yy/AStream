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
	void resetProgroess(size_t);
	void showDesktop(bool);
	void setLock(bool);
	bool getLock();
protected:
	QVector<lrc> allLyrics;
	std::shared_ptr<desktopLyrics> deskLyrics;
	QLabel *first;
	QLabel *second;
	QLabel *third;
	QLabel *fourth;
	QLabel *fifth;
	int currentIndex;
};

#endif // LYRICSWIDGET_H
