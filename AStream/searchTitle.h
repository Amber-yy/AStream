#ifndef SEARCHTITLE_H
#define SEARCHTITLE_H

#include <QWidget>

class QLabel;

class searchTitle : public QWidget
{
	Q_OBJECT

public:
	searchTitle(QWidget *parent);
	~searchTitle();
protected:
	virtual void paintEvent(QPaintEvent *)override;
	QColor color;
	QLabel *songName;
	QLabel *singerName;
	QLabel *duration;
	QLabel *operate;
};

#endif // SEARCHTITLE_H
