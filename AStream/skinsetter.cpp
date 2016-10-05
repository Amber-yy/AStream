#include "skinsetter.h"
#include "paletteWidget.h"

#include <QPushButton>
#include <QScrollArea>
#include <QFile>
#include <QEvent>
#include <QPainter>
#include <QDir>
#include <QFileDialog>

skinSetter::skinSetter(int w,int h,QWidget *parent): tranWidget(w,h,parent), hoverWidget(nullptr), currentSkin(nullptr)
{
	try
	{
		allSkinArea = new QScrollArea(this);
		palette = new QPushButton(u8"调色板",this);
		custom = new QPushButton(u8"自定义皮肤", this);
		closeBt = new QPushButton(this);
		skinWidget = new QWidget(allSkinArea);
		paletteW = new paletteWidget(526, 288, 0);
		paletteW->hide();

		allSkinArea->setGeometry(0,30,522,240);
		palette->setGeometry(370,280,50,25);
		custom->setGeometry(440,280,75,25);
		closeBt->setGeometry(499,0,23,18);
		allSkinArea->setWidget(skinWidget);
		connect(closeBt,&QPushButton::clicked,this,&QWidget::close);
		connect(custom,&QPushButton::clicked,this,&skinSetter::setCustom);
		connect(palette, &QPushButton::clicked, paletteW, &QWidget::show);

		connect(paletteW, &paletteWidget::colorSetted, [this](QColor t) 
		{
			emit colorSetted(t);
			currentSkin = nullptr;
		}
		);

		allSkinArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		closeBt->setObjectName("closeBt");
		palette->setObjectName("setBt");
		custom->setObjectName("setBt");

		setTitle(u8"主题与皮肤设置");

		QFile qss("data/qss/skinSetter.qss");
		qss.open(QIODevice::ReadOnly);
		if (qss.isOpen())
		{
			setStyleSheet(qss.readAll());
		}
		setColor(Qt::white);
		loadImages();
	}
	catch (std::bad_alloc &)
	{
		abort();
	}

}

skinSetter::~skinSetter()
{

}

bool skinSetter::eventFilter(QObject *obj, QEvent *e)
{
	auto ptr = dynamic_cast<QPushButton *>(obj);
	if (ptr == nullptr)
	{
		return tranWidget::eventFilter(obj, e);
	}

	if (e->type() == QEvent::Paint)
	{
		QPainter pa(ptr);
		pa.drawPixmap(ptr->rect(), smallPic[smallSkin.indexOf(ptr)]);

		if (currentSkin == ptr)
		{
			QPainter pa(ptr);
			pa.drawPixmap(QRect(90, 45, 24, 24), QPixmap("data/icon/currentSkin.png"));
		}

		if (hoverWidget==ptr)
		{
			QPainter pa(ptr);
			pa.setBrush(QColor(102, 204, 250, 150));
			pa.drawRect(-1,-1,119,71);
		}

		return true;
	}
	if (e->type() == QEvent::HoverEnter)
	{
		hoverWidget = ptr;
		ptr->repaint();
		return true;
	}
	if (e->type() == QEvent::HoverLeave)
	{
		hoverWidget = nullptr;
		ptr->repaint();
		return true;
	}

	return tranWidget::eventFilter(obj,e);
}

void skinSetter::findCurrent(const QString &route)
{
	currentSkin = smallSkin[allRoute.indexOf(route)];
}

void skinSetter::setCustom()
{
	QFileDialog fd(this);
	fd.setNameFilter("Image Files(*.png *.jpg *.bmp)");
	auto result=fd.exec();
	if (result != QDialog::Accepted)
	{
		return;
	}

	auto fileName = fd.selectedFiles()[0];
	QFileInfo fileRoute(fileName);
	QFile tempFile(fileName);
	fileName = "data/skin/" + fileRoute.completeBaseName() + '.' + fileRoute.suffix();

	if (allRoute.indexOf(fileName) != -1)
	{
		return;
	}

	tempFile.copy(fileName);
	allRoute.push_back(fileName);

	QPixmap currentMap(allRoute.last());

	auto size = getRealPicSize(currentMap.width(), currentMap.height());

	smallPic.push_back(std::move(currentMap.copy((currentMap.width() - size.width) / 2, (currentMap.height() - size.hight) / 2,
		size.width, size.hight).scaled(118, 70)));

	skinWidget->hide();

	QPushButton *pb = new QPushButton(skinWidget);
	smallSkin.push_back(pb);

	pb->setToolTip(u8"设为皮肤");
	pb->setGeometry(5 + ((allRoute.size() - 1) % 4) * 128, 5 + ((allRoute.size() - 1) / 4) * 80, 118, 70);

	pb->installEventFilter(this);
	connect(pb, &QPushButton::clicked, this, &skinSetter::resetCurrent);

	skinWidget->resize(512, (allRoute.size() / 4 + 1) * 80);
	skinWidget->show();

	currentSkin = pb;
	emit pictureSetted(allRoute.last());
}

void skinSetter::loadImages()
{

	QDir dir("data/skin/");
	QStringList filter;
	filter << "*.jpg" << "*.png" << "*.bmp";
	auto allPic = dir.entryInfoList(filter);

	skinWidget->resize(512,(allPic.size()/4+1)*80);

	int index = 0;
	for (auto onePic : allPic)
	{
		allRoute.push_back("data/skin/" + onePic.baseName()+'.'+ onePic.suffix());
		QPixmap currentMap(allRoute.last());

		auto size = getRealPicSize(currentMap.width(), currentMap.height());

		smallPic.push_back(std::move(currentMap.copy((currentMap.width() - size.width) / 2, (currentMap.height() - size.hight) / 2,
			size.width, size.hight).scaled(118,70)));

		QPushButton *pb = new QPushButton(skinWidget);
		smallSkin.push_back(pb);
		pb->installEventFilter(this);
		pb->setGeometry(5 +(index%4)*128,5+(index/4)*80,118,70);
		pb->setToolTip(u8"设为皮肤");
		index++;
		connect(pb, &QPushButton::clicked, this,&skinSetter::resetCurrent);
	}

}

void skinSetter::resetCurrent()
{
	auto ptr = dynamic_cast<QPushButton *>(sender());
	auto lastSkin = currentSkin;
	if (lastSkin)
	{
		lastSkin->update();
	}
	currentSkin = ptr;
	currentSkin->update();

	emit pictureSetted(allRoute[smallSkin.indexOf(currentSkin)]);
}

skinSetter::picSize skinSetter::getRealPicSize(int w, int h)
{
	auto rate = [](double a, double b)->double
	{
		if (b != 0.0)
		{
			return a / b;
		}
		else
		{
			return 0.0;
		}
	};

	auto buttonRate = rate(118, 70);

	double realWidth, realHeight;
	if (rate(w, h)>buttonRate)
	{
		realWidth = w;
		realHeight = realWidth / buttonRate;
	}
	else
	{
		realHeight = h;
		realWidth = realHeight*buttonRate;
	}

	return picSize(realWidth, realHeight);
}
