#pragma once

#include <QString>

struct generalSet
{
	bool autoPlay;
	bool playHello;
	bool keepIcon;
	bool isDefault;
	bool autoSave;
	QString lrcRoute;
	QString downLoadRoute;
	QString saveRoute;
};

struct hotKeys
{
	hotKeys()
	{
		playKeys.resize(2);
		prevKeys.resize(2);
		nextKeys.resize(2);
		higherKeys.resize(2);
		lowerKeys.resize(2);
		muteKeys.resize(2);
		screenShotKeys.resize(2);
	}
	QString playPause;
	QVector<int> playKeys;
	QString prevSong;
	QVector<int> prevKeys;
	QString nextSong;
	QVector<int> nextKeys;
	QString higher;
	QVector<int> higherKeys;
	QString lower;
	QVector<int> lowerKeys;
	QString mute;
	QVector<int> muteKeys;
	QString screenShot;
	QVector<int> screenShotKeys;
	bool isHotKey;
};

struct windowLrc
{
	enum fontType
	{
		none,
		bond,
		italic,
		italicBond
	};
	QString fontName;
	QColor unPlay;
	QColor played;
	fontType type;
	int pixelSize;
};

struct deskLrc
{
	QString fontName;
	QColor unPlay[3];
	QColor played[3];
	windowLrc::fontType type;
	int pixelSize;
};

struct networkSet
{
	QString address;
	QString port;
	QString userName;
	QString passWord;
	int proxyType;
	int resultNum;
};