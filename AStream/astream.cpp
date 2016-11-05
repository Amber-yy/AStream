#include "astream.h"
#include "kuWoAPI.h"
#include "kuGouAPI.h"
#include "lrcDecoder.h"
#include "trayIcon.h"
#include "songsList.h"
#include "searchList.h"
#include "lyricsWidget.h"
#include "screenShoter.h"
#include "aboutPlayerWidget.h"
#include "aboutKGWidget.h"
#include "aboutLAVWidget.h"
#include "aboutFFMPEGWidget.h"
#include "desktopLyrics.h"
#include "skinsetter.h"

#include <QScreen>
#include <QClipBoard>
#include <QDeskTopWidget>
#include <QCloseEvent>
#include <QPushButton>
#include <QSettings>
#include <QPainter>
#include <QLineEdit>
#include <QListView>
#include <QLabel>
#include <QMediaPlayer>
#include <QTimer>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QDropEvent>
#include <QMimeData>

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>

#ifdef WIN32
#include <windows.h>
#endif

AStream::AStream(QWidget *parent)
	: QWidget(parent), keepTrayIcon(true),isMute(false),volume(80),isPress(false),currentMode(orderPlay),isLike(false),isPause(true),isBlock(true),isDeskLrc(true),isPureColor(false)
{
	try
	{

		createSubCom();
		readConfig();
		setSubObjectName();
		connectSignal();
		setCursor();

		player->setVolume(volume);
		
		tray->show();
		setWindowIcon(tray->icon());
		setWindowFlags(Qt::FramelessWindowHint);

	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

AStream::~AStream()
{
	auto id = reinterpret_cast<HWND>(this->winId());
	UnregisterHotKey(id, shotKey);
	UnregisterHotKey(id, prevKey);
	UnregisterHotKey(id, nextKey);
	UnregisterHotKey(id, highKey);
	UnregisterHotKey(id, lowKey);
	UnregisterHotKey(id, pauseKey);
}

void AStream::exit()
{
	keepTrayIcon = false;
	close();
}

void AStream::pauseSong(bool b)
{

	if (isBlock)
	{
		auto song = playList->getCurrentWidget();
		if (song == nullptr)
		{
			return;
		}
		playSong(song->getSong());
		isBlock = false;
	}

	isPause = b;
	
	if (b)
	{
		playBt->setObjectName("playBt");
		timer->stop();
		player->pause();
	}
	else
	{
		playBt->setObjectName("pauseBt");
		player->play();
		timer->start(50);
	}
	playBt->setStyleSheet(playBtStyle);
	if (tray->getPause() != b)
	{
		tray->setPause(b);
	}
}

void AStream::playSong(songWidget::songInfo info)
{
	isBlock = false;
	timer->stop();
	setSongName(info.musicName);
	tray->setText(info.musicName);
	lrcProvider->getLrc(info);
	setLike(info.isLike);
	if (info.isLocal)
	{
		player->setMedia(QUrl(info.playUrl));
	}
	else
	{
		musicProvider->getPlayUrl(info.hashCode);
	}

}

bool AStream::getPause()
{
	return isPause;
}

void AStream::stopSong()
{
	pauseSong(true);
	resetPlayBar();
	isBlock = true;
}

void AStream::prevSong()
{
	if (isBlock)
	{
		auto song = playList->getCurrentWidget();
		if (song == nullptr)
		{
			return;
		}
		playSong(song->getSong());
		isBlock = false;
	}

	timer->stop();
	player->pause();
	if (currentMode == singleLoop)
	{
		player->setPosition(0);
		playProgress = 0;
		lastUpdateTime = 0;
		player->play();
		timer->start(50);
	}
	else 
	{
		auto song = playList->getPrevSong();
		if (song == nullptr)
		{
			isBlock = true;
			return;
		}
		playList->setCurrentWidget(song);
	}

}

void AStream::nextSong()
{
	if (isBlock)
	{
		auto song = playList->getCurrentWidget();
		if (song == nullptr)
		{
			return;
		}
		playSong(song->getSong());
		isBlock = false;
	}

	timer->stop();
	player->pause();
	if (currentMode == singleLoop)
	{
		player->setPosition(0);
		playProgress = 0;
		lastUpdateTime = 0;
		lyricsBar->restartLyrics();
		player->play();
		timer->start(50);
	}
	else if (currentMode == orderPlay)
	{
		auto song = playList->getNextSong();
		if (song == nullptr)
		{
			pauseSong(true);
			resetPlayBar();
			isBlock = true;
			return;
		}
		playList->setCurrentWidget(song);
	}
	else if (currentMode == listLoop)
	{
		auto song = playList->getNextSong();
		if (song == nullptr)
		{
			song = playList->getFirst();
			if (song == nullptr)
			{
				pauseSong(true);
				resetPlayBar();
				isBlock = true;
				return;
			}
		}
		playList->setCurrentWidget(song);
	}
	else if (currentMode == randomPlay)
	{
		auto t = numProvider() % (playList->getSize());
		auto song = playList->getOneWidget(t);
		if (song == nullptr)
		{
			pauseSong(true);
			resetPlayBar();
			isBlock = true;
			return;
		}
		playList->setCurrentWidget(song);
	}

}

void AStream::showSetting()
{

}

void AStream::setMute(bool m)
{
	isMute = m;

	if (m)
	{
		soundBt->setObjectName("muteBt");
	}
	else
	{
		if (volume< 50)
		{
			soundBt->setObjectName("lowVolBt");
		}
		else
		{
			soundBt->setObjectName("highVolBt");
		}
	}
	player->setMuted(m);
	tray->setMute(m);
	soundBt->setStyleSheet(soundBtStyle);
}

void AStream::downLoadSong(songWidget::songInfo song)
{
	if (song.isLocal)
	{
		return;
	}

	fileName = downLoadRoute + song.musicName + '.';
	musicDownLoader->getPlayUrl(song.hashCode);
}

bool AStream::getMute()
{
	return isMute;
}

bool AStream::getLike()
{
	return isLike;
}

void AStream::setVolume(int v)
{
	if (v == volume)
	{
		return;
	}

	if (v >= 0 && v <= 100)
	{
		volume = v;
		tray->setVolume(v);
		if (volumeSlider->value() != v)
		{
			volumeSlider->setValue(v);
		}

		player->setVolume(v);
		if (volume < 50)
		{
			soundBt->setObjectName("lowVolBt");
		}
		else
		{
			soundBt->setObjectName("highVolBt");
		}

		soundBt->setStyleSheet(soundBtStyle);
	}
}

void AStream::setSkin(QString route)
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
	
	skin.load(route);
	double realWidth, realHeight;
	if(rate(skin.width(),skin.height())>rate(width(),height()))
	{ 
		realWidth = skin.width();
		realHeight = realWidth/rate(width(), height());
	}
	else
	{
		realHeight = skin.height();
		realWidth = realHeight*rate(width(), height());
	}
	isPureColor = false;
	skin = std::move(skin.copy((skin.width() - realWidth) / 2, (skin.height() - realHeight) / 2, realWidth, realHeight));
	update();
}

void AStream::setColor(QColor c)
{
	isPureColor = true;
	currentColor = c;
	update();
}

void AStream::setSongName(QString name)
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
			if (i > maxSongName-3)
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

	songName->setText(temp);
}

