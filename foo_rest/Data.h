#pragma once

#include <string>
#include <codecvt>

#include "json.hpp"


static std::string wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

struct Playlist
{
	std::wstring title;
	size_t id;
	size_t itemCount;
	bool isPlaying;
};

struct PlaylistItem
{
	std::wstring artist;
	std::wstring title;
	std::wstring album;
	std::wstring trackNumber;
	size_t id;
	bool isPlaying;
};