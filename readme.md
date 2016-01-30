foo_rest
==================

REST interface for [foobar2000](https://www.foobar2000.org/).
This interace does not expose anywhere close to all of foobar2000's functionality.

Uses [eidheim's Simple-Web-Server](https://github.com/eidheim/Simple-Web-Server) (included), [nlohmann's json](https://github.com/nlohmann/json) (included), [boost](http://www.boost.org/) and the [foobar2000 SDK](https://www.foobar2000.org/SDK).

Tested with Visual Studio 2015 and foobar2000 1.3.9.

## Demos
<table>
	<thead>
		<tr>
			<th>Title</th>
			<th>Description</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td>simple.html</td>
			<td>Minimal example showing retrieval of playlists and playlist content.</td>
		</tr>
		<tr>
			<td>playlists.html</td>
			<td>Example showing more advanced playlist usage. Shows retrieval and displaying of playlists and playlist content, and shows how to play songs from a playlist.</td>
		</tr>
		<tr>
			<td>playback.html</td>
			<td>Shows how to control some playback features, and shows how to control volume, including a volume slider displaying volume levels the same way as foobar2000.</td>
		</tr>
	</tbody>
</table>

## Resources
For example usage, see demos in the /demo folder.

<table>
	<thead>
		<tr>
			<th>Resource</th>
			<th>Description</th>
			<th>Example</th>
			<th>Returns</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td>/playlists GET</td>
			<td>Returns an array of all the playlists.</td>
			<td>/playlists</td>
			<td>[ { title, id, itemCount, isPlaying } ]</td>
		</tr>
		<tr>
			<td>/playlist/{id} GET</td>
			<td>Returns the items in a specific playlist.</td>
			<td>/playlist/1</td>
			<td>[ { title, id, isPlaying } ]</td>
		</tr>
		<tr>
			<td>/play/{playlistId}/{trackId} POST</td>
			<td>Plays the specified item in the specific playlist.</td>
			<td>/play/1/13</td>
			<td>Nothing.</td>
		</tr>
		<tr>
			<td>/playback/{commandId} POST</td>
			<td>Executes the specified playback command. The list of available commands can be found in foobar::PlaybackCommand</td>
			<td>/playback/1</td>
			<td>Nothing.</td>
		</tr>
		<tr>
			<td>/volume GET</td>
			<td>Returns the current volume, in dB.</td>
			<td>/volume</td>
			<td>{ db }</td>
		</tr>
		<tr>
			<td>/volume/{volume} POST</td>
			<td>Sets the volume. Must be in dB.</td>
			<td>/volume/-5.31</td>
			<td>Nothing.</td>
		</tr>
		<tr>
			<td>/volume/ismuted GET</td>
			<td>Returns whether the volume is muted or not.</td>
			<td>/volume/ismuted</td>
			<td>{ isMuted }</td>
		</tr>
	</tbody>
</table>