void AStream::resetProgress()
{
	auto intToString = [this](size_t t)->QString
	{
		size_t min,sec;
		QString minStr,secStr;
		min = t / 60;
		t -= min*60;
		sec = t;

		minStr += QString::number(min);
		if (min<10)
		{
			minStr.push_front('0');
		}

		secStr += QString::number(sec);
		if (sec<10)
		{
			secStr.push_front('0');
		}
		return minStr+':'+secStr;
	};

	playProgress= player->position();
	if (resetLyrics)
	{
		resetLyrics = false;
		lyricsBar->resetProgroess(playProgress);
	}
	else
	{
		lyricsBar->updateLyrics(playProgress);
	}

	if (playProgress - lastUpdateTime >1000)
	{
		lastUpdateTime = playProgress;
		progress->setText(intToString(playProgress/1000) + '/' + intToString(maxDuration/1000));
		songSlider->setValue(playProgress/1000);
	}

	if (playProgress == maxDuration&&maxDuration!=0)
	{
		nextSong();
	}
}

void AStream::resetPlayBar()
{
	songName->setText(playBarText);
	progress->setText("");
	songSlider->setValue(0);
}

void AStream::showLyrics()
{
	searchResult->hide();
	lyricsBar->show();
}

void AStream::showResult()
{
	lyricsBar->hide();
	searchResult->show();
}

