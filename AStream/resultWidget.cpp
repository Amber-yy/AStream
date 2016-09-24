#include "resultWidget.h"
#include <QLabel>
#include <QPushButton>
#include "searchList.h"

#include <QMessageBox>

resultWidget::resultWidget(QVector<QRect> &geo,QWidget *parents)
	: QWidget(parents)
{
	try
	{
		songName = new QLabel(this);
		singerName = new QLabel(this);
		duration = new QLabel(this);

		play = new QPushButton(this);
		add = new QPushButton(this);
		downLoad = new QPushButton(this);

		mainList = dynamic_cast<searchList *>(parents);

		songName->setObjectName("songName");
		singerName->setObjectName("singerName");
		duration->setObjectName("duration");
		play->setObjectName("playBt");
		add->setObjectName("addBt");
		downLoad->setObjectName("downLoadBt");

		setGeo(geo);

		connect(play, &QPushButton::clicked, [this]()
		{
			mainList->playSong(this);
		}
		);

		connect(add, &QPushButton::clicked, [this]()
		{
			mainList->addSong(this);
		}
		);

		connect(downLoad, &QPushButton::clicked, [this]()
		{
			mainList->downLoadSong(this);
		}
		);

	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

resultWidget::~resultWidget()
{

}

void resultWidget::setGeo(QVector<QRect>& geo)
{
	songName->setGeometry(geo[0]);
	singerName->setGeometry(geo[1]);
	duration->setGeometry(geo[2]);
	play->setGeometry(geo[3]);
	add->setGeometry(geo[4]);
	downLoad->setGeometry(geo[5]);
}

void resultWidget::setInfo(musicAPI::musicInfo &i)
{
	info = i;

	auto setSongInfo = [](QLabel *label,QString &name,size_t maxName)
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

		if (length > maxName)
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
				if (i > maxName - 3)
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

		label->setText(temp);
	};

	setSongInfo(songName, info.musicName, maxSongName);
	setSongInfo(singerName, info.singer, maxSingerName);

	auto intToString = [](size_t t)->QString
	{
		size_t min, sec;
		QString minStr, secStr;
		min = t / 60;
		t -= min * 60;
		sec = t;

		minStr += QString::number(min);
		if (min<10)
		{
			minStr.push_front('0');
		}

		secStr += QString::number(sec);
		if (sec<10)
		{
			secStr.push_front('0');
		}
		return minStr + ':' + secStr;
	};

	duration->setText(intToString(i.duration));

}

musicAPI::musicInfo resultWidget::getInfo()
{
	return info;
}
