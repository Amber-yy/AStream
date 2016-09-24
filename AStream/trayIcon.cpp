#include "trayIcon.h"
#include "astream.h"

#include <QMenu>
#include <QEvent>
#include <QSettings>
#include <QFile>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidgetAction>
#include <QAction>
#include <QSlider>
#include <QPainter>

#include <QLabel>
#include<QDebug>
#include <QMessageBox>

trayIcon::trayIcon(AStream *parent) :QSystemTrayIcon(parent),mainPlayer(parent),isPause(true)
{
	try
	{
		createSubCom();
		setSubObjectName();
		loadConfig();
		createMenu();
		setCursorStyle();
		connectSignal();
	}
	catch (std::bad_alloc &)
	{
		abort();
	}

}

void trayIcon::setText(QString name)
{
	size_t asciiNum = 0, chineseNum = 0;

	for (auto s : name)
	{
		if (s.isLower() || s.isNumber() || s.isUpper())
		{
			asciiNum++;
		}
		else
		{
			chineseNum++;
		}
	}

	size_t length = asciiNum + 2 * chineseNum;
	QString temp;

	if (length > maxSongName)
	{
		int i = 0;

		for (auto s : name)
		{
			if (s.isLower() || s.isNumber() || s.isUpper())
			{
				temp.push_back(s);
				i++;
			}
			else
			{
				temp.push_back(s);
				i += 2;
			}
			if (i > maxSongName - 3)
			{
				break;
			}
		}

		temp.push_back("...");

	}
	else
	{
		temp = name;
	}

	playBarText->setText(temp);
	playBarText->setToolTip(name);
}

void trayIcon::setVolume(int v)
{
	if (v ==volumeNum)
	{
		return;
	}

	if (v >= 0 && v <= 100)
	{
		volume->setValue(v);
		volumeNum = v;
		if (v< 50)
		{
			muteBt->setObjectName("lowVolButton");
		}
		else
		{
			muteBt->setObjectName("highVolButton");
		}
		mainMenu->setStyleSheet(style);
		mainPlayer->setVolume(v);
	}
}

void trayIcon::setMute(bool m)
{
	if (!m)
	{
		if (mainPlayer->getVolume() < 50)
		{
			muteBt->setObjectName("lowVolButton");
		}
		else
		{
			muteBt->setObjectName("highVolButton");
		}
	}
	else
	{
		muteBt->setObjectName("muteButton");
	}
	mainMenu->setStyleSheet(style);
}

void trayIcon::setPause(bool b)
{
	isPause = b;
	if (b)
	{
		playBt->setObjectName("playButton");
	}
	else
	{
		playBt ->setObjectName("pauseButton");
	}
	mainMenu->setStyleSheet(style);
	if (mainPlayer->getPause() != b)
	{
		mainPlayer->pauseSong(b);
	}
}

bool trayIcon::getPause()
{
	return isPause;
}

bool trayIcon::getIsDesk()
{
	return isDesk;
}

void trayIcon::setIsDesk(bool b)
{
	isDesk = b;
	if (mainPlayer->getIsDesk() != b)
	{
		mainPlayer->setIsDesk(b);
	}
	
	if (isDesk)
	{
		showLrcAc->setIcon(QIcon(showLrcIcon));
		showLrcAc->setText(showLrcText);
		lockLrcAc->setEnabled(true);
		if (mainPlayer->getLock())
		{
			lockLrcAc->setIcon(QIcon(unlockAbleIcon));
		}
		else
		{
			lockLrcAc->setIcon(QIcon(unlockAbleIcon));
		}
	}
	else
	{
		showLrcAc->setIcon(QIcon(hideLrcIcon));
		showLrcAc->setText(hideLrcText);
		lockLrcAc->setEnabled(false);
		if (mainPlayer->getLock())
		{
			lockLrcAc->setIcon(QIcon(unlockUnableIcon));
		}
		else
		{
			lockLrcAc->setIcon(QIcon(lockUnableIcon));
		}
	}

}

