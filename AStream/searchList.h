#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include "searchTitle.h"
#include "resultWidget.h"

class songsList;

class searchList : public QListWidget
{
	Q_OBJECT
public:
	searchList(songsList *,QWidget *parent=0);
	~searchList();
	void search(QString);
	size_t getMaxSize();
	void setMaxSize(size_t);
	void clearSongs();
	void addSong(resultWidget *);
	void playSong(resultWidget *);
	void downLoadSong(resultWidget *);
protected:
	void showResult(QVector<musicAPI::musicInfo>);
protected:
	QVector<QRect> resultGeo;
	QString resultWidgetStyle;
	size_t maxSize;
	songsList *songs;
	searchTitle *t;
	musicAPI *resultProvider;
};

#endif // SEARCHLIST_H
