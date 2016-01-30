#pragma once

#include <memory>

class Server;

class ServerProxy
{
public:
	ServerProxy();

	~ServerProxy();

	std::unique_ptr<Server> server;
};

