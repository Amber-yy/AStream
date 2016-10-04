#include "aboutFFMPEGWidget.h"

#include <QUrl>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>

aboutFFMPEGWidget::aboutFFMPEGWidget(int w,int h,QWidget *parent):tranWidget(w,h,parent)
{
	try
	{
		ffmpegIcon=new QLabel(this);
		describe=new QLabel(u8"FFmpeg��һ�׿���������¼��ת��������Ƶ����Ƶ��\n���ܽ���ת��Ϊ���Ŀ�Դ���������",this);
		officialWeb=new QLabel(u8"�ٷ���վ��<a style='color:white' href=https://ffmpeg.org>https://ffmpeg.org/</a>",this);
		submit=new QPushButton(u8"ȷ��",this);
		setTitle(u8"����FFMPEG");
		ffmpegIcon->setGeometry(128, 50, 64, 64);
		describe->setGeometry(10, 120, 310, 40);
		officialWeb->setGeometry(10, 165, 290, 20);
		submit->setGeometry(130, 200, 60, 30);
		ffmpegIcon->setPixmap(QPixmap("data/icon/favicon.ico"));

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


aboutFFMPEGWidget::~aboutFFMPEGWidget()
{
}
