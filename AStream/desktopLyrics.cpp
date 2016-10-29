#include "desktopLyrics.h"
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include <QPaintEvent>

#include <QDebug>

desktopLyrics::desktopLyrics(int w, int h, QWidget *parent):tranWidget(w,h,parent),progress(10),isUp(true), repaintAll(true)
{
	try
	{
		font.setFamily(u8"ºÚÌå");
		font.setPixelSize(32);
		showTiltle(false);

		pix = QPixmap(980, 116);
		pix.fill(Qt::transparent);

		normalGradient1.setStart(0,9);
		normalGradient1.setFinalStop(0, 41);
		normalGradient1.setColorAt(0, QColor(0, 51, 153));
		normalGradient1.setColorAt(0.8, QColor(0, 102, 255));
		normalGradient1.setColorAt(1, QColor(80, 170, 250));

		normalGradient2.setStart(0, 67);
		normalGradient2.setFinalStop(0, 99);
		normalGradient2.setColorAt(0, QColor(0, 51, 153));
		normalGradient2.setColorAt(0.8, QColor(0, 102, 255));
		normalGradient2.setColorAt(1, QColor(80, 170, 250));

		maskGradient1.setStart(0, 0);
		maskGradient1.setFinalStop(0, 50);
		maskGradient1.setColorAt(0, QColor(102,204,250));
		maskGradient1.setColorAt(0.5, QColor(255, 255, 255));
		maskGradient1.setColorAt(1, QColor(102,204,250));

		maskGradient2.setStart(0, 58);
		maskGradient2.setFinalStop(0, 108);
		maskGradient2.setColorAt(0, QColor(102, 204, 250));
		maskGradient2.setColorAt(0.5, QColor(255, 255, 255));
		maskGradient2.setColorAt(1, QColor(102, 204, 250));
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

desktopLyrics::~desktopLyrics()
{

}

void desktopLyrics::setCurrentText(const QString &text)
{
	first = text;
	isUp = !isUp;

	repaintAll = true;
	pix.fill(Qt::transparent);

	auto str = text.toStdWString();
	maxPix = 0;
	for (auto ch: str)
	{
		if (0 <= ch&&ch <= 128)
		{
			maxPix += font.pixelSize()/2;
		}
		else
		{
			maxPix += font.pixelSize();
		}
	}

	update();
}

void desktopLyrics::setNextText(const QString & text)
{
	second = text;
	update();
}

void desktopLyrics::setLyricsFont(QFont f)
{
	font = f;
	update();
}

void desktopLyrics::setFirstColor(QColor c)
{
	firstColor = c;
	update();
}

void desktopLyrics::setSecondColor(QColor c)
{
	secondColor = c;
	update();
}

void desktopLyrics::resetProgress(double p)
{
	progress = p;
	update();
}

void desktopLyrics::paintEvent(QPaintEvent *e)
{
	if (isUp)
	{
		if (repaintAll)
		{
			QPainter firstPainter(&pix);
			firstPainter.setFont(font);
			firstPainter.setPen(QColor(0, 0, 0));
			firstPainter.drawText(1, 1, 800, 58, Qt::AlignVCenter | Qt::AlignLeft, first);
			firstPainter.setPen(QPen(normalGradient1, 0));
			firstPainter.drawText(0, 0, 800, 58, Qt::AlignVCenter | Qt::AlignLeft, first);

			firstPainter.setPen(QColor(0, 0, 0));
			firstPainter.drawText(1, 59, 800, 58, Qt::AlignVCenter | Qt::AlignRight, second);
			firstPainter.setPen(QPen(normalGradient2, 0));
			firstPainter.drawText(0, 58, 800, 58, Qt::AlignVCenter | Qt::AlignRight, second);

			temp = pix;
			repaintAll = false;
		}
		else
		{
			pix = temp;
			if (progress <= 1)
			{
				QPainter mask(&temp);
				mask.setFont(font);
				mask.setPen(QPen(maskGradient1, 0));
				mask.drawText(0, 0, progress*maxPix, 58, Qt::AlignVCenter | Qt::AlignLeft, first);
			}
		}

	}
	else
	{
		if (repaintAll)
		{
			QPainter firstPainter(&pix);
			firstPainter.setFont(font);
			firstPainter.setPen(QColor(0, 0, 0));
			firstPainter.drawText(1, 59, 800, 58, Qt::AlignVCenter | Qt::AlignRight, first);
			firstPainter.setPen(QPen(normalGradient2, 0));
			firstPainter.drawText(0, 58, 800, 58, Qt::AlignVCenter | Qt::AlignRight, first);

			firstPainter.setPen(QColor(0, 0, 0));
			firstPainter.drawText(1, 1, 800, 58, Qt::AlignVCenter | Qt::AlignLeft, second);
			firstPainter.setPen(QPen(normalGradient1, 0));
			firstPainter.drawText(0, 0, 800, 58, Qt::AlignVCenter | Qt::AlignLeft, second);

			temp = pix;
			repaintAll = false;
		}
		else
		{
			temp = pix;
			if (progress <= 1)
			{
				QPainter mask(&temp);
				mask.setFont(font);
				mask.setPen(QPen(maskGradient2, 0));
				mask.drawText(800 - maxPix, 58, progress*maxPix, 58, Qt::AlignVCenter | Qt::AlignLeft, first);
			}
		}
	}

	QPainter pixDrawer(this);
	pixDrawer.drawPixmap(0, 0, temp);

	return tranWidget::paintEvent(e);
}

bool desktopLyrics::event(QEvent *e)
{
	
	if (e->type() == QEvent::Enter)
	{
		auto c = getColor();
		c.setAlpha(100);
		setColor(c);
		return false;
	}
	else if(e->type() == QEvent::Leave)
	{
		auto c = getColor();
		c.setAlpha(1);
		setColor(c);
		return false;
	}

	return QWidget::event(e);
}
