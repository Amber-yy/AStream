#pragma once

#ifndef ASTREAM_H
#define ASTREAM_H

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include <random>
#include "songWidget.h"
#include "setStructs.h"

#include <QMap>

class lrcDecoder;
class trayIcon;
class songsList;
class searchList;
class lyricsWidget;
class musicAPI;
class QLineEdit;
class QListView;
class QLabel;
class QSlider;
class QMediaPlayer;
class QTimer;
class QMenu;
class QAction;
class screenShoter;
class aboutPlayerWidget;
class aboutKGWidget;
class aboutLAVWidget;
class aboutFFMPEGWidget;
class skinSetter;
class setWidget;

class AStream : public QWidget,public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	AStream(QWidget *parent = 0);
	~AStream();
	void exit();
	void prevSong();
	void pauseSong(bool);
	void playSong(songWidget::songInfo);
	bool getPause();
	void stopSong();
	void nextSong();
	void showSetting();
	void setMute(bool);
	void downLoadSong(songWidget::songInfo);
	bool getMute();
	bool getLike();
	void setLike(bool);
	void setVolume(int);
	void setSkin(QString);
	void setColor(QColor);
	void setSongName(QString);
	void resetProgress();
	void resetPlayBar();
	void showLyrics();
	void showResult();
	bool getIsDesk();
	bool getBlock();
	void setIsDesk(bool);
	bool getLock();
	void setLock(bool);
	int getVolume();
private:
	void setHotkey(hotKeys &);
	void createSubCom();
	void readConfig();
	void setSubObjectName();
	void connectSignal();
	void setPlayMode();
	void setCursor();
	void playHandle(qint64);
	void downLoadHandle(QString);
protected:
	enum playMode
	{
		orderPlay,
		singleLoop,
		listLoop,
		randomPlay
	};
	virtual bool eventFilter(QObject *, QEvent *) override;
	virtual void paintEvent(QPaintEvent *)override;
	virtual void mouseReleaseEvent(QMouseEvent *)override;
	virtual void mousePressEvent(QMouseEvent *)override;
	virtual void mouseMoveEvent(QMouseEvent *)override;
	virtual void closeEvent(QCloseEvent *) override;
	virtual void dragEnterEvent(QDragEnterEvent *)override;
	virtual void dropEvent(QDropEvent *)override;
	virtual bool nativeEventFilter(const QByteArray &, void *, long *)override;
	void setGeneral(generalSet &);
	void setWindowLrc(windowLrc &);
	void setDeskLrc(deskLrc &);
	void setProxy(networkSet &);
protected:
	QMap<int, int> keyMap;
	QPixmap skin;
	QColor currentColor;
	QPoint lastPos;
	QString playBtStyle,likeBtStyle,playModeStyle,soundBtStyle;
	QString lrcDeskStyle;
	QString playBarText;
	QString downLoadRoute,fileName;
	std::random_device numProvider;
	lrcDecoder *lrcProvider;
	QTimer *timer;
	QMediaPlayer *player;
	trayIcon *tray;
	QPushButton *closeBt, *minBt;
	QPushButton *prevBt, *nextBt, *playBt;
	QPushButton *helpBt;
	QPushButton *searchBt;
	QPushButton *likeBt, *downLoadBt, *playModeBt, *soundBt;
	QPushButton *thorBt, *setBt,*skinBt;
	QPushButton *musicLibBt, *lyricsBt;
	QPushButton *deskLyricsBt;
	QLineEdit *searchEditor;
	songsList *playList;
	searchList *searchResult;
	screenShoter *shoter;
	lyricsWidget *lyricsBar;
	setWidget *setter;
	QLabel *songName, *progress,*playerIcon;
	QSlider *songSlider, *volumeSlider;
	musicAPI *musicDownLoader,*musicProvider;
	QMenu *aboutMenu;
	QAction *aboutPlayer;
	QAction *aboutKugou;
	QAction *aboutQt;
	QAction *aboutLAV;
	QAction *aboutFF;
	aboutPlayerWidget *aboutPlayerW;
	aboutKGWidget *aboutKG;
	aboutLAVWidget *aboutLAVW;
	aboutFFMPEGWidget *aboutFFW;
	skinSetter *skinModifier;
	playMode currentMode;
	static const size_t maxSongName=34;
	size_t maxDuration,playProgress,lastUpdateTime;
	int volume;
	int shotKey,prevKey,nextKey,highKey,lowKey,pauseKey,muteKey;
	bool isDeskLrc;
	bool keepTrayIcon;
	bool resetLyrics;
	bool isBlock;
	bool isMute;
	bool isPress;
	bool isLike;
	bool isPause;
	bool isPureColor;
	bool isAutoPlay;
	bool playHello;
	bool isHotKey;
};

#endif // ASTREAM_H
