#include "aboutPlayerWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>

#include <QDebug>

aboutPlayerWidget::aboutPlayerWidget(int w, int h, QWidget * parent):tranWidget(w,h,parent)
{
	try
	{
		QSettings set("data/aboutPlayer.ini",QSettings::IniFormat);
		auto num = set.value("Main/size").toString().split(' ');

		QSize size(num[0].toInt(), num[1].toInt());
		int width = num[0].toInt();
		setMaximumSize(size);
		setMinimumSize(size);

		promt=new QLabel(this);
		email = new QLabel(this);
		qq = new QLabel(this);
		github = new QLabel(this);
		version = new QLabel(this);
		releaseDate = new QLabel(this);
		thanks = new QLabel(this);
		thanks1 = new QLabel(this);
		thanks2 = new QLabel(this);
		 thanks3 = new QLabel(this);
		submit = new QPushButton(this);

		promt->setGeometry(3, 50, width, 20);
		email->setGeometry(3, 70, width, 20);
		qq->setGeometry(3, 90, width, 20);
		github->setGeometry(3, 110, width, 20);
		version->setGeometry(3, 130, width, 20);
		releaseDate->setGeometry(3, 150, width, 20);
		thanks->setGeometry(3, 190, width, 20);
		thanks1->setGeometry(3,210, width, 20);
		thanks2->setGeometry(3, 230, width, 20);
		thanks3->setGeometry(3, 250, width, 20);
		submit->setGeometry(180, 275, 60,30);

		setTitle(set.value("Main/title").toString());
		num = set.value("Main/color").toString().split(' ');
		QColor c(num[0].toInt(), num[1].toInt(), num[2].toInt(), num[3].toInt());
		setColor(c);
		promt->setText(set.value("Main/promtWord").toString());
		email->setText(set.value("Main/emailWord").toString());
		qq->setText(set.value("Main/qqWord").toString());
		github->setText(u8"ÏîÄ¿µØÖ·£º<a style='color: white;' href =https://github.com/Amber-yy/AStream>https://github.com/Amber-yy/AStream</a>");
		version->setText(set.value("Main/versionWord").toString());
		releaseDate->setText(set.value("Main/releaseWord").toString());
		thanks->setText(set.value("Main/thanksWord").toString());
		thanks1->setText(set.value("Main/thanks1Word").toString());
		thanks2->setText(set.value("Main/thanks2Word").toString());
		thanks3->setText(set.value("Main/thanks3Word").toString());
		submit->setText(set.value("Main/submitWord").toString());

		connect(submit, &QPushButton::clicked, this, &QWidget::hide);
		connect(github, &QLabel::linkActivated, [](QString url) {QDesktopServices::openUrl(QUrl(url)); });

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

aboutPlayerWidget::~aboutPlayerWidget()
{
}
