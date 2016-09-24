#include "songsList.h"
#include "songWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QFile>
#include <QEvent>
#include <QPainter>
#include <QScrollBar>
#include "astream.h"

songsList *songWidget::mainList;

#include <QMessageBox>
#include <QDebug>

songsList::songsList(AStream *p,QWidget *parent): QListWidget(parent),mainPlayer(p),currentWidget(nullptr)
{
	songWidget::mainList = this;
	
	QSettings set("data/playListGeo.ini", QSettings::IniFormat);
	auto size = set.value("Main/likeBt").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}
	size = set.value("Main/songName").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}
	size = set.value("Main/playBt").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}
	size = set.value("Main/downLoadBt").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}
	size = set.value("Main/addBt").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}
	size = set.value("Main/deleteBt").toString().split(' ');
	if (size.size() >= 4)
	{
		songWidgetGeo.push_back(QRect(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt()));
	}

	QFile qss("data/qss/songWidget.qss");
	qss.open(QIODevice::ReadOnly);
	if (qss.isOpen())
	{
		widgetStyle = qss.readAll();
	}
	
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	setStyleSheet(widgetStyle);

	QFile saveFile("data/songs/allSongs.sl");
	saveFile.open(QIODevice::ReadOnly);

	if (saveFile.isOpen())
	{

		for (;;)
		{
			songWidget::songInfo info;
			info.musicName = saveFile.readLine();
			info.musicName.chop(1);
			if (info.musicName.isEmpty())
			{
				break;
			}
			info.playUrl = saveFile.readLine();
			info.playUrl.chop(1);
			info.hashCode = saveFile.readLine();
			info.hashCode.chop(1);
			info.isLocal = saveFile.readLine()[0];
			info.isLike = saveFile.readLine()[0];
			addSong(info);
		}

	}
	if (allSongs.size() > 0)
	{
		currentWidget = allSongs[0];
		currentWidget->setCurrent();
	}
	currentWidgetColor.setRgb(qRgb(102,204,250));
}

songsList::~songsList()
{
	QFile saveFile("data/songs/allSongs.sl");
	saveFile.open(QIODevice::WriteOnly);

	if (saveFile.isOpen())
	{

		for (auto s : allSongs)
		{
			QByteArray t;
			auto tempSong = s->getSong();
			t.append(tempSong.musicName);
			t.append('\n');
			t.append(tempSong.playUrl);
			t.append('\n');
			t.append(tempSong.hashCode);
			t.append('\n');
			t.append(tempSong.isLocal);
			t.append('\n');
			t.append(tempSong.isLike);
			t.append('\n');
			saveFile.write(t);
		}

	}

}

void songsList::emitDownLoad(songWidget::songInfo n)
{
	emit downLoad(n);
}

void songsList::setLike(bool like)
{
	if (mainPlayer->getLike() != like)
	{
		mainPlayer->setLike(like);
	}

	if (currentWidget == nullptr)
	{
		return;
	}
	
	if (currentWidget->getLike()!=like)
	{
		currentWidget->setLike(like);
	}
}

void songsList::addPlayQueue(songWidget *sw)
{
	playQueue.enqueue(sw);
}

void songsList::removeWidget(songWidget *sw)
{
	int index = allSongs.indexOf(sw);
	removeItemWidget(item(index));
	delete takeItem(index);
	allSongs.removeAt(index);

	index = playQueue.indexOf(sw);
	if (index != -1)
	{
		playQueue.removeAt(index);
	}
	index = histoy.indexOf(sw);
	if (index != -1)
	{
		histoy.removeAt(index);
	}

	if (currentWidget == sw)
	{
		currentWidget = dynamic_cast<songWidget *>(itemWidget(item(0)));
		if (!mainPlayer->getBlock())
		{
			mainPlayer->stopSong();
		}
	}

}

void songsList::setCurrentWidget(songWidget *sw)
{
	if (currentWidget != nullptr)
	{
		currentWidget->unSetCurrent();
	}
	currentWidget = sw;

	histoy.push_back(sw);

	if (histoy.size() > maxHistory)
	{
		histoy.pop_front();
	}
	currentWidget->setCurrent();
	setStyleSheet(widgetStyle);
	mainPlayer->playSong(sw->getSong());
}

songWidget* songsList::addSong(songWidget::songInfo &info)
{
	QListWidgetItem *lwi = new QListWidgetItem(this);
	songWidget *sw = new songWidget(info, songWidgetGeo, this);
	sw->installEventFilter(this);
	setItemWidget(lwi, sw);
	allSongs.push_back(sw);
	return sw;
}

int songsList::getSize()
{
	return allSongs.size();
}

songWidget * songsList::getPrevSong()
{
	if (histoy.isEmpty())
	{
		return nullptr;
	}

	histoy.pop_back();
	if (histoy.isEmpty())
	{
		return nullptr;
	}
	auto p = histoy.last();
	histoy.pop_back();
	return p;
}

songWidget * songsList::getFirst()
{
	if (allSongs.size() > 0)
	{
		return allSongs[0];
	}
	return nullptr;
}

songWidget * songsList::getNextSong()
{
	if (!playQueue.isEmpty())
	{
		auto sw = playQueue.head();
		playQueue.dequeue();
		return sw;
	}

	int index = allSongs.indexOf(currentWidget);
	if (index == allSongs.size() - 1)
	{
		return nullptr;
	}
	return allSongs[index + 1];
}

songWidget * songsList::getOneWidget(size_t n)
{
	songWidget* sw = nullptr;

	if (allSongs.size() > 0)
	{
		sw = allSongs[n];
	}

	if (!playQueue.isEmpty())
	{
		sw = playQueue.head();
		playQueue.dequeue();
	}
	return sw;
}

songWidget * songsList::getCurrentWidget()
{
	return currentWidget;
}


QString& songsList::getStyle()
{
	return widgetStyle;
}

bool songsList::eventFilter(QObject *obj, QEvent *e)
{
	auto widget = dynamic_cast<songWidget *>(obj);

	if (widget == nullptr)
	{
		return QListWidget::eventFilter(obj, e);
	}

	if (widget == currentWidget&&e->type() == QEvent::Paint)
	{
		QPainter t(widget);
		t.setBrush(QBrush(currentWidgetColor));
		t.drawRect(-1,-1, widget->width()+1, widget->height()+1);
	}
	else if (e->type() == QEvent::MouseButtonDblClick)
	{
		setCurrentWidget(widget);
	}

	return QListWidget::eventFilter(obj,e);
}
