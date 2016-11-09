#include "musicAPI.h"
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QTimer>

#include <QDebug>
#include <QMessageBox>

musicAPI::musicAPI(QObject *p) :QObject(p),re(nullptr)
{
	netManager = std::make_shared<QNetworkAccessManager>();
	netRequest = std::make_shared<QNetworkRequest>();
	timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(netManager.get(), &QNetworkAccessManager::finished, this, &musicAPI::replyHandle);
	connect(timer, &QTimer::timeout, [this]()
	{
		re->abort();
		emit outOfTime();
	}
	);
}

void musicAPI::replyHandle(QNetworkReply *reply)
{
	timer->stop();
	re = nullptr;
	if (reply->bytesAvailable() == 0)
	{
		emit outOfTime();
	}

	if (flag == getinfo)
	{
		emit infoGetted(infoHandle(reply));
	}
	else if (flag == geturl)
	{
		emit playLinkGetted(playLinkHandle(reply));
	}
	else if (flag == download)
	{
		QFile music(fileRoute);
		music.open(QIODevice::WriteOnly);
		music.write(reply->readAll());
		music.close();
		emit downLoadFinished();
	}
}

void musicAPI::downLoad(const QString & url, const QString &route)
{
	if (re != nullptr)
	{
		emit isBusy();
		return;
	}
	timer->start(shotTime);
	fileRoute = route;
	flag = download;
	netRequest->setUrl(QUrl(url));
	re=netManager->get(*netRequest);
}

QString musicAPI::lrcUrlConnect(const QString & hash)
{
	return lrcHead + hash + lrcEnd;
}

void musicAPI::getPlayUrl(const QString &keyWords)
{
	if (re != nullptr)
	{
		emit isBusy();
		return;
	}
	timer->start(shotTime);
	flag = geturl;
	QString url = playHead + keyWords + playEnd;
	netRequest->setUrl(QUrl(url));
	re=netManager->get(*netRequest);
}

void musicAPI::getAllInfo(const QString &keyWords, size_t maxNum)
{
	if (re != nullptr)
	{
		emit isBusy();
		return;
	}
	timer->start(shotTime);
	flag = getinfo;
	QString url = searchHead + keyWords + searchMid + QString::number(maxNum) + searchEnd;
	netRequest->setUrl(QUrl(url));
	re=netManager->get(*netRequest);
	
}

void musicAPI::setProxy(QNetworkProxy & t)
{
	netManager->setProxy(t);
}