bool trayIcon::getLock()
{
	return isLock;
}

void trayIcon::setLock(bool b)
{
	isLock = b;
	if (mainPlayer->getLock() != b)
	{
		mainPlayer->setLock(b);
	}

	if (isDesk)
	{
		if (isLock)
		{
			lockLrcAc->setIcon(QIcon(unlockAbleIcon));
			lockLrcAc->setText(lrcUnlockText);
		}
		else
		{
			lockLrcAc->setIcon(QIcon(lockAbleIcon));
			lockLrcAc->setText(lrcLockText);
		}
	}
	else
	{
		if (isLock)
		{
			lockLrcAc->setIcon(QIcon(unlockUnableIcon));
			lockLrcAc->setText(lrcUnlockText);
		}
		else
		{
			lockLrcAc->setIcon(QIcon(lockUnableIcon));
			lockLrcAc->setText(lrcLockText);
		}
	}

}

void trayIcon::createSubCom()
{
	mainMenu = std::make_shared<QMenu>();
	topWidget = std::make_shared<QWidget>();
	soundWidget = std::make_shared<QWidget>();
	playBar = new QWidgetAction(mainMenu.get());
	soundBar = new QWidgetAction(mainMenu.get());
	nextBt = new QPushButton(topWidget.get());
	playBt = new QPushButton(topWidget.get());
	prevBt = new QPushButton(topWidget.get());
	playBarText = new QPushButton(topWidget.get());
	muteBt = new QPushButton(soundWidget.get());
	setAc = new QAction(mainMenu.get());
	exitAc = new QAction(mainMenu.get());
	showAc = new QAction(mainMenu.get());
	volume = new QSlider(Qt::Horizontal, soundWidget.get());
	showLrcAc = new QAction(mainMenu.get());
	lockLrcAc= new QAction(mainMenu.get());

	QVBoxLayout *topLayout = new QVBoxLayout(topWidget.get());
	QHBoxLayout *btLayout = new QHBoxLayout(topWidget.get());
	QHBoxLayout *soundLayout = new QHBoxLayout(soundWidget.get());

	btLayout->addWidget(prevBt);
	btLayout->addWidget(playBt);
	btLayout->addWidget(nextBt);
	topLayout->addLayout(btLayout);
	topLayout->addWidget(playBarText);
	topWidget->setLayout(topLayout);
	soundLayout->addWidget(muteBt);
	soundLayout->addWidget(volume);
	soundWidget->setLayout(soundLayout);

}

void trayIcon::setSubObjectName()
{
	playBt->setObjectName("playButton");
	prevBt->setObjectName("prevButton");
	nextBt->setObjectName("nextButton");
	playBarText->setObjectName("textButton");
	topWidget->setObjectName("topWidget");
	volume->setObjectName("volumeSlider");
}

