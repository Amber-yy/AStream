#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include "musicAPI.h"

class QLabel;
class QPushButton;
class searchList;

class resultWidget : public QWidget
{
	Q_OBJECT
public:
	resultWidget(QVector<QRect> &geo, QWidget *parent);
	~resultWidget();
	void setGeo(QVector<QRect> &);
	void setInfo(musicAPI::musicInfo &);
	musicAPI::musicInfo getInfo();
protected:
	musicAPI::musicInfo info;
	QColor color;
	static const size_t maxSongName = 38;
	static const size_t maxSingerName = 17;
	QLabel *songName;
	QLabel *singerName;
	QLabel *duration;
	QPushButton *play;
	QPushButton *add;
	QPushButton *downLoad;
	searchList* mainList;
};

#endif // RESULTWIDGET_H
