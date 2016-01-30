#pragma once

#include <thread>

#include "server_http.hpp"

#include "Foobar.h"


typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

class Server
{
public:
	Server();

	~Server();

private:
	HttpServer server;
	std::thread serverThread;
};

