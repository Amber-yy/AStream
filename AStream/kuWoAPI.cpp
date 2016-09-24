#include "kuWoAPI.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>

QVector<musicAPI::musicInfo> kuWoAPI::infoHandle(QNetworkReply *reply)
{
	QByteArray json = reply->readAll();
	int size = json.size();
	for (int i = 0; i < size; i++)
	{
		if (json[i] == '\'')
		{
			json[i] = '"';
		}
	}
	QJsonParseError error;
	QJsonDocument doucoument = QJsonDocument::fromJson(json, &error);
	QJsonObject obj = doucoument.object();

	QJsonArray jsArray = obj.take("abslist").toArray();
	size = jsArray.size();

	QVector<musicInfo> t(size);

	for (int i = 0; i < size; i++)
	{
		t[i].hashCode = jsArray[i].toObject().take("MUSICRID").toString();
		t[i].musicName = jsArray[i].toObject().take("SONGNAME").toString();
		t[i].singer = jsArray[i].toObject().take("ARTIST").toString();
		t[i].fileName = t[i].singer + "-" + t[i].musicName;
	}

	return t;
}

QString kuWoAPI::playLinkHandle(QNetworkReply *reply)
{
	QString t = reply->readAll();
	return t;
}

kuWoAPI::kuWoAPI(QObject *parents) :musicAPI(parents)
{
	searchHead = "http://search.kuwo.cn/r.s?all=";
	searchMid = "&ft=music&itemset=web_2013&client=kt&pn=0&rn=";
	searchEnd = "&rformat=json&encoding=utf8";

	playHead = "http://antiserver.kuwo.cn/anti.s?type=convert_url&rid=";
	playEnd = "&format=mp3&response=url";
}

kuWoAPI::~kuWoAPI()
{

}