bool AStream::getIsDesk()
{
	return isDeskLrc;
}

bool AStream::getBlock()
{
	return isBlock;
}

void AStream::setIsDesk(bool lrc)
{
	isDeskLrc = lrc;
	if (tray->getIsDesk() != lrc)
	{
		tray->setIsDesk(lrc);
	}

	if (isDeskLrc)
	{
		lyricsBar->showDesktop(true);
		deskLyricsBt->setObjectName("lrcShow");
	}
	else
	{
		deskLyricsBt->setObjectName("lrcHide");
		lyricsBar->showDesktop(false);
	}
	deskLyricsBt->setStyleSheet(lrcDeskStyle);
}

bool AStream::getLock()
{
	return lyricsBar->getLock();
}

void AStream::setLock(bool b)
{
	lyricsBar->setLock(b);
	if (tray->getLock()!= b)
	{
		tray->setLock(b);
	}
}

int AStream::getVolume()
{
	return volume;
}

void AStream::loadHotKey()
{
	
	QSettings hotKeys("data/hotKey.ini", QSettings::IniFormat);

	auto getOneKey = [this,&hotKeys](int &oneKey,const QString &keyValue)
	{
		QString result = hotKeys.value(keyValue).toString();
		oneKey = GlobalAddAtomA(result.toStdString().c_str());
		auto metaKeys = result.split('+');
		int ctrlKey = 0;
		char realKey = 'Q';
		for (auto singleKey : metaKeys)
		{
			if (singleKey == "Ctrl")
			{
				ctrlKey |= MOD_CONTROL;
			}
			else if (singleKey == "Alt")
			{
				ctrlKey |= MOD_ALT;
			}
			else if (singleKey == "Shift")
			{
				ctrlKey |= MOD_SHIFT;
			}
			else if (singleKey == "Shift")
			{
				ctrlKey |= MOD_SHIFT;
			}
			else
			{
				realKey = singleKey.toInt();
			}
		}
		auto b = RegisterHotKey(reinterpret_cast<HWND>(this->winId()), oneKey, ctrlKey, realKey);
		if (!b)
		{
			UnregisterHotKey(reinterpret_cast<HWND>(this->winId()), oneKey);
			RegisterHotKey(reinterpret_cast<HWND>(this->winId()), oneKey, ctrlKey, realKey);
		}
	};
	
	getOneKey(shotKey, "Main/shotKey");
	getOneKey(prevKey, "Main/prevKey");
	getOneKey(nextKey, "Main/nextKey");
	getOneKey(highKey, "Main/highKey");
	getOneKey(lowKey, "Main/lowKey");
	getOneKey(pauseKey, "Main/pauseKey");

}

void AStream::createSubCom()
{
	tray = new trayIcon(this);
	closeBt = new QPushButton(this);
	minBt = new QPushButton(this);
	prevBt = new QPushButton(this);
	nextBt = new QPushButton(this);
	playBt = new QPushButton(this);
	searchBt = new QPushButton(this);
	likeBt = new QPushButton(this);
	downLoadBt = new QPushButton(this);
	playModeBt = new QPushButton(this);
	soundBt = new QPushButton(this);
	thorBt = new QPushButton(this);
	setBt = new QPushButton(this);
	skinBt = new QPushButton(this);
	helpBt = new QPushButton(this);
	searchEditor = new QLineEdit(this);
	playList = new songsList(this, this);
	searchResult = new searchList(playList,this);
	songName = new QLabel(this);
	progress = new QLabel(this);
	playerIcon = new QLabel(this);
	songSlider = new QSlider(Qt::Horizontal, this);
	volumeSlider = new QSlider(Qt::Horizontal, this);
	musicDownLoader = new kuGouAPI(this);
	musicProvider = new kuGouAPI(this);
	player = new QMediaPlayer(this);
	timer = new QTimer(this);
	lrcProvider = new lrcDecoder("data/lyrics/",this);
	lyricsBt = new QPushButton(this);
	musicLibBt = new QPushButton(this);
	deskLyricsBt = new QPushButton(this);
	lyricsBar = new lyricsWidget(this);
	shoter = new screenShoter("e:/");

	aboutMenu=new QMenu(this);
	aboutPlayer=new QAction(aboutMenu);
	aboutKugou = new QAction(aboutMenu);
	aboutQt = new QAction(aboutMenu);
	aboutLAV = new QAction(aboutMenu);
	aboutFF = new QAction(aboutMenu);
	aboutPlayerW = new aboutPlayerWidget(420, 310, 0);
	aboutKG = new aboutKGWidget(250, 240, 0);
	aboutLAVW = new aboutLAVWidget(320, 180, 0);
	aboutFFW = new aboutFFMPEGWidget(320, 240, 0);

	skinModifier = new skinSetter(522, 315);
}

