#include "paletteWidget.h"

#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QFile>

paletteWidget::paletteWidget(int w,int h,QWidget *parent):tranWidget(w,h,parent)
{
	try
	{
		submit=new QPushButton(u8"确定",this);
		cancel=new QPushButton(u8"取消",this);
		closeBt = new QPushButton(this);
		palette=new QLabel(this);
		cursor = new QLabel(this);
		palImage = new QImage("data/icon/palette.png");

		submit->setGeometry(406,253,50,25);
		cancel->setGeometry(466, 253, 50, 25);
		closeBt->setGeometry(503,0,23,18);
		palette->setGeometry(10, 40, 506, 203);
		cursor->setGeometry(259, 137, 8, 8);

		closeBt->setObjectName("closeBt");
		submit->setObjectName("setBt");
		cancel->setObjectName("setBt");
		palette->setObjectName("palette");
		cursor->setObjectName("cursor");

		QFile qss("data/qss/palette.qss");
		qss.open(QIODevice::ReadOnly);
		if (qss.isOpen())
		{
			setStyleSheet(qss.readAll());
		}

		connect(closeBt, &QPushButton::clicked, this, [this]() {emit closed(); close(); });
		connect(cancel, &QPushButton::clicked, this, [this]() {emit closed(); close(); });

		connect(submit, &QPushButton::clicked, [this]() 
		{
			emit colorSetted(palImage->pixelColor(cursor->x() -10+ 4, cursor->y() -40+ 4));
			cursor->setGeometry(259, 137, 8, 8);
			this->close();
		}
		);

		setColor(Qt::white);
		setTitle(u8"调色板");
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

paletteWidget::~paletteWidget()
{
}

void paletteWidget::mousePressEvent(QMouseEvent *e)
{
	int x = e->x(), y = e->y();
	palette->setGeometry(10, 40, 506, 203);
	if (x >= 10 && x <= 516 && y >= 40 && y <= 243)
	{
		cursor->setGeometry(x-4, y-4, 8, 8);
	}
	else
	{
		return tranWidget::mousePressEvent(e);
	}
}

void paletteWidget::mouseMoveEvent(QMouseEvent *e)
{
	int x = e->x(), y = e->y();
	palette->setGeometry(10, 40, 506, 203);
	if (x >= 10 && x <= 516 && y >= 40 && y <= 243)
	{
		cursor->setGeometry(x - 4, y - 4, 8, 8);
	}
	else
	{
		return tranWidget::mouseMoveEvent(e);
	}
}
