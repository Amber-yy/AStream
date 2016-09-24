#include "kuGouAPI.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>

kuGouAPI::kuGouAPI(QObject *parents) :musicAPI(parents)
{
	searchHead = "http://mobilecdn.kugou.com/api/v3/search/song?format=jsonp&keyword=";
	searchMid = "&page=0&pagesize=";
	searchEnd = "&showtype=1&callback=kgJSONP238513750";

	playHead = "http://m.kugou.com/app/i/getSongInfo.php?hash=";
	playEnd = "&cmd=playInfo";

	lrcHead = "http://m.kugou.com/app/i/krc.php?cmd=100&keyword=music&hash=";

	lrcEnd = "&timelength=246000&d=0.5261128980200738";

}

kuGouAPI::~kuGouAPI()
{

}

QString kuGouAPI::playLinkHandle(QNetworkReply *reply)
{
	QByteArray json = reply->readAll();
	QJsonParseError error;
	QJsonDocument doucoument = QJsonDocument::fromJson(json, &error);
	QJsonObject obj = doucoument.object();

	return obj.take("url").toString();
}

QVector<musicAPI::musicInfo> kuGouAPI::infoHandle(QNetworkReply *reply)
{
	QByteArray json = reply->readAll();
	int size;
	json.chop(1);
	json.remove(0, 17);
	QJsonParseError error;
	QJsonDocument doucoument = QJsonDocument::fromJson(json, &error);
	QJsonObject obj = doucoument.object();

	QJsonArray jsArray = obj.take("data").toObject().take("info").toArray();
	size = jsArray.size();

	QVector<musicInfo> t(size);

	for (int i = 0; i < size; i++)
	{
		t[i].hashCode = jsArray[i].toObject().take("hash").toString();
		if (t[i].hashCode == "")
		{
			t[i].hashCode = jsArray[i].toObject().take("320hash").toString();
		}
		t[i].musicName = jsArray[i].toObject().take("songname").toString();
		t[i].singer = jsArray[i].toObject().take("singername").toString();
		t[i].fileName = t[i].singer + "-" + t[i].musicName;
		t[i].duration = jsArray[i].toObject().take("duration").toInt();
	}

	return t;
}
