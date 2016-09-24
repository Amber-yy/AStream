#include "desktopLyrics.h"
#include <QLabel>
#include <QEvent>

#include <QDebug>

desktopLyrics::desktopLyrics(int w, int h, QWidget *parent):tranWidget(w,h,parent)
{
	try
	{
		first = new QLabel(this);
		second = new QLabel(this);

		first->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
		second->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

		first->setGeometry(0,0,w,h/2);
		second->setGeometry(0, h/2, w, h / 2);

		QFont t;
		t.setFamily(u8"ºÚÌå");
		t.setPointSize(24);
		
		first->setFont(t);
		second->setFont(t);
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
	first->setText(text);
}

void desktopLyrics::setNextText(const QString & text)
{
	second->setText(text);
}

void desktopLyrics::setLyricsFont(QFont f)
{
	first->setFont(f);
	second->setFont(f);
}

void desktopLyrics::setFirstColor(QColor c)
{
	QPalette p;
	p.setColor(QPalette::WindowText,c);
	first->setPalette(p);
}

void desktopLyrics::setSecondColor(QColor c)
{
	QPalette p;
	p.setColor(QPalette::WindowText, c);
	second->setPalette(p);
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