void AStream::readConfig()
{
	/*读取配置*/
	QSettings set("data/mainPlayer.ini", QSettings::IniFormat);
	downLoadRoute = set.value("downLoadRoute").toString();
	setWindowTitle(set.value("windowTitle").toString());
	QString style;
	QFile qss(set.value("qssRoute").toString());
	qss.open(QIODevice::ReadOnly);
	if (qss.isOpen())
	{
		style = qss.readAll();
		setStyleSheet(style);
	}
	setSkin(set.value("skin").toString());
	QSettings geo("data/mainGeo.ini", QSettings::IniFormat);
	auto size = set.value("windowSize").toString().split(' ');
	if (size.size() >= 2)
	{
		resize(size[0].toInt(), size[1].toInt());
	}

	songSlider->setRange(0, 100);
	songSlider->setValue(0);
	volumeSlider->setRange(0, 100);
	volumeSlider->setValue(volume);
	playBarText = set.value("playBarText").toString();
	setSongName(playBarText);
	tray->setText(playBarText);
	auto getGeo = [this,&geo,&size](QWidget *widget,QString key)
	{
		size = std::move(geo.value(key).toString().split(' '));
		if (size.size() == 4)
		{
			widget->setGeometry(size[0].toInt(), size[1].toInt(), size[2].toInt(), size[3].toInt());
		}
		else if (size.size() == 5)
		{
				widget->hide();
		}
	};

	getGeo(closeBt,"Main/closeBt");
	getGeo(minBt, "Main/minBt");
	getGeo(prevBt, "Main/prevBt");
	getGeo(nextBt, "Main/nextBt");
	getGeo(playBt, "Main/playBt");
	getGeo(searchBt, "Main/searchBt");
	getGeo(likeBt, "Main/likeBt");
	getGeo(downLoadBt, "Main/downLoadBt");
	getGeo(playModeBt, "Main/playModeBt");
	getGeo(soundBt, "Main/soundBt");
	getGeo(thorBt, "Main/thorBt");
	getGeo(setBt, "Main/setBt");
	getGeo(skinBt, "Main/skinBt");
	getGeo(searchEditor, "Main/searchEditor");
	getGeo(playList, "Main/playList");
	getGeo(searchResult, "Main/searchResult");
	getGeo(songName, "Main/songName");
	getGeo(progress, "Main/progress");
	getGeo(songSlider, "Main/songSlider");
	getGeo(volumeSlider, "Main/volumeSlider");
	getGeo(helpBt, "Main/helpBt");
	getGeo(playerIcon, "Main/playerIcon");
	getGeo(musicLibBt, "Main/musicLibBt");
	getGeo(lyricsBt, "Main/lyricsBt");
	getGeo(lyricsBar, "Main/lyricsBar");
	getGeo(deskLyricsBt, "Main/deskLyricsBt");

	QFile playBtQss(set.value("playBtStyle").toString());
	playBtQss.open(QIODevice::ReadOnly);
	if (playBtQss.isOpen())
	{
		playBtStyle = playBtQss.readAll();
		playBt->setStyleSheet(playBtStyle);
	}

	QFile likeBtQss(set.value("likeBtStyle").toString());
	likeBtQss.open(QIODevice::ReadOnly);
	if (likeBtQss.isOpen())
	{
		likeBtStyle = likeBtQss.readAll();
		likeBt->setStyleSheet(likeBtStyle);
	}

	QFile playModeQss(set.value("playModeBtStyle").toString());
	playModeQss.open(QIODevice::ReadOnly);
	if (playModeQss.isOpen())
	{
		playModeStyle = playModeQss.readAll();
		playModeBt->setStyleSheet(playModeStyle);
	}

	QFile muteBtQss(set.value("soundBtStyle").toString());
	muteBtQss.open(QIODevice::ReadOnly);
	if (muteBtQss.isOpen())
	{
		soundBtStyle = muteBtQss.readAll();
		soundBt->setStyleSheet(soundBtStyle);
	}

	QFile lrcQss("data/qss/desklrc.qss");
	lrcQss.open(QIODevice::ReadOnly);
	if (lrcQss.isOpen())
	{
		lrcDeskStyle = lrcQss.readAll();
		deskLyricsBt->setStyleSheet(lrcDeskStyle);
	}

	setAcceptDrops(true);
	qApp->installNativeEventFilter(this);
	loadHotKey();

	keepTrayIcon = set.value("keepTrayIcon").toInt();
	isMute = set.value("isMute").toInt();
	volume = set.value("volume").toInt();
	shoter->setAutoSave(set.value("autoSave").toInt());
	shoter->setFileRoute(set.value("autoSaveRoute").toString());
	currentMode = static_cast<playMode>(set.value("currentMode").toInt());
	isDeskLrc = set.value("isDeskLrc").toInt();
	setIsDesk(isDeskLrc);
	if (isDeskLrc)
	{
		deskLyricsBt->setObjectName("lrcShow");
	}
	else
	{
		deskLyricsBt->setObjectName("lrcHide");
	}
	setLock(set.value("isDeskLrcLock").toInt());
	searchResult->hide();
	shoter->hide();
	searchResult->setMaxSize(set.value("maxSearchSize").toInt());

	searchBt->setShortcut(Qt::Key_Return);

	aboutPlayer->setText(u8"关于yy播放器");
	aboutQt->setText(u8"关于Qt");
	aboutKugou->setText(u8"关于酷狗");
	aboutLAV->setText(u8"关于LAV Filter");
	aboutFF->setText(u8"关于FFMPEG");

	aboutMenu->addAction(aboutPlayer);
	aboutMenu->addAction(aboutQt);
	aboutMenu->addAction(aboutKugou);
	aboutMenu->addAction(aboutLAV);
	aboutMenu->addAction(aboutFF);
	helpBt->setMenu(aboutMenu);

	aboutPlayerW->hide();
	aboutKG->hide();
	aboutLAVW->hide();
	aboutFFW->hide();
	skinModifier->hide();

}

