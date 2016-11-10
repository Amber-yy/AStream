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

		pix = std::move(QPixmap(980, 116));
		pix.fill(Qt::transparent);

		movePix = std::move(QPixmap(980, 58));
		movePix.fill(Qt::transparent);

		normalGradient1.setStart(0,0);
		normalGradient1.setFinalStop(0, 58);

		normalGradient2.setStart(0, 58);
		normalGradient2.setFinalStop(0, 116);

		maskGradient1.setStart(0, 0);
		maskGradient1.setFinalStop(0, 58);

		maskGradient2.setStart(0, 58);
		maskGradient2.setFinalStop(0, 116);

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
			maxPix +=font.pixelSize()/2;
		}
		else
		{
			maxPix +=font.pixelSize();
		}
	}

	update();
}

void desktopLyrics::setNextText(const QString & text)
{
	second = text;
	update();
}

void desktopLyrics::resetProgress(double p)
{
	progress = p;
	update();
}

void desktopLyrics::setFontStyle(int flag)
{

	if (flag == 0)
	{
		font.setBold(false);
		font.setItalic(false);
	}
	else if (flag == 1)
	{
		font.setBold(true);
		font.setItalic(false);
	}
	else if (flag == 2)
	{
		font.setBold(false);
		font.setItalic(true);
	}
	else if (flag == 3)
	{
		font.setBold(true);
		font.setItalic(true);
	}

}

void desktopLyrics::setUnplayColor(QColor *c)
{
	normalGradient1.setColorAt(0.2, c[0]);
	normalGradient1.setColorAt(0.5, c[1]);
	normalGradient1.setColorAt(0.8, c[2]);

	normalGradient2.setColorAt(0.2, c[0]);
	normalGradient2.setColorAt(0.5, c[1]);
	normalGradient2.setColorAt(0.8, c[2]);
}

void desktopLyrics::setPlayedColor(QColor *c)
{
	maskGradient1.setColorAt(0.2, c[0]);
	maskGradient1.setColorAt(0.5, c[1]);
	maskGradient1.setColorAt(0.8, c[2]);

	maskGradient2.setColorAt(0.2, c[0]);
	maskGradient2.setColorAt(0.5, c[1]);
	maskGradient2.setColorAt(0.8, c[2]);
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
			firstPainter.drawText(1, 1, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, first);
			firstPainter.setPen(QPen(normalGradient1, 0));
			firstPainter.drawText(0, 0, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, first);

			firstPainter.setPen(QColor(0, 0, 0));
			firstPainter.drawText(1, 59, 980, 58, Qt::AlignVCenter | Qt::AlignRight, second);
			firstPainter.setPen(QPen(normalGradient2, 0));
			firstPainter.drawText(0, 58, 980, 58, Qt::AlignVCenter | Qt::AlignRight, second);

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
			firstPainter.drawText(1, 1, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, second);
			firstPainter.setPen(QPen(normalGradient1, 0));
			firstPainter.drawText(0, 0, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, second);

			temp = pix;
			QPainter tempPainter(&temp);
			tempPainter.setFont(font);
			tempPainter.setPen(QColor(0, 0, 0));
			tempPainter.drawText(1, 59, 980, 58, Qt::AlignVCenter | Qt::AlignRight, first);
			tempPainter.setPen(QPen(normalGradient2, 0));
			tempPainter.drawText(0, 58, 980, 58, Qt::AlignVCenter | Qt::AlignRight, first);
			repaintAll = false;
		}
		else
		{
			temp = pix;

			if (progress <= 1)
			{
				movePix.fill(Qt::transparent);
				QPainter mask(&movePix);
				mask.setFont(font);

				mask.setPen(QColor(0, 0, 0));
				mask.drawText(1, 1, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, first);
				mask.setPen(QPen(normalGradient1, 0));
				mask.drawText(0, 0, 980, 58, Qt::AlignVCenter | Qt::AlignLeft, first);

				mask.setPen(QPen(maskGradient1, 0));
				mask.drawText(0, 0, progress*maxPix, 58, Qt::AlignVCenter | Qt::AlignLeft, first);

				QPainter tempPainter(&temp);
				tempPainter.drawPixmap((980-maxPix), 58, movePix);
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
