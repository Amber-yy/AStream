#ifndef SONGSLIST_H
#define SONGSLIST_H

#include <QListWidget>
#include <QQueue>
#include "songWidget.h"
class QLabel;
class QPushButton;
class songsList;
class AStream;

class songsList : public QListWidget
{
	Q_OBJECT
public:
	songsList(AStream *,QWidget *parent=0);
	~songsList();
	void emitDownLoad(songWidget::songInfo);
	void setLike(bool);
	void addPlayQueue(songWidget *);
	void removeWidget(songWidget *);
	void setCurrentWidget(songWidget *);
	int getSize();
	songWidget *addSong(songWidget::songInfo &);
	songWidget *getPrevSong();
	songWidget *getFirst();
	songWidget *getNextSong();
	songWidget *getOneWidget(size_t);
	songWidget *getCurrentWidget();
	QString& getStyle();
signals:
	void downLoad(songWidget::songInfo);
protected:
	virtual bool eventFilter(QObject*, QEvent *)override;
	static const int maxHistory = 20;
	QVector<QRect> songWidgetGeo;
	QVector<songWidget *>allSongs;
	QQueue<songWidget *>playQueue;
	QVector<songWidget *>histoy;
	QColor currentWidgetColor;
	QString widgetStyle;
	AStream *mainPlayer;
	songWidget *currentWidget;
};

#endif // SONGSLIST_H