void AStream::setSubObjectName()
{
	closeBt->setObjectName("closeBt");
	minBt->setObjectName("minBt");
	prevBt->setObjectName("prevBt");
	nextBt->setObjectName("nextBt");
	playBt->setObjectName("playBt");
	searchBt->setObjectName("searchBt");
	likeBt->setObjectName("likeBt");
	downLoadBt->setObjectName("downLoadBt");
	playModeBt->setObjectName("orderPlayBt");
	soundBt->setObjectName("highVolBt");
	thorBt->setObjectName("thorBt");
	setBt->setObjectName("setBt");
	skinBt->setObjectName("skinBt");
	helpBt->setObjectName("helpBt");
	searchEditor->setObjectName("searchEditor");
	playList->setObjectName("playList");
	searchResult->setObjectName("searchResult");
	songName->setObjectName("songName");
	progress->setObjectName("progress");
	playerIcon->setObjectName("playerIcon");
	songSlider->setObjectName("songSlider");
	volumeSlider->setObjectName("volumeSlider");
	musicLibBt->setObjectName("musicLibBt");
	lyricsBt->setObjectName("lyricsBt");
	lyricsBar->setObjectName("lyricsBar");


}

void AStream::connectSignal()
{
	connect(playBt, &QPushButton::clicked, [this]()
	{
		pauseSong(!isPause);
	}
	);
	
	connect(prevBt, &QPushButton::clicked,this,&AStream::prevSong);
	connect(nextBt, &QPushButton::clicked, this, &AStream::nextSong);
	connect(playModeBt, &QPushButton::clicked, this, &AStream::setPlayMode);
	connect(closeBt, &QPushButton::clicked, this, &QWidget::close);
	connect(minBt, &QPushButton::clicked, this, &QWidget::showMinimized);

	connect(likeBt, &QPushButton::clicked, [this]()
	{
		setLike(!isLike);
	}
	);

	connect(volumeSlider, &QSlider::valueChanged, [this](int value)
	{
		if (volume != value)
		{
			setVolume(value);
		}
	}
	);

	connect(songSlider, &QSlider::valueChanged, [this](int value)
	{
		if (!isBlock&&playProgress/1000!=value)
		{
			player->setPosition(value * 1000);
			lyricsBar->resetProgroess(value * 1000);
			resetLyrics = true;
		}
	}
	);

	connect(soundBt, &QPushButton::clicked, [this]()
	{
		setMute(!isMute);
	}
	);

	connect(downLoadBt, &QPushButton::clicked, [this]()
	{
		auto widget = playList->getCurrentWidget();
		if (widget)
		{
			downLoadSong(widget->getSong());
		}
	}
	);

	connect(musicProvider, &musicAPI::playLinkGetted, [this](QString &t)
	{
		player->setMedia(QUrl(t));
	}
	);

	connect(deskLyricsBt, &QPushButton::clicked, [this]()
	{
		setIsDesk(!isDeskLrc);
	}
	);

	connect(searchBt, &QPushButton::clicked, [this]()
	{
		QString key = searchEditor->text();
		if (key.isEmpty())
		{
			return;
		}
		searchResult->search(key);
		showResult();
	}	
	);

	connect(timer, &QTimer::timeout, this, &AStream::resetProgress);
	connect(playList, &songsList::downLoad, this, &AStream::downLoadSong);
	connect(player,&QMediaPlayer::durationChanged,this,&AStream::playHandle);
	connect(musicDownLoader, &musicAPI::playLinkGetted, this, &AStream::downLoadHandle);
	connect(lrcProvider, &lrcDecoder::decodeFinished, lyricsBar, &lyricsWidget::resetLyrics);
	connect(musicLibBt, &QPushButton::clicked, this, &AStream::showResult);
	connect(lyricsBt, &QPushButton::clicked,this,&AStream::showLyrics);

	connect(aboutPlayer, &QAction::triggered, aboutPlayerW, &QWidget::show);
	connect(aboutQt, &QAction::triggered, [this] {QMessageBox::aboutQt(this); });
	connect(aboutKugou, &QAction::triggered, aboutKG, &QWidget::show);
	connect(aboutLAV, &QAction::triggered, aboutLAVW,&QWidget::show);
	connect(aboutFF, &QAction::triggered, aboutFFW, &QWidget::show);
	
	connect(skinModifier, &skinSetter::pictureSetted, this, &AStream::setSkin);
	connect(skinModifier, &skinSetter::colorSetted, this, &AStream::setColor);

	connect(skinBt, &QPushButton::clicked, skinModifier, &QWidget::show);

	songSlider->installEventFilter(this);
	volumeSlider->installEventFilter(this);
}

