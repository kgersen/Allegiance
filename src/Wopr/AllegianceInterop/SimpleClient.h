#pragma once

#include "messagecore.h"


class SimpleClient : public IFedMessagingSite
{
public:
	SimpleClient();

	// Inherited via IFedMessagingSite
	virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) override;
};

