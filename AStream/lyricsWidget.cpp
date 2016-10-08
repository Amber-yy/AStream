#include "lyricsWidget.h"
#include <QLabel>
#include <QSettings>
#include <QPainter>
#include <QPaintEvent>

#include <QDebug>
#include <windows.h>

lyricsWidget::lyricsWidget(QWidget *parent):QWidget(parent),playProgress(0),repaintAll(true),moveRest(0)
{
	try
	{
		pix = QPixmap(540,410);
		pix.fill(Qt::transparent);

		deskLyrics = std::make_shared<desktopLyrics>(980, 116);
		deskLyrics->setColor(QColor(102, 204, 250, 1));
		deskLyrics->setFirstColor(QColor(102, 204, 250));
		deskLyrics->setSecondColor(QColor(102, 204, 250));
		deskLyrics->setGeometry(193,620,980,116);
		deskLyrics->show();

		QSettings setting("data/lyrics.ini", QSettings::IniFormat);
	
		auto setGeo = [&setting](QRect &la,QString key)
		{
			auto list = setting.value("Geometry/"+key).toString();
			auto coord=list.split(' ');
			la=QRect(coord[0].toInt(),coord[1].toInt(),coord[2].toInt(),coord[3].toInt());
		};

		setGeo(firstRect, "first");
		setGeo(secondRect, "second");
		setGeo(thirdRect, "third");
		setGeo(fourthRect, "fourth");
		setGeo(fifthRect, "fifth");
		setGeo(sixthRect, "sixth");
		setGeo(seventhRect, "seventh");

		font.setFamily(u8"ºÚÌå");
		font.setPixelSize(22);
		maskStartPoint.setY(fourthRect.y());
		/*
		QFile qss("data/qss/lyrics.qss");
		qss.open(QIODevice::ReadOnly);
		setStyleSheet(qss.readAll());
		*/
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
	pix.fill(Qt::transparent);
	restartLyrics();
}

void lyricsWidget::updateLyrics(size_t duration)
{
	int size = allLyrics.size();

	if (size == 0)
	{
		return;
	}

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
	playProgress = currentLen / sectionLen;

	auto str = fourth.toStdWString();
	maxPix = deskLyrics->getMaxPix()*11;
	maskStartPoint.setX((width() - maxPix) / 2);
	
	if (size > currentIndex+1&&duration>=allLyrics[currentIndex+1].duration)
	{
		repaintAll = true;
		pix.fill(Qt::transparent);
		currentIndex++;

		if (currentIndex >=3)
		{
			first = allLyrics[currentIndex - 3].lyrics;
		}
		if (currentIndex >= 2)
		{
			second = allLyrics[currentIndex - 2].lyrics;
		}
		if (currentIndex >= 1)
		{
			third = allLyrics[currentIndex - 1].lyrics;
		}

		fourth=allLyrics[currentIndex].lyrics;

		deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
		deskLyrics->setNextText("");

		fifth="";
		sixth = "";
		seventh = "";

		if (size > currentIndex + 1)
		{
			fifth=allLyrics[currentIndex + 1].lyrics;
			deskLyrics->setNextText(fifth);
		}
		if (size > currentIndex + 2)
		{
			sixth = allLyrics[currentIndex + 2].lyrics;
		}
		if (size > currentIndex + 3)
		{
			seventh = allLyrics[currentIndex + 3].lyrics;
		}

	}
	update();
}

void lyricsWidget::restartLyrics()
{
	currentIndex = 0;

	first = "";
	second = "";
	third = "";
	fourth = "";
	fifth = "";
	sixth = "";
	seventh = "";
	deskLyrics->setCurrentText("");
	deskLyrics->setNextText("");

	if (allLyrics.size() > 0)
	{
		fourth=allLyrics[0].lyrics;
		deskLyrics->setCurrentText(allLyrics[0].lyrics);
	}

	if (allLyrics.size() > 1)
	{
		fifth=allLyrics[1].lyrics;
		deskLyrics->setNextText(allLyrics[1].lyrics);
	}

	if (allLyrics.size() > 2)
	{
		sixth=allLyrics[2].lyrics;
	}

	if (allLyrics.size() > 3)
	{
		seventh = allLyrics[2].lyrics;
	}

	repaintAll = true;
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

	first = "";
	second = "";
	third = "";
	fourth = "";
	fifth = "";
	sixth = "";
	seventh = "";
	deskLyrics->setCurrentText("");
	deskLyrics->setNextText("");

	if (currentIndex == -1)
	{
		currentIndex = size-1;

		if (currentIndex > 3)
		{
			first = allLyrics[currentIndex - 3].lyrics;
		}

		if (currentIndex > 2)
		{
			second=allLyrics[currentIndex - 2].lyrics;
		}

		if (currentIndex > 1)
		{
			third = allLyrics[currentIndex - 1].lyrics;
		}

		if (currentIndex>=0)
		{
			fourth=allLyrics[currentIndex].lyrics;
			deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
		}

		return;
	}

	if (currentIndex > 1)
	{
		third = allLyrics[currentIndex - 1].lyrics;
	}

	if (currentIndex > 2)
	{
		second=allLyrics[currentIndex-2].lyrics;
	}

	if (currentIndex > 3)
	{
		first=allLyrics[currentIndex - 3].lyrics;
	}

	if (size- currentIndex> 0)
	{
		fourth=allLyrics[currentIndex].lyrics;
		deskLyrics->setCurrentText(allLyrics[currentIndex].lyrics);
	}

	if (size- currentIndex> 1)
	{
		fifth=allLyrics[currentIndex+1].lyrics;
		deskLyrics->setNextText(allLyrics[currentIndex+1].lyrics);
	}

	if (size- currentIndex> 2)
	{
		sixth=allLyrics[currentIndex+2].lyrics;
	}

	if (size - currentIndex> 3)
	{
		seventh = allLyrics[currentIndex + 3].lyrics;
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

void lyricsWidget::paintEvent(QPaintEvent *e)
{

	if (repaintAll)
	{
		QPainter painter(&pix);
		painter.setFont(font);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(firstRect, Qt::AlignCenter, first);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(secondRect, Qt::AlignCenter, second);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(thirdRect, Qt::AlignCenter, third);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(fourthRect, Qt::AlignCenter, fourth);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(fifthRect, Qt::AlignCenter, fifth);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(sixthRect, Qt::AlignCenter, sixth);

		painter.setPen(QColor(255, 255, 255));
		painter.drawText(seventhRect, Qt::AlignCenter, seventh);

		temp = pix;
		moveRest = 56;
		repaintAll = false;
		update();
	}
	else
	{
		temp = pix;
		QPainter painter(&temp);
		painter.setFont(font);

		if (playProgress <= 1)
		{
			painter.setPen(QColor(251,219,131));
			painter.drawText(maskStartPoint.x(), maskStartPoint.y(), playProgress*maxPix, 56, Qt::AlignVCenter | Qt::AlignLeft, fourth);
		}
	}

	if (moveRest <= 0)
	{
		moveRest = 0;
		QPainter pixDrawer(this);
		pixDrawer.drawPixmap(0, 0, temp);
	}
	else
	{
		QPainter pixDrawer(this);
		pixDrawer.drawPixmap(0, moveRest, temp);
		moveRest -= 3;
		Sleep(10);
		update();
	}

}
