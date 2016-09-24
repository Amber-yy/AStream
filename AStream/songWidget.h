#pragma once

#include <QWidget>

class songsList;
class QLabel;
class QPushButton;

class songWidget :public QWidget
{
	Q_OBJECT
public:
	struct songInfo
	{
		QString musicName;
		QString playUrl;
		QString hashCode;
		size_t duration;
		bool isLike;
		bool isLocal;
	};
	songWidget(songInfo, QVector<QRect> &, QWidget *parents = 0);
	virtual ~songWidget();
	void setSongName(QString);
	void setLike(bool);
	bool getLike();
	void setCurrent();
	void unSetCurrent();
	songInfo& getSong();
	static songsList *mainList;
private:
	void connectSignal();
	void setCursor();
protected:
	static const size_t maxSongName = 24;
	songInfo song;
	QLabel *songName;
	QPushButton *addBt;
	QPushButton *deleteBt;
	QPushButton *downLoadBt;
	QPushButton *likeBt;
	QPushButton *playBt;
};