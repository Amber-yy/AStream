#include "songWidget.h"
#include "songsList.h"
#include <QListWidget>

#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QFile>
#include <QEvent>
#include <QPainter>

songWidget::songWidget(songInfo info, QVector<QRect> &geo, QWidget *parent) : QWidget(parent), song(info)
{
	likeBt = new QPushButton(this);
	songName = new QLabel(this);
	setSongName(info.musicName);
	playBt = new QPushButton(this);
	downLoadBt = new QPushButton(this);
	addBt = new QPushButton(this);
	deleteBt = new QPushButton(this);

	likeBt->setGeometry(geo[0]);
	songName->setGeometry(geo[1]);
	playBt->setGeometry(geo[2]);
	downLoadBt->setGeometry(geo[3]);
	addBt->setGeometry(geo[4]);
	deleteBt->setGeometry(geo[5]);

	if (!info.isLike)
	{
		likeBt->setObjectName("likeBt");
	}
	else
	{
		likeBt->setObjectName("ulikeBt");
	}

	songName->setObjectName("songName");
	playBt->setObjectName("playBt");
	downLoadBt->setObjectName("downLoadBt");
	addBt->setObjectName("addBt");
	deleteBt->setObjectName("deleteBt");

	connectSignal();
	setCursor();

}

void songWidget::setSongName(QString name)
{
	size_t asciiNum = 0, chineseNum = 0;

	for (auto s : name)
	{
		if (s.isLower() || s.isNumber() || s.isUpper())
		{
			asciiNum++;
		}
		else
		{
			chineseNum++;
		}
	}

	size_t length = asciiNum + 2 * chineseNum;
	QString temp;

	if (length > maxSongName)
	{
		int i = 0;

		for (auto s : name)
		{
			if (s.isLower() || s.isNumber() || s.isUpper())
			{
				temp.push_back(s);
				i++;
			}
			else
			{
				temp.push_back(s);
				i += 2;
			}
			if (i > maxSongName - 3)
			{
				break;
			}
		}

		temp.push_back("...");

	}
	else
	{
		temp = name;
	}

	songName->setText(temp);

}

songWidget::~songWidget()
{

}

void songWidget::connectSignal()
{
	connect(downLoadBt, &QPushButton::clicked, [this]()
	{
		mainList->emitDownLoad(song);
	}
	);

	connect(likeBt, &QPushButton::clicked, [this]()
	{
		setLike(!song.isLike);
	}
	);

	connect(addBt, &QPushButton::clicked, [this]()
	{
		mainList->addPlayQueue(this);
	}
	);

	connect(deleteBt, &QPushButton::clicked, [this]()
	{
		mainList->removeWidget(this);
	}
	);

	connect(playBt, &QPushButton::clicked, [this]()
	{
		mainList->setCurrentWidget(this);
	}
	);

}

void songWidget::setCursor()
{
	likeBt->setCursor(Qt::PointingHandCursor);
	playBt->setCursor(Qt::PointingHandCursor);
	downLoadBt->setCursor(Qt::PointingHandCursor);
	addBt->setCursor(Qt::PointingHandCursor);
	deleteBt->setCursor(Qt::PointingHandCursor);
}

void songWidget::setLike(bool like)
{
	song.isLike = like;
	if (like)
	{
		likeBt->setObjectName("ulikeBt");
	}
	else
	{
		likeBt->setObjectName("likeBt");
	}
	setStyleSheet(mainList->getStyle());
	if (this == mainList->getCurrentWidget())
	{
		mainList->setLike(like);
	}
}

bool songWidget::getLike()
{
	return song.isLike;
}

void songWidget::unSetCurrent()
{
	songName->setObjectName("songName");
}

void songWidget::setCurrent()
{
	songName->setObjectName("currentSongName");
}

songWidget::songInfo& songWidget::getSong()
{
	return song;
}
