var url = 'http://localhost:9998/';

function insertPlaylists(data) {
	var tbody = $('#playlists tbody');
	for (var i = 0; i < data.length; ++i) {
		var title = '<td>' + data[i].title + '</td>';
		var isPlaying = '<td>' + (data[i].isPlaying ? '✅' : '') + '</td>';
		var itemCount  = '<td>' + data[i].itemCount + '</td>';
		var row = '<tr data-id="' + data[i].id + '">' + title + isPlaying + itemCount + '</tr>';
		tbody.append(row);
	}

	//Click table row to load playlist content.
	tbody.children('tr').click(function() {
		var $this = $(this);
		var id = $this.data('id');
		$.getJSON({
			url: url + 'playlist/' + id,
			success: function(data) {
				insertPlaylistContent($this, data, id);
			},
		});
	});
}

function insertPlaylistContent(tr, data, playlistId) {
	var pre = tr.find('pre');
	if (pre.length > 0) {
		//Already loaded.
		pre.slideToggle();
	} else {
		var tracks = [];
		for (var i = 0; i < data.length; ++i) {
			var item = data[i];
			var artist = '<span class="artist">' + item.artist + '</span>';
			var title = '<span class="title">' + item.title + '</span>';
			var album = '<span class="album">' + item.album + '</span>';
			var trackNumber = '<span class="trackNumber">(' + item.trackNumber + ')</span>';

			var active = item.isPlaying ? 'playing' : '';
			var span = '<span class="track ' + active + '" data-playlist-id="' + playlistId + '" data-track-id="' + item.id + '">';
			span += artist + ' - ' + title + ' - ' + album + ' ' + trackNumber + '</span>';

			tracks.push(span);
		}
		pre = $('<pre>' + tracks.join('\n') + '</pre>');
		pre.appendTo(tr);
		pre.hide();
		pre.slideDown();

		pre.find('.track').click(function(e) {
			playTrack($(this));
			//Prevent track click from collapsing playlist. 
			e.stopPropagation();

			//Reload playlist.
			pre.remove();
			tr.click();
		});
	}
}

function playTrack(span) {
	var playlistId = span.data('playlist-id');
	var trackId = span.data('track-id');

	$.post({
		url: url + 'play/' + playlistId + '/' + trackId,
	});
}

$(function() {
	$.getJSON({
		url: url + 'playlists',
		success: insertPlaylists,
		error: function(xhr, ajaxOptions, thrownError) {
			$('body').append('<div>Failed to load playlists.</div>');
			$('body').append('<pre>' + JSON.stringify(xhr, null, 2) + '</pre>');
			$('body').append('<pre>' + JSON.stringify(ajaxOptions, null, 2) + '</pre>');
			$('body').append('<pre>' + JSON.stringify(thrownError, null, 2) + '</pre>');
		}
	});
});
