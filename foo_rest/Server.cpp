#include "stdafx.h"
#include "Server.h"

#include "Foobar.h"
#include "Serialization.h"


using nlohmann::json;

/*
// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
*/

//Super important HTTP header which allows javascript running in a browser to communicate with the server.
const std::string accessControl("Access-Control-Allow-Origin: *");

void writeContent(HttpServer::Response& response, const std::wstring& content)
{
	const std::string utf8 = wstring_to_utf8(content);
	response << "HTTP/1.1 200 OK\r\n" + accessControl + "\r\nContent-Length: " << content.length() << "\r\n\r\n" << utf8;
}

void writeError(HttpServer::Response& response, const std::wstring& content)
{
	const std::string utf8 = wstring_to_utf8(content);
	response << "HTTP/1.1 500 Error!!!\r\n" << accessControl << "\r\nContent-Length: " << content.length() << "\r\n\r\n" << utf8;
}

void writeJson(HttpServer::Response& response, const json& json)
{
	std::string content = json.dump(4);
	response << "HTTP/1.1 200 OK\r\n" + accessControl + "\r\nContent-Length: " << content.length() << "\r\nContent-Type: \"application/json\";\r\n\r\n" << content;
}

void getPlaylists(HttpServer::Response & response, std::shared_ptr<HttpServer::Request> request)
{
	const std::vector<Playlist> playlists = foobar::getPlaylists();

	json root(serialize(playlists));
	writeJson(response, root);
}

void getPlaylistContent(HttpServer::Response & response, std::shared_ptr<HttpServer::Request> request)
{
	std::string number = request->path_match[1];
	unsigned long playlistId = 1;
	try
	{
		playlistId = std::stoul(number);
	}
	catch (const std::exception& e)
	{
		writeError(response, utf8_to_wstring(e.what()));
	}
	std::vector<PlaylistItem> content = foobar::getPlaylistContent(playlistId);

	json root(serialize(content));
	writeJson(response, root);
}

void getVolume(HttpServer::Response & response, std::shared_ptr<HttpServer::Request> request)
{
	float db = foobar::getVolume();
	json j;
	j["db"] = db;
	writeJson(response, j);
}

void setVolume(HttpServer::Response & response, std::shared_ptr<HttpServer::Request> request)
{
	std::string number = request->path_match[1];
	float db = -100.0f;
	try
	{
		db = std::stof(number);
	}
	catch (const std::exception& e)
	{
		writeError(response, utf8_to_wstring(e.what()));
	}
	foobar::setVolume(db);
	writeContent(response, L"");
}


void isMuted(HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request)
{
	bool isMuted = foobar::isMuted();
	json j{ {"isMuted", isMuted} };
	writeJson(response, j);
}

void playTrack(HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request)
{
	std::string playlistIdString = request->path_match[1];
	std::string trackString = request->path_match[2];
	size_t playlistId = 0;
	size_t track = 0;
	try
	{
		playlistId = std::stoul(playlistIdString);
		track = std::stoul(trackString);
	}
	catch (const std::exception& e)
	{
		writeError(response, utf8_to_wstring(e.what()));
	}
	bool result = foobar::playTrack(playlistId, track);
	json j{ {"result", result} };
	writeJson(response, j);
}

void playbackCommand(HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request)
{
	std::string commandString = request->path_match[1];
	foobar::PlaybackCommand command;
	try
	{
		command = (foobar::PlaybackCommand)std::stoul(commandString);
	}
	catch (const std::exception& e)
	{
		writeError(response, utf8_to_wstring(e.what()));
	}
	bool result = foobar::executePlaybackCommand(command);
	json j{ {"result", result} };
	writeJson(response, j);
}

Server::Server()
	: server(9998)
{
	server.config.address = "127.0.0.1";
	server.resource["^/playlists$"]["GET"] = &getPlaylists;
	server.resource["^/playlist/([0-9]+)$"]["GET"] = &getPlaylistContent;

	server.resource["^/play/([0-9]+)/([0-9]+)$"]["POST"] = &playTrack;
	server.resource["^/playback/([0-9]+)$"]["POST"] = &playbackCommand;

	server.resource["^/volume$"]["GET"] = &getVolume;
	server.resource["^/volume/(-?[0-9.]+)$"]["POST"] = &setVolume;
	server.resource["^/volume/ismuted$"]["GET"] = &isMuted;

	server.default_resource["GET"] = [](HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request) {
		printf("Request to '%s' ignored.\n", request->path.c_str());
		writeError(response, L"This error should really be a 404");
	};

	serverThread = std::thread([this]() {
		server.start();
	});
}

Server::~Server()
{
	if (serverThread.joinable()) {
		serverThread.join();
	}
}
