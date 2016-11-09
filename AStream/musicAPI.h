#pragma once

#include <memory>
#include <QString>
#include <QObject>

class QTimer;
class QNetworkReply;
class QNetworkAccessManager;
class QNetworkRequest;

#include <QNetworkProxy>

class musicAPI :public QObject
{
	Q_OBJECT

public:
	struct musicInfo
	{
		QString fileName;
		QString musicName;
		QString singer;
		QString hashCode;
		size_t duration;
	};
	enum callFlag
	{
		download,
		geturl,
		getinfo
	};
	musicAPI(QObject *parents = 0);
	musicAPI(const musicAPI &) = delete;
	virtual ~musicAPI() {}
	virtual void downLoad(const QString &, const QString &) final;
	virtual void getPlayUrl(const QString&) final;
	virtual void getAllInfo(const QString &, size_t) final;
	virtual QString lrcUrlConnect(const QString &);
	virtual void setProxy(QNetworkProxy &t);
signals:
	void infoGetted(QVector<musicInfo>);
	void downLoadFinished();
	void playLinkGetted(QString);
	void isBusy();
	void outOfTime();
protected:
	static const size_t shotTime = 5000;
	virtual void replyHandle(QNetworkReply *) final;
	virtual QString playLinkHandle(QNetworkReply *) = 0;
	virtual QVector<musicInfo> infoHandle(QNetworkReply *) = 0;
	QString searchHead, searchMid, searchEnd;
	QString playHead, playEnd;
	QString lrcHead,lrcEnd;
	QString fileRoute;
	std::shared_ptr<QNetworkAccessManager> netManager;
	std::shared_ptr<QNetworkRequest> netRequest;
	QNetworkReply *re;
	QTimer *timer;
	callFlag flag;
};
