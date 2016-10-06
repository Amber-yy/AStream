#include "lyricsWidget.h"
#include <QLabel>
#include <QSettings>
#include <QDebug>
#include <windows.h>

lyricsWidget::lyricsWidget(QWidget *parent):QWidget(parent)
{
	try
	{
		first = new QLabel(this);
		second = new QLabel(this);
		third = new QLabel(this);
		fourth = new QLabel(this);
		fifth = new QLabel(this);

		deskLyrics = std::make_shared<desktopLyrics>(980, 116);
		deskLyrics->setColor(QColor(102, 204, 250, 1));
		deskLyrics->setFirstColor(QColor(102, 204, 250));
		deskLyrics->setSecondColor(QColor(102, 204, 250));
		deskLyrics->setGeometry(193,620,980,116);
		deskLyrics->show();

		first->setAlignment(Qt::AlignCenter);
		second->setAlignment(Qt::AlignCenter);
		third->setAlignment(Qt::AlignCenter);
		fourth->setAlignment(Qt::AlignCenter);
		fifth->setAlignment(Qt::AlignCenter);

		QSettings setting("data/lyrics.ini", QSettings::IniFormat);
	
		auto setGeo = [&setting](QLabel *la,QString key)
		{
			auto list = setting.value("Geometry/"+key).toString();
			la->setObjectName(key);
			auto coord=list.split(' ');
			la->setGeometry(coord[0].toInt(),coord[1].toInt(),coord[2].toInt(),coord[3].toInt());
		};

		setGeo(first, "first");
		setGeo(second, "second");
		setGeo(third, "third");
		setGeo(fourth, "fourth");
		setGeo(fifth, "fifth");

		QFile qss("data/qss/lyrics.qss");
		qss.open(QIODevice::ReadOnly);
		setStyleSheet(qss.readAll());

	}
	catch(std::bad_alloc &)
	{
		abort();
	}

}

lyricsWidget::~lyricsWidget()
{

}

void lyricsWidget::resetLyrics(QVector<lrc>&lrcs)
{
	allLyrics = std::move(lrcs);
	restartLyrics();
}

void lyricsWidget::updateLyrics(size_t duration)
{
	int size = allLyrics.size();

	double sectionLen, currentLen;

	if (currentIndex == 0)
	{
		if (duration < allLyrics[0].duration)
		{
			currentLen = 0;
			sectionLen = 1;
		}
		else
		{ 
			currentLen = static_cast<double>(duration - allLyrics[currentIndex].duration);
			sectionLen = static_cast<double>(allLyrics[currentIndex + 1].duration - allLyrics[currentIndex].duration);
		}
	}
	else if (currentIndex == size - 1)
	{
		currentLen = static_cast<double>(duration - allLyrics[currentIndex].duration);
		sectionLen= static_cast<double>(maxDuration - allLyrics[currentIndex].duration);
	}
	else
	{
		currentLen = static_cast<double>(duration - allLyrics[currentIndex].duration);
		sectionLen = static_cast<double>(allLyrics[currentIndex+1].duration - allLyrics[currentIndex].duration);
	}

	deskLyrics->resetProgress(currentLen / sectionLen);

	if (size > currentIndex+1&&duration>=allLyrics[currentIndex+1].duration)
	{
		if (currentIndex >=0)
		{
			second->setText(allLyrics[currentIndex].lyrics);
		}
		if (currentIndex >=1)
		{
			first->setText(allLyrics[currentIndex-1].lyrics);
		}
		currentIndex++;
		third->setText(allLyrics[currentIndex].lyrics);
		deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
		deskLyrics->setNextText("");
		fourth->setText("");
		fifth->setText("");
		if (size > currentIndex + 1)
		{
			fourth->setText(allLyrics[currentIndex +1].lyrics);
			deskLyrics->setNextText(allLyrics[currentIndex + 1].lyrics);
		}
		if (size > currentIndex + 2)
		{
			fifth->setText(allLyrics[currentIndex + 2].lyrics);
		}

	}
}

void lyricsWidget::restartLyrics()
{
	currentIndex = 0;

	first->setText("");
	second->setText("");
	third->setText("");
	fourth->setText("");
	fifth->setText("");
	deskLyrics->setCurrentText("");
	deskLyrics->setNextText("");

	if (allLyrics.size() > 0)
	{
		third->setText(allLyrics[0].lyrics);
		deskLyrics->setCurrentText(allLyrics[0].lyrics);
	}

	if (allLyrics.size() > 1)
	{
		fourth->setText(allLyrics[1].lyrics);
		deskLyrics->setNextText(allLyrics[1].lyrics);
	}

	if (allLyrics.size() > 2)
	{
		fifth->setText(allLyrics[2].lyrics);
	}

}

void lyricsWidget::setMaxDuration(size_t d)
{
	maxDuration = d;
}

void lyricsWidget::resetProgroess(size_t duration)
{
	int size = allLyrics.size();
	currentIndex = -1;
	for (int i = 0; i <size;i++)
	{
		if (allLyrics[i].duration>=duration)
		{
			if (i > 0)
			{
				currentIndex = i-1;
			}
			else
			{
				currentIndex = i;
			}
			break;
		}
	}

	first->setText("");
	second->setText("");
	third->setText("");
	fourth->setText("");
	fifth->setText("");
	deskLyrics->setCurrentText("");
	deskLyrics->setNextText("");

	if (currentIndex == -1)
	{
		currentIndex = size-1;
		if (currentIndex > 1)
		{
			second->setText(allLyrics[currentIndex - 1].lyrics);
		}

		if (currentIndex > 2)
		{
			first->setText(allLyrics[currentIndex - 2].lyrics);
		}

		if (currentIndex>=0)
		{
			third->setText(allLyrics[currentIndex].lyrics);
			deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
		}
		return;
	}

	if (currentIndex > 1)
	{
		second->setText(allLyrics[currentIndex-1].lyrics);
	}

	if (currentIndex > 2)
	{
		first->setText(allLyrics[currentIndex - 2].lyrics);
	}

	if (size- currentIndex> 0)
	{
		third->setText(allLyrics[currentIndex].lyrics);
		deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
	}

	if (size- currentIndex> 1)
	{
		fourth->setText(allLyrics[currentIndex+1].lyrics);
		deskLyrics->setNextText(allLyrics[currentIndex+1].lyrics);
	}

	if (size- currentIndex> 2)
	{
		fifth->setText(allLyrics[currentIndex+2].lyrics);
	}

}

void lyricsWidget::showDesktop(bool show)
{
	if (show)
	{
		deskLyrics->show();
	}
	else
	{
		deskLyrics->hide();
	}
}

void lyricsWidget::setLock(bool b)
{
	deskLyrics->setLock(b);
}

bool lyricsWidget::getLock()
{
	return deskLyrics->getLock();
}
