#include "tranwidget.h"
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

#include <QDebug>

tranWidget::tranWidget(int w, int h, QWidget *parent) : QWidget(parent),isLocked(false),isPress(false)
{
	try
	{
		setMaximumSize(QSize(w, h));
		setMinimumSize(QSize(w, h));

		setAutoFillBackground(false);
		setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint|Qt::Tool);
		setAttribute(Qt::WA_TranslucentBackground, true);

		titleWidget = new QWidget(this);
		titleText = new QLabel(titleWidget);
		titleText->setAlignment(Qt::AlignCenter);

		titleWidget->setGeometry(0, 0, w, 30);
		titleText->setGeometry(0, 0, w, 30);
		titleText->setFont(QFont(u8"Î¢ÈíÑÅºÚ"));
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

tranWidget::~tranWidget()
{

}

void tranWidget::paintEvent(QPaintEvent *e)
{
	if (!isLocked)
	{
		QPainter p(this);
		p.fillRect(rect(), color);
	}

	if (titleWidget->isVisible())
	{
		QPainter paint(this);
		paint.fillRect(titleWidget->rect(), QColor(102, 204, 250));
	}

	return QWidget::paintEvent(e);
}

void tranWidget::mouseReleaseEvent(QMouseEvent *)
{
	isPress = false;
}

void tranWidget::mousePressEvent(QMouseEvent *event)
{
	if (isLocked)
	{
		return;
	}
	if (titleWidget->isVisible())
	{
		lastPos = event->globalPos();
		if (event->y() < titleWidget->height())
		{
			isPress = true;
		}
	}
	else
	{
		lastPos = event->globalPos();
		isPress = true;
	}
}

void tranWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (isPress)
	{
		int dx = event->globalX() - lastPos.x();
		int dy = event->globalY() - lastPos.y();
		lastPos = event->globalPos();
		move(x() + dx, y() + dy);
	}
}

void tranWidget::setColor(QColor c)
{
	color = c;
	update();
}

void tranWidget::showTiltle(bool show)
{
	if (show)
	{
		titleWidget->show();
	}
	else
	{
		titleWidget->hide();
	}
}

void tranWidget::setTitle(const QString &t)
{
	titleText->setText(t);
}

void tranWidget::setLock(bool l)
{
	isLocked = l;
	update();
}

bool tranWidget::getLock()
{
	return isLocked;
}

QColor tranWidget::getColor()
{
	return color;
}
