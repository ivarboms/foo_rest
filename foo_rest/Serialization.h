#pragma once

#include "Data.h"

nlohmann::json serialize(const Playlist& playlist)
{
	nlohmann::json json;
	json["title"] = wstring_to_utf8(playlist.title);
	json["id"] = playlist.id;
	json["itemCount"] = playlist.itemCount;
	json["isPlaying"] = playlist.isPlaying;
	return json;
}

nlohmann::json serialize(const PlaylistItem& item)
{
	nlohmann::json json;
	json["artist"] = wstring_to_utf8(item.artist);
	json["title"] = wstring_to_utf8(item.title);
	json["album"] = wstring_to_utf8(item.album);
	json["trackNumber"] = wstring_to_utf8(item.trackNumber);
	json["id"] = item.id;
	json["isPlaying"] = item.isPlaying;
	return json;
}

template <typename T>
std::vector<nlohmann::json> serialize(const std::vector<T>& vec)
{
	std::vector<nlohmann::json> json(vec.size());
	std::transform(std::begin(vec), std::end(vec), std::begin(json), [](const T& t)
	{
		return serialize(t);
	});
	return json;
}