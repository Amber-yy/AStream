#ifndef SCREENSHOTER_H
#define SCREENSHOTER_H

#include <QWidget>


class screenShoter : public QWidget
{
	Q_OBJECT
public:
	screenShoter(QString, QWidget *parent = 0);
	~screenShoter();
	void setAutoSave(bool);
	void setFileRoute(QString);
protected:
	virtual void paintEvent(QPaintEvent *)override;
	virtual void mouseReleaseEvent(QMouseEvent *)override;
	virtual void mousePressEvent(QMouseEvent *)override;
	virtual void mouseMoveEvent(QMouseEvent *)override;
private:
	QString fileRoute;
	QColor color;
	QPoint firstPos, lastPos;
	QSize deskSize;
	bool autoSave;
};

#endif // SCREENSHOTER_H
