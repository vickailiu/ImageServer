#include "StdAfx.h"
#include "EchoService.h"

#include "WebsocketClient.h"
#include "WebsocketDataMessage.h"

#include "ImageSearch.h"
#include "DatabaseHandler.h"

EchoService::EchoService(void)
{
}

EchoService::~EchoService(void)
{
}

void EchoService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
{
	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
	WebsocketClient& client = (WebsocketClient&) (*pClient);

	WebsocketDataMessage response(EchoCommunication);
	response.SetArguments(request.GetArg1());

	try {
		//ImageSearch::run();
		DatabaseHandler::buildIndex();
	}
	catch(exception & ex)				// catch any exception, including CdvsException
	{
		cerr << "Exception: " << ex.what() << endl;
	}

	client.PushPacket(&response);
}