void AStream::setLike(bool like)
{
	isLike = like;
	if (like)
	{
		likeBt->setObjectName("ulikeBt");
	}
	else
	{
		likeBt->setObjectName("likeBt");
	}
	likeBt->setStyleSheet(likeBtStyle);
	playList->setLike(like);
}

void AStream::setPlayMode()
{
	if (currentMode == randomPlay)
	{
		currentMode = orderPlay;
	}
	else
	{
		currentMode=static_cast<playMode>(currentMode+1);
	}

	if (currentMode == orderPlay)
	{
		playModeBt->setObjectName("orderPlayBt");
	}
	else if (currentMode == singleLoop)
	{
		playModeBt->setObjectName("singleLoopBt");
	}
	else if (currentMode == listLoop)
	{
		playModeBt->setObjectName("listLoopBt");
	}
	else if (currentMode == randomPlay)
	{
		playModeBt->setObjectName("randomPlayBt");
	}

	playModeBt->setStyleSheet(playModeStyle);
}

void AStream::setCursor()
{

	closeBt->setCursor(Qt::PointingHandCursor);
	minBt->setCursor(Qt::PointingHandCursor);
	prevBt->setCursor(Qt::PointingHandCursor);
	nextBt->setCursor(Qt::PointingHandCursor);
	playBt->setCursor(Qt::PointingHandCursor);
	helpBt->setCursor(Qt::PointingHandCursor);
	searchBt->setCursor(Qt::PointingHandCursor);
	likeBt->setCursor(Qt::PointingHandCursor);
	downLoadBt->setCursor(Qt::PointingHandCursor);
	playModeBt->setCursor(Qt::PointingHandCursor);
	soundBt->setCursor(Qt::PointingHandCursor);
	thorBt->setCursor(Qt::PointingHandCursor);
	setBt->setCursor(Qt::PointingHandCursor);
	skinBt->setCursor(Qt::PointingHandCursor);
	songSlider->setCursor(Qt::PointingHandCursor);
	volumeSlider->setCursor(Qt::PointingHandCursor);

}

