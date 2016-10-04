#ifndef SKINSETTER_H
#define SKINSETTER_H

#include "tranWidget.h"
#include <QVector>

class QScrollArea;
class QPushButton;
class paletteWidget;

class skinSetter : public tranWidget
{
	Q_OBJECT
public:
	skinSetter(int,int,QWidget *parent = 0);
	virtual ~skinSetter();
	virtual bool eventFilter(QObject *, QEvent *)override;
	virtual void findCurrent(const QString &);
	virtual void setCustom();
signals:
	void pictureSetted(QString &);
	void colorSetted(QColor);
protected:
	struct picSize
	{
		picSize(double w, double h)
		{
			width = w;
			hight = h;
		}
		double width;
		double hight;
	};
	virtual void loadImages();
	virtual void resetCurrent();
	virtual picSize getRealPicSize(int, int);
protected:
	QVector<QPushButton *> smallSkin;
	QVector<QPixmap> smallPic;
	QStringList allRoute;
	paletteWidget *paletteW;
	QScrollArea *allSkinArea;
	QPushButton *hoverWidget;
	QPushButton *currentSkin;
	QPushButton *palette;
	QPushButton *custom;
	QPushButton *closeBt;
	QWidget *skinWidget;
};

#endif // SKINSETTER_H
