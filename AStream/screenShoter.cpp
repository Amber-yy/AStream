#include "screenshoter.h"
#include <QApplication>
#include <QDeskTopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDateTime>
#include <QClipBoard>

screenShoter::screenShoter(QString f, QWidget *parent)
	: QWidget(parent), fileRoute(f), autoSave(false)
{
	auto t = QApplication::desktop();

	deskSize = t->size();

	setMaximumSize(deskSize);
	setMinimumSize(deskSize);

	setAutoFillBackground(false);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);

	color = QColor(102, 204, 250, 100);

	firstPos.setX(t->width());
	firstPos.setY(t->height());
	lastPos = firstPos;

}

screenShoter::~screenShoter()
{

}

void screenShoter::setAutoSave(bool a)
{
	autoSave = a;
}

void screenShoter::setFileRoute(QString r)
{
	fileRoute = r;
}

void screenShoter::paintEvent(QPaintEvent *)
{
	int smallX, bigX, smallY, bigY;
	smallX = firstPos.x() < lastPos.x() ? firstPos.x() : lastPos.x();
	bigX = firstPos.x() > lastPos.x() ? firstPos.x() : lastPos.x();
	smallY = firstPos.y() < lastPos.y() ? firstPos.y() : lastPos.y();
	bigY = firstPos.y() > lastPos.y() ? firstPos.y() : lastPos.y();

	QPoint smallPos(smallX, smallY), bigPos(bigX, bigY);

	{
		QRect x(0, 0, smallPos.x(), deskSize.height());
		QPainter p(this);
		p.fillRect(x, color);
	}

	{
		QRect x(smallPos.x(), 0, bigPos.x() - smallPos.x(), smallPos.y());
		QPainter p(this);
		p.fillRect(x, color);
	}

	{
		QRect x(bigPos.x(), 0, deskSize.width() - bigPos.x(), deskSize.height());
		QPainter p(this);
		p.fillRect(x, color);
	}

	{
		QRect x(smallPos.x(), bigPos.y(), bigPos.x() - smallPos.x(), deskSize.height() - bigPos.y());
		QPainter p(this);
		p.fillRect(x, color);
	}
}

void screenShoter::mouseReleaseEvent(QMouseEvent *event)
{
	int smallX, bigX, smallY, bigY;
	smallX = firstPos.x() < lastPos.x() ? firstPos.x() : lastPos.x();
	bigX = firstPos.x() > lastPos.x() ? firstPos.x() : lastPos.x();
	smallY = firstPos.y() < lastPos.y() ? firstPos.y() : lastPos.y();
	bigY = firstPos.y() > lastPos.y() ? firstPos.y() : lastPos.y();
	QPoint smallPos(smallX, smallY), bigPos(bigX, bigY);
	QPixmap pic = QPixmap::grabWindow(QApplication::desktop()->winId(), smallPos.x(), smallPos.y(), bigPos.x() - smallPos.x(), bigPos.y() - smallPos.y());
	QApplication::clipboard()->setPixmap(pic);
	if (autoSave)
	{
		QDateTime t = QDateTime::currentDateTime();
		QString name = t.toString("yyyymmddhhmmss");
		QFile p(fileRoute + name + ".png");
		p.open(QIODevice::WriteOnly);
		pic.save(&p);
		p.close();
	}
	firstPos=QPoint(deskSize.width(),deskSize.height());
	lastPos = firstPos;
	repaint();
	hide();
}

void screenShoter::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
	{
		hide();
		return;
	}
	firstPos = event->pos();
	lastPos = event->pos();
	update();
}

void screenShoter::mouseMoveEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	update();
}
