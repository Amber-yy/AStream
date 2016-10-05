#include "desktopLyrics.h"
#include <QLabel>
#include <QEvent>
#include <QPainter>

#include <QDebug>

desktopLyrics::desktopLyrics(int w, int h, QWidget *parent):tranWidget(w,h,parent),progress(10)
{
	try
	{
		font.setFamily(u8"ºÚÌå");
		font.setPixelSize(32);
		showTiltle(false);
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
	QPainter firstPainter(this);
	firstPainter.setFont(font);
	firstPainter.setPen(firstColor);
	firstPainter.drawText(0, 0, 800,58 , Qt::AlignVCenter | Qt::AlignLeft, first);

	QPainter mask(this);
	mask.setFont(font);
	mask.setPen(QColor(255, 255, 255));
	mask.drawText(0, 0, progress*32 *first.length() , 58, Qt::AlignVCenter | Qt::AlignLeft, first);

	QPainter secondPainter(this);
	secondPainter.setFont(font);
	secondPainter.setPen(secondColor);
	secondPainter.drawText(0, 58, 800, 58, Qt::AlignVCenter | Qt::AlignRight, second);

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
