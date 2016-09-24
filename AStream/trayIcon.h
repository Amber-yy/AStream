#pragma once

#include <QSystemTrayIcon>
#include <memory>

class QMenu;
class QPushButton;
class QWidgetAction;
class QAction;
class AStream;
class QSlider;

class trayIcon final:public QSystemTrayIcon
{
	Q_OBJECT
public:
	trayIcon(AStream *parent=0);
	virtual ~trayIcon(){}
	void setText(QString);
	void setVolume(int);
	void setMute(bool);
	void setPause(bool);
	bool getPause();
	bool getIsDesk();
	void setIsDesk(bool);
	bool getLock();
	void setLock(bool);
private:
	void createSubCom();
	void setSubObjectName();
	void loadConfig();
	void createMenu();
	void setCursorStyle();
	void connectSignal();
protected:
	QString style;
	QString showLrcText, hideLrcText;
	QString showLrcIcon, hideLrcIcon;
	QString lrcLockText, lrcUnlockText;
	QString lockAbleIcon, lockUnableIcon, unlockAbleIcon, unlockUnableIcon;
	std::shared_ptr<QMenu> mainMenu;
	std::shared_ptr<QWidget> topWidget;
	std::shared_ptr<QWidget> soundWidget;
	std::shared_ptr<QPixmap> soundHover, soundNormal, soundPress;
	AStream *mainPlayer;
	QPushButton *playBarText;
	QWidgetAction *playBar,*soundBar;
	QAction *exitAc, *setAc,*showAc,*showLrcAc,*lockLrcAc;
	QSlider *volume;
	QPushButton *nextBt,*prevBt,*playBt,*muteBt;
	static const size_t maxSongName=15;
	int volumeNum;
	bool isPause;
	bool isDesk;
	bool isLock;
};
