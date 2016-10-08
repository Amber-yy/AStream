#include "lrcDecoder.h"
#include "kuGouAPI.h"
#include <algorithm>
#include <QDebug>
#include <QMessageBox>

lrcDecoder::lrcDecoder(const QString &r,QObject *parent):lrcRoute(r),QObject(parent)
{
	try
	{
		lrcProvider = new kuGouAPI(this);
		connect(lrcProvider, &musicAPI::downLoadFinished, this, &lrcDecoder::lrcHandle);
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

lrcDecoder::~lrcDecoder()
{

}

void lrcDecoder::getLrc(const songWidget::songInfo &info)
{

	currentRoute = lrcRoute + info.musicName.split('.')[0] + ".lrc";
	QFile lrcFile(currentRoute);
	if (!lrcFile.exists()&&!info.isLocal)
	{
		lrcProvider->downLoad(lrcProvider->lrcUrlConnect(info.hashCode), currentRoute);
	}
	else
	{
		lrcHandle();
	}
}

void lrcDecoder::lrcHandle()
{
	QFile lrcFile(currentRoute);
	lrcFile.open(QIODevice::ReadOnly);
	QVector <lrc>  allLyrics;
	QString allData = lrcFile.readAll();
	auto lines = allData.split('\n');
	for (QString &line:lines)
	{
		QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
		QString lrcString = line;
		lrcString.replace(rx, "");

		int pos = rx.indexIn(line, 0);
		while (pos != -1) 
		{ 
			QString cap = rx.cap(0);
			QRegExp regexp;

			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int millisecond = regexp.cap(0).toInt();
			lrc tempLrc;
			tempLrc.lyrics = lrcString;
			tempLrc.duration = minute * 60000 + second * 1000 + millisecond * 10;

			allLyrics.push_front(tempLrc);

			pos += rx.matchedLength();
			pos = rx.indexIn(line, pos);

		}
	
	}

	std::sort(allLyrics.begin(), allLyrics.end(), [](lrc a, lrc b) {return a < b; });

	lrcFile.close();
	emit decodeFinished(allLyrics);
}
