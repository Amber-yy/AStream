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
	QJsonObject obj = doucoument.object();//响应播放请求

	return obj.take("url").toString();//解析播放链接JSON
}

QVector<musicAPI::musicInfo> kuGouAPI::infoHandle(QNetworkReply *reply)
{
	if (!reply->isOpen())
	{
		reply->open(QIODevice::ReadOnly);
	}

	QByteArray json = reply->readAll();
	int size;
	json.chop(1);//从尾部删除一个字符
	json.remove(0, 17);//从头部删除一段字符
	QJsonParseError error;
	QJsonDocument doucoument = QJsonDocument::fromJson(json, &error);
	QJsonObject obj = doucoument.object();//字符串转换为JSON对象

	QJsonArray jsArray = obj.take("data").toObject().take("info").toArray();//data里是有用的数据
	size = jsArray.size();//数据条数

	QVector<musicInfo> t(size);

	for (int i = 0; i < size; i++)//遍历整个JSON
	{
		t[i].hashCode = jsArray[i].toObject().take("hash").toString();//取出其中键为hash的值
		if (t[i].hashCode == "")
		{
			t[i].hashCode = jsArray[i].toObject().take("320hash").toString();
		}
		t[i].musicName = jsArray[i].toObject().take("songname").toString();
		t[i].singer = jsArray[i].toObject().take("singername").toString();
		t[i].fileName = t[i].singer + "-" + t[i].musicName;//将歌手名和音乐名拼接成文件名
		t[i].duration = jsArray[i].toObject().take("duration").toInt();//获取时长
	}

	return t;
}
