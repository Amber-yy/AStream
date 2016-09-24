#include "searchList.h"
#include "kuGouAPI.h"
#include "songsList.h"
#include <QEvent>
#include <QFile>
#include <QSettings>

#include <QDebug>
#include <QMessageBox>

searchList::searchList(songsList *sl, QWidget *parent): QListWidget(parent),songs(sl)
{
	try
	{
		resultProvider = new kuGouAPI(this);

		t = new searchTitle(this);
		QListWidgetItem *lwi = new QListWidgetItem(this);
		setItemWidget(lwi, t);

		QFile qss("data/qss/resultWidget.qss");
		qss.open(QIODevice::ReadOnly);
		if (qss.isOpen())
		{
			resultWidgetStyle = qss.readAll();
		}
		setStyleSheet(resultWidgetStyle);

		resultGeo.resize(6);
		QSettings set("data/resultGeo.ini", QSettings::IniFormat);

		auto s = set.value("Main/songName").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[0] = QRect(s[0].toInt(),s[1].toInt(),s[2].toInt(),s[3].toInt());
		}

		s = set.value("Main/singerName").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[1] = QRect(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/duration").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[2] = QRect(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/playBt").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[3] = QRect(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/addBt").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[4] = QRect(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/downLoadBt").toString().split(' ');
		if (s.size() >= 4)
		{
			resultGeo[5] = QRect(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		connect(resultProvider, &musicAPI::infoGetted, this, &searchList::showResult);

	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

searchList::~searchList()
{

}

void searchList::search(QString key)
{
	resultProvider->getAllInfo(key,maxSize);
}

size_t searchList::getMaxSize()
{
	return maxSize;
}

void searchList::setMaxSize(size_t s)
{
	maxSize = s;
}

void searchList::clearSongs()
{
	size_t size = count();
	for (; size > 0; size--)
	{
		delete takeItem(size);
	}
}

void searchList::addSong(resultWidget *rw)
{
	songWidget::songInfo info;
	musicAPI::musicInfo originInfo = rw->getInfo();

	info.duration = originInfo.duration;
	info.hashCode = originInfo.hashCode;
	info.isLocal = false;
	info.isLike = false;
	info.musicName = originInfo.fileName;

	songs->addSong(info);
}

void searchList::playSong(resultWidget *rw)
{
	songWidget::songInfo info;
	musicAPI::musicInfo originInfo = rw->getInfo();

	info.duration = originInfo.duration;
	info.hashCode = originInfo.hashCode;
	info.isLocal = false;
	info.isLike = false;
	info.musicName = originInfo.fileName;

	auto w=songs->addSong(info);
	songs->setCurrentWidget(w);
}

void searchList::downLoadSong(resultWidget *rw)
{
	songWidget::songInfo info;
	musicAPI::musicInfo originInfo = rw->getInfo();

	info.hashCode = originInfo.hashCode;
	info.isLocal = false;
	info.musicName = originInfo.fileName;

	songs->emitDownLoad(info);
}

void searchList::showResult(QVector<musicAPI::musicInfo> allInfo)
{
	clearSongs();

	for (auto &t : allInfo)
	{
		resultWidget *w = new resultWidget(resultGeo, this);
		auto it= new QListWidgetItem(this);
		setItemWidget(it, w);
		w->setInfo(t);
	}

}
