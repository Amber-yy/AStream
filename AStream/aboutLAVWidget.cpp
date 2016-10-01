#include "aboutLAVWidget.h"

#include <QUrl>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>

aboutLAVWidget::aboutLAVWidget(int w,int h,QWidget *parent):tranWidget(w,h,parent)
{
	try
	{
		describe1 = new QLabel(u8"LAV Filter是一款基于FFMPEG的开源多媒体解码库。",this);
		describe2 = new QLabel(u8"LAV Filter支持几乎所有音频、视频格式。",this);
		officialWeb = new QLabel(u8"项目地址：<a style='color:white;'href=https://github.com/Nevcairiel/LAVFilters>https://github.com/Nevcairiel/LAVFilters</a>", this);
		submit = new QPushButton(u8"确定",this);
		setTitle(u8"关于LAV Filter");
		describe1->setGeometry(10,40,310,20);
		describe2->setGeometry(10, 70, 310, 20);
		officialWeb->setGeometry(10, 100, 300, 20);
		submit->setGeometry(130, 140, 60, 30);

		connect(submit, &QPushButton::clicked, this, &QWidget::close);
		connect(officialWeb, &QLabel::linkActivated, [](QString url) {QDesktopServices::openUrl(QUrl(url)); });
		setColor(QColor(0, 0, 0, 155));

		QFile qss("data/qss/aboutPlayer.qss");
		qss.open(QIODevice::ReadOnly);
		if (qss.isOpen())
		{
			setStyleSheet(qss.readAll());
		}
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}


aboutLAVWidget::~aboutLAVWidget()
{
}