void AStream::playHandle(qint64 duration)
{
	if (isPause)
	{
		isPause = false;
		playBt->setObjectName("pauseBt");
		playBt->setStyleSheet(playBtStyle);
	}
	player->pause();
	timer->stop();
	if (tray->getPause())
	{
		tray->setPause(false);
	}
	maxDuration = duration;
	songSlider->setRange(0, maxDuration/1000);
	playProgress = 0;
	lastUpdateTime = 0;
	player->play();
	timer->start(50);
	lyricsBar->resetProgroess(0);
	lyricsBar->setMaxDuration(maxDuration);
	resetLyrics = true;
}

void AStream::downLoadHandle(QString url)
{
	fileName += url.split('.').last();

	musicDownLoader->downLoad(url, fileName);
}

bool AStream::eventFilter(QObject *obj, QEvent *e)
{
	auto sliderHandle = [&e,this](QSlider *sli,int min)->bool
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent *>(e);
		if (mouse->button() == Qt::LeftButton)
		{
			double x = mouse->x();
			double w = sli->width();
			double vo = x / w * sli->maximum();
			if (std::abs(vo - sli->value())>=min)
			{
				sli->setValue(vo);
				e->accept();
				return true;
			}
		}
		return false;
	};

	QEvent::Type t = e->type();
	if (obj == songSlider)
	{
		if (t == QEvent::MouseButtonPress)
		{
			if (sliderHandle(songSlider,10))
			{
				resetLyrics = true;
				return true;
			}
		}
	}
	else if (obj == volumeSlider)
	{
		if (t == QEvent::MouseButtonPress)
		{
			if (sliderHandle(volumeSlider,8))
			{
				return true;
			}
		}
	}

	return QWidget::eventFilter(obj,e);
}

void AStream::paintEvent(QPaintEvent *e)
{
	if (isPureColor)
	{
		QPainter pa(this);
		pa.fillRect(rect(), QBrush(currentColor, Qt::SolidPattern));
	}
	else
	{
		QPainter pa(this);
		pa.drawPixmap(rect(), skin);
	}

	QPainter r(this);
	r.fillRect(QRect(330, 70, 540, 40), QBrush(QColor(100, 100, 100, 200), Qt::SolidPattern));
}

void AStream::mouseReleaseEvent(QMouseEvent *event)
{
	isPress = false;
}

void AStream::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->globalPos();
	isPress = true;
}

void AStream::mouseMoveEvent(QMouseEvent *event)
{
	if (isPress)
	{
		int dx = event->globalX() - lastPos.x();
		int dy = event->globalY() - lastPos.y();
		lastPos = event->globalPos();
		move(x() + dx, y() + dy);
	}
}

void AStream::closeEvent(QCloseEvent * e)
{

	if (keepTrayIcon)
	{
		this->hide();
		e->ignore();
	}
	/*
#ifdef _WIN32
	int w = width();
	int h = height();

	int dx = w / 100;
	int dy = h / 100;

	int  xcoord= x();
	int ycoord = y();

	for (int i = 0; i < 100; i++)
	{
		move(xcoord+i*dx/2,ycoord+i*dy/2);
		resize(width()-dx,height()-dy);
		Sleep(1);
	}
#endif
	*/
}

void AStream::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void AStream::dropEvent(QDropEvent *event)
{
	auto file= event->mimeData()->urls()[0].toLocalFile();

	songWidget::songInfo info;
	info.isLike = false;
	info.isLocal = true;
	info.playUrl = file;
	info.musicName = file.split('/').last();

	playList->setCurrentWidget(playList->addSong(info));
}

bool AStream::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
	MSG *msg = static_cast<MSG *>(message);
	if (msg ->message != WM_HOTKEY)
	{
		return false;
	}

	auto parameter = msg->wParam;
	if (parameter == shotKey)
	{
		shoter->show();
		return true;
	}

	if (parameter == prevKey)
	{
		prevSong();
		return true;
	}

	if (parameter == nextKey)
	{
		nextSong();
		return true;
	}

	if (parameter == highKey)
	{
		setVolume(volume + 10);
		return true;
	}

	if (parameter == lowKey)
	{
		setVolume(volume - 10);
		return true;
	}

	if (parameter == pauseKey)
	{
		pauseSong(!isPause);
		return true;
	}

	return false;
}
