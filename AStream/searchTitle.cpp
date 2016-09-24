#include "searchTitle.h"
#include <QLabel>
#include <QSettings>
#include <QPainter>
#include <QEvent>

#include <QDebug>

searchTitle::searchTitle(QWidget *parent): QWidget(parent)
{
	try
	{
		songName=new QLabel(this);
		singerName = new QLabel(this);
		duration = new QLabel(this);
		operate = new QLabel(this);

		QSettings set("data/searchTitle.ini", QSettings::IniFormat);

		auto s = set.value("Main/songName").toString().split(' ');
		if (s.size() >= 4)
		{
			songName->setGeometry(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/singerName").toString().split(' ');
		if (s.size() >= 4)
		{
			singerName->setGeometry(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/duration").toString().split(' ');
		if (s.size() >= 4)
		{
			duration->setGeometry(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s = set.value("Main/operate").toString().split(' ');
		if (s.size() >= 4)
		{
			operate->setGeometry(s[0].toInt(), s[1].toInt(), s[2].toInt(), s[3].toInt());
		}

		s= set.value("Main/color").toString().split(' ');
		if (s.size() == 3)
		{
			color.setRgb(s[0].toInt(), s[1].toInt(), s[2].toInt());
		}
		else if (s.size() == 4)
		{
			color.setRgb(s[0].toInt(), s[1].toInt(), s[2].toInt(),  s[3].toInt());
		}

		songName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
		singerName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		duration->setAlignment(Qt::AlignCenter);
		operate->setAlignment(Qt::AlignCenter);

		songName->setText(set.value("Main/songNameText").toString());
		singerName->setText(set.value("Main/singerNameText").toString());
		duration->setText(set.value("Main/durationText").toString());
		operate->setText(set.value("Main/operateText").toString());

	}
	catch (std::bad_alloc &)
	{
		abort();
	}

}

searchTitle::~searchTitle()
{

}

void searchTitle::paintEvent(QPaintEvent *)
{
	QPainter t(this);
	t.setBrush(QBrush(color));
	t.drawRect(-1, -1, width()+1, height()+1);
}