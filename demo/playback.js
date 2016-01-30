var url = 'http://localhost:9998/';

//Values from foobar::PlaybackCommand enum.
var commands = {};
commands['Play'] = 0;
commands['PlayPause'] = 1;
commands['Next'] = 2;
commands['Previous'] = 3;
commands['Random'] = 4;
commands['ToggleMute'] = 5;

//Foobar's volume slider uses dB as measurement. These two functions help with making a similar slider.
//Note: These functions do not produce results that match foobar's results perfectly, but they come close.
function toDb(percentage) {
	if (percentage == 0) {
		return -100.0;
	}
	return Math.round(1000000000 * 20 * (Math.log(percentage * 0.01) / Math.log(10))) / 1000000000;
}

function toPercentage(db) {
	return Math.round(1000000000 * Math.pow(10, db / 20)) / 10000000;
}

function updateSliderText() {
	var sliderValue = $('#volume-slider').val();
	var db = toDb(sliderValue);
	$('#volume-text').html(db.toFixed(2) + ' dB (' + sliderValue + '%)')
}

$(function() {
	//Clicking button send command specified by the button's text.
	$('button').click(function() {
		$.post({
			url: url + 'playback/' + commands[$(this).html()],
		});
	});

	//Update foobar's volume with slider. This sends a large amount of requests while sliding.
	var slider = $('#volume-slider');
	slider.on('input change', function() {
		$.post({
			url: url + 'volume/' + toDb(slider.val()),
		});
		updateSliderText();
	});

	//Set initial value of volume slider.
	$.getJSON({
		url: url + 'volume',
		success: function(data) {
			$('#volume').html("volume: " + JSON.stringify(data));
			slider.val(toPercentage(data.db));
			updateSliderText();
		},
	});
});
