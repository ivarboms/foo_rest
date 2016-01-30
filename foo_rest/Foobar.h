#pragma once

#include <vector>

#include "Data.h"


// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str);

namespace foobar
{
	enum class PlaybackCommand
	{
		Play = 0,
		PlayPause = 1,
		Next = 2,
		Previous = 3,
		Random = 4,
		ToggleMute = 5,
	};

	std::vector<Playlist> getPlaylists();

	std::vector<PlaylistItem> getPlaylistContent(size_t playlistId);

	bool playTrack(size_t playlistId, size_t item);

	float getVolume();

	void setVolume(float db);

	bool isMuted();

	bool executePlaybackCommand(PlaybackCommand command);
}