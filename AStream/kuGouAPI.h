#pragma once

#include "musicAPI.h"

class kuGouAPI :public musicAPI
{
	Q_OBJECT

public:
	kuGouAPI(QObject *parents = 0);
	kuGouAPI(const kuGouAPI &) = delete;
	virtual ~kuGouAPI();
protected:
	virtual QString playLinkHandle(QNetworkReply *) override;
	virtual QVector<musicInfo> infoHandle(QNetworkReply *) override;
};
