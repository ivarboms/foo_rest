#include "stdafx.h"

#include "ServerProxy.h"

#include "Server.h"


ServerProxy::ServerProxy()
	: server(std::make_unique<Server>())
{
}

ServerProxy::~ServerProxy()
{
}
