#pragma once

#include "musicAPI.h"

class kuWoAPI :public musicAPI
{
	Q_OBJECT

public:
	kuWoAPI(QObject *parents = 0);
	kuWoAPI(const kuWoAPI &) = delete;
	virtual ~kuWoAPI();
protected:
	virtual QString playLinkHandle(QNetworkReply *) override;
	virtual QVector<musicInfo> infoHandle(QNetworkReply *) override;
};
