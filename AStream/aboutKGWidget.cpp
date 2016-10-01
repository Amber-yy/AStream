#include "aboutKGWidget.h"

#include <QUrl>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>

aboutKGWidget::aboutKGWidget(int w, int h, QWidget * parent) :tranWidget(w, h, parent)
{
	try
	{
		kgIcon=new QLabel(this);
		describe=new QLabel(u8"�ṷ������һ�������������ֲ�����",this);
		officialWeb=new QLabel(u8"�ٷ���վ��<a style='color: white;' href =http://www.kugou.com>http://www.kugou.com/</a>",this);
		submit = new QPushButton(u8"ȷ��",this);
		setTitle(u8"���ڿṷ");
		kgIcon->setGeometry(101, 50, 48, 48);
		kgIcon->setPixmap(QPixmap("data/icon/kugou.ico"));
		describe->setGeometry(10, 120, 240, 20);
		officialWeb->setGeometry(10, 160, 240, 20);
		submit->setGeometry(95, 200, 60, 30);

		connect(submit, &QPushButton::clicked, this, &QWidget::close);
		connect(officialWeb, &QLabel::linkActivated, [](QString url) {QDesktopServices::openUrl(QUrl(url)); });
		setColor(QColor(0,0,0,155));

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


aboutKGWidget::~aboutKGWidget()
{
}
