#include "stdafx.h"
#include "Foobar.h"
#include "MainThreadCallback.h"

#include "foobar2000/SDK/foobar2000.h"


std::vector<Playlist> getPlaylistsImpl()
{
	static_api_ptr_t<playlist_manager_v4> api;

	const t_size count = api->get_playlist_count();
	const t_size playing = api->get_playing_playlist();

	std::vector<Playlist> playlists;
	playlists.reserve(count);

	pfc::string8 name;
	for (size_t i = 0; i < count; ++i)
	{
		name.reset();
		if (api->playlist_get_name(i, name))
		{
			Playlist playlist;
			playlist.title = utf8_to_wstring(name.c_str());
			playlist.id = i;
			playlist.itemCount = api->playlist_get_item_count(i);
			playlist.isPlaying = playing == i;
			playlists.push_back(playlist);
		}
	}

	return playlists;
}

std::vector<PlaylistItem> getPlaylistContentImpl(size_t playlistId)
{
	static_api_ptr_t<playlist_manager_v4> api;

	metadb_handle_list handles;
	api->playlist_get_all_items(playlistId, handles);

	std::vector<PlaylistItem> content;
	content.reserve(handles.get_size());

	titleformat_object::ptr script;
	pfc::string8 pattern("%artist%|%title%|%album%");
	static_api_ptr_t<titleformat_compiler>()->compile_safe_ex(script, pattern);
	static_api_ptr_t<playback_control> playbackControl;

	size_t playingPlaylist;
	size_t playingItem;
	const bool isPlaying = api->get_playing_item_location(&playingPlaylist, &playingItem);
	bool isPlayingCurrentPlaylist = playlistId == playingPlaylist && isPlaying;

	handles.for_each([&script, &playbackControl, &content, playingItem, isPlayingCurrentPlaylist](const metadb_handle_ptr handle)
	{
		pfc::string_formatter formattedTitle;
		playbackControl->playback_format_title_ex(handle, nullptr, formattedTitle, script, nullptr, playback_control::display_level_all);

		PlaylistItem item;
		item.title = utf8_to_wstring(formattedTitle.c_str());
		item.id = content.size();
		item.isPlaying = isPlayingCurrentPlaylist && item.id == playingItem;

		content.push_back(item);
	});

	return content;
}

bool executePlaybackCommandImpl(foobar::PlaybackCommand command)
{
	using namespace foobar;

	static_api_ptr_t<playback_control> playbackControl;
	switch (command)
	{
	case PlaybackCommand::PlayPause:
		if (playbackControl->is_playing() && !playbackControl->is_paused())
		{
			playbackControl->pause(true);
		}
		else
		{
			playbackControl->play_or_unpause();
		}
		return true;

	case PlaybackCommand::Next:
		playbackControl->next();
		return true;

	case PlaybackCommand::Previous:
		playbackControl->previous();
		return true;

	case PlaybackCommand::Random:
		playbackControl->start(playback_control::track_command_rand);
		return true;

	case PlaybackCommand::ToggleMute:
		playbackControl->volume_mute_toggle();
		return true;
	}
	
	return false;
}

void runOnMainThread(const std::function<void()>& func)
{
	auto mainThreadCallback = runOnMainThread_(func);

	while (!mainThreadCallback->isCompleted)
	{
		Sleep(1);
	}
}

template <typename T>
T runOnMainThread(const std::function<T()>& func)
{
	auto mainThreadCallback = runOnMainThread_(func);

	while (!mainThreadCallback->isCompleted)
	{
		Sleep(1);
	}

	return mainThreadCallback->result;
}

namespace foobar
{
	std::vector<Playlist> getPlaylists()
	{
		return runOnMainThread<std::vector<Playlist>>([]
		{
			return getPlaylistsImpl();
		});
	}

	std::vector<PlaylistItem> getPlaylistContent(size_t playlistId)
	{
		return runOnMainThread<std::vector<PlaylistItem>>([playlistId]
		{
			return getPlaylistContentImpl(playlistId);
		});
	}

	bool playTrack(size_t playlistId, size_t item)
	{
		return runOnMainThread<bool>([playlistId, item]
		{
			static_api_ptr_t<playlist_manager_v4> api;
			api->set_active_playlist(playlistId);
			return api->playlist_execute_default_action(playlistId, item);
		});
	}

	float getVolume()
	{
		return runOnMainThread<float>([]
		{
			static_api_ptr_t<playback_control> playbackControl;
			return playbackControl->get_volume();
		});
	}

	void setVolume(float db)
	{
		runOnMainThread([db]
		{
			static_api_ptr_t<playback_control> playbackControl;
			playbackControl->set_volume(db);
		});
	}

	bool isMuted()
	{
		return runOnMainThread<bool>([]
		{
			static_api_ptr_t<playback_control> playbackControl;
			return playbackControl->is_muted();
		});
	}

	bool executePlaybackCommand(PlaybackCommand command)
	{
		return runOnMainThread<bool>([command]
		{
			return executePlaybackCommandImpl(command);
		});
	}
}

std::wstring utf8_to_wstring(const std::string & str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}