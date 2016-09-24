#ifndef TRANWIDGET_H
#define TRANWIDGET_H

#include <QWidget>

class QLabel;

class tranWidget : public QWidget
{
	Q_OBJECT
public:
	tranWidget(int, int, QWidget *parent = 0);
	virtual ~tranWidget();
	virtual void setColor(QColor);
	virtual void showTiltle(bool);
	virtual void setTitle(const QString &);
	virtual void setLock(bool);
	virtual bool getLock();
	virtual QColor getColor();
protected:
	virtual void paintEvent(QPaintEvent *)override;
	virtual void mouseReleaseEvent(QMouseEvent *)override;
	virtual void mousePressEvent(QMouseEvent *)override;
	virtual void mouseMoveEvent(QMouseEvent *)override;
private:
	QColor color;
	QPoint lastPos;
	QWidget *titleWidget;
	QLabel *titleText;
	bool isPress;
	bool isLocked;
};

#endif // TRANWIDGET_H
