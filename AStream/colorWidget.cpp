#include "colorWidget.h"

#include <QPushButton>
#include <QEvent>
#include <QPixmap>
#include <QPainter>

colorWidget::colorWidget(int w, int h, QColor *c, QWidget *parent) : tranWidget(w, h, parent),first(c[0]),second(c[1]),third(c[2])
{
	try
	{
		colors[0] = c[0];
		colors[1] = c[1];
		colors[2] = c[2];

		firstBt=new QPushButton(this);
		secondBt = new QPushButton(this);
		thirdBt = new QPushButton(this);
		submit = new QPushButton(u8"确定",this);
		cancel = new QPushButton(u8"取消",this);
		closeBt = new QPushButton(this);

		firstBt->setGeometry(85,50,80,25);
		secondBt->setGeometry(85, 90, 80, 25);
		thirdBt->setGeometry(85, 130, 80, 25);
		submit->setGeometry(35,170,80,25);
		cancel->setGeometry(135, 170, 80, 25);

		closeBt->setStyleSheet
		(
			"QPushButton\
			{\
				border-image:url(\"data/icon/closeNormal.png\");\
			}\
			QPushButton:hover\
			{\
				border-image:url(data/icon/closeHover.png);\
			}\
			QPushButton:pressed\
			{\
				border-image:url(data/icon/closePress.png);\
			}"
		);

		submit->setStyleSheet
		(
			"QPushButton\
			{\
				border-image:url(data/icon/buttonNormal.png);\
			} \
			QPushButton:hover\
			{\
				border-image:url(data/icon/buttonHover.png);\
			}\
			QPushButton:pressed\
			{\
				border-image:url(data/icon/buttonPress.png);\
			}"
		);
		cancel->setStyleSheet(submit->styleSheet());

		closeBt->setGeometry(w-23,0,23,18);

		firstBt->installEventFilter(this);
		secondBt->installEventFilter(this);
		thirdBt->installEventFilter(this);

		connect(cancel, &QPushButton::clicked, this, &QWidget::close);
		connect(closeBt, &QPushButton::clicked, this, &QWidget::close);
		connect(submit, &QPushButton::clicked, this, &colorWidget::apply);

		setColor(QColor(200, 200, 200, 255));
	}
	catch (std::bad_alloc &)
	{
		abort();
	}

}


colorWidget::~colorWidget()
{
}

void colorWidget::apply()
{
	first = colors[0];
	second = colors[1];
	third = colors[2];
	emit colorApply();
	close();
}

void colorWidget::showEvent(QShowEvent *e)
{
	colors[0] =first;
	colors[1] = second;
	colors[2] = third;
}

bool colorWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() != QEvent::Paint)
	{
		return tranWidget::eventFilter(obj, event);
	}

	auto bt = dynamic_cast<QPushButton *>(obj);
	if (bt == nullptr)
	{
		return tranWidget::eventFilter(obj, event);
	}

	QPainter painter(bt);
	QColor c;
	if (bt == firstBt)
	{
		c = colors[0];
	}
	else if (bt == secondBt)
	{
		c = colors[1];
	}
	else
	{
		c = colors[2];
	}

	painter.fillRect(bt->rect(),c);
	event->accept();

	return true;
}