void trayIcon::loadConfig()
{
	QSettings set("data/trayIcon.ini", QSettings::IniFormat);
	setIcon(QIcon(set.value("icon").toString()));

	QFile qss(set.value("qssRoute").toString());
	qss.open(QIODevice::ReadOnly);
	if (qss.isOpen())
	{
		style = qss.readAll();
		mainMenu->setStyleSheet(style);
	}
	setText(set.value("playBarText").toString());
	setAc->setIcon(QIcon(set.value("setIcon").toString()));
	setAc->setText(set.value("setText").toString());
	exitAc->setIcon(QIcon(set.value("exitIcon").toString()));
	exitAc->setText(set.value("exitText").toString());
	showAc->setIcon(QIcon(set.value("showMainIcon").toString()));
	showAc->setText(set.value("showMainText").toString());

	showLrcText = set.value("lrcTextShow").toString();
	hideLrcText = set.value("lrcTextHide").toString();
	showLrcIcon= set.value("lrcIconShow").toString(); 
	hideLrcIcon = set.value("lrcIconHide").toString();
	showLrcAc->setIcon(QIcon(showLrcIcon));
	showLrcAc->setText(showLrcText);

	lrcLockText = set.value("lrcLockText").toString();
	lrcUnlockText = set.value("lrcunlockText").toString();

	lockAbleIcon = set.value("lrcLockAble").toString(); 
	lockUnableIcon = set.value("lrcLockDisable").toString();
	unlockAbleIcon = set.value("lrcUnlockAble").toString();
	unlockUnableIcon = set.value("lrcUnlockDisable").toString();

	lockLrcAc->setText(lrcLockText);
	lockLrcAc->setIcon(QIcon(lockAbleIcon));
	isLock = false;

	setToolTip(set.value("toolTip").toString());

	auto size = set.value("muteBtSize").toString().split(' ');

	if (size.size() >= 2)
	{
		muteBt->resize(size[0].toInt(), size[1].toInt());

	}
	soundNormal = std::make_shared<QPixmap>(set.value("soundNormal").toString());
	soundHover = std::make_shared<QPixmap>(set.value("soundHover").toString());
	soundPress = std::make_shared<QPixmap>(set.value("soundPress").toString());

	volume->setRange(0, 100);
	volume->setValue(mainPlayer->getVolume());
	volumeNum= mainPlayer->getVolume();

	if (volumeNum< 50)
	{
		muteBt->setObjectName("lowVolButton");
	}
	else
	{
		muteBt->setObjectName("highVolButton");
	}

}

void trayIcon::createMenu()
{
	playBar->setDefaultWidget(topWidget.get());
	soundBar->setDefaultWidget(soundWidget.get());
	mainMenu->addAction(playBar);
	mainMenu->addSeparator();
	mainMenu->addAction(soundBar);
	mainMenu->addAction(showLrcAc);
	mainMenu->addAction(lockLrcAc);
	mainMenu->addAction(setAc);
	mainMenu->addAction(showAc);
	mainMenu->addAction(exitAc);
	setContextMenu(mainMenu.get());
}

void trayIcon::setCursorStyle()
{
	playBt->setCursor(Qt::PointingHandCursor);
	prevBt->setCursor(Qt::PointingHandCursor);
	nextBt->setCursor(Qt::PointingHandCursor);
	volume->setCursor(Qt::PointingHandCursor);
	muteBt->setCursor(Qt::PointingHandCursor);
}

void trayIcon::connectSignal()
{
	connect(playBt, &QPushButton::clicked, [this]()
	{
		setPause(!isPause);
	}
	);

	connect(this, &QSystemTrayIcon::activated, [this](ActivationReason reason)
	{
		if (reason == QSystemTrayIcon::Trigger)
		{
			mainPlayer->show();
		}
	}
	);

	connect(exitAc, &QAction::triggered, [this]()
	{
		mainPlayer->exit();
	}
	);

	connect(muteBt, &QPushButton::clicked, [this]()
	{
		mainPlayer->setMute(!mainPlayer->getMute());
		mainMenu->setStyleSheet(style);
	}
	);

	connect(volume, &QSlider::valueChanged, [this](int value)
	{
		if (volumeNum != value)
		{
			setVolume(value);
		}
	}
	);

	connect(showLrcAc, &QAction::triggered, [this]()
	{
		setIsDesk(!isDesk);
	}
	);

	connect(lockLrcAc, &QAction::triggered, [this]()
	{
		setLock(!isLock);
	}
	);

	connect(showAc, &QAction::triggered, mainPlayer, &QWidget::show);
	connect(setAc, &QAction::triggered, mainPlayer, &AStream::showSetting);

	connect(prevBt, &QPushButton::clicked, mainPlayer, &AStream::prevSong);
	connect(nextBt, &QPushButton::clicked, mainPlayer, &AStream::nextSong);
}
