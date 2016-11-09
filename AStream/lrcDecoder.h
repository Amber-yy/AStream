#ifndef LRCDECODER_H
#define LRCDECODER_H

#include <QObject>
#include <QFile>
#include "songsList.h"
#include "songWidget.h"
class musicAPI;

struct lrc
{
	size_t duration;
	QString lyrics;
	bool operator<(const lrc&l)
	{
		return duration < l.duration;
	}
};

class lrcDecoder : public QObject
{
	Q_OBJECT
public:
	lrcDecoder(const QString &,QObject *parent=0);
	~lrcDecoder();
	void getLrc(const songWidget::songInfo &);
	void lrcHandle();
	void setRoute(QString lrc)
	{
		lrcRoute = lrc;
	}
signals:
	void decodeFinished(QVector<lrc> &);
protected:
	QString lrcRoute;
	QString currentRoute;
	musicAPI *lrcProvider;
};

#endif 
