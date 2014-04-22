# overkill
An overkill Raspberry Pi project.

Currently used for timelapse. Still tweaking the heatpad to support very low temperature - Yeah it's Canada here (can go down to -30c).

Here's some [pictures](https://plus.google.com/photos/113754991837021958718/albums/5970466140155217569) and some [timelapses](https://twitter.com/RasPimelapse).

## Commands

### ok-heatpad-on
Set GPIO `$HEATPAD_PIN` on - send 3.3v.

### ok-heatpad-off
Set GPIO `$HEATPAD_PIN` off - stop 3.3v.

### ok-heatpad-read
Return GPIO `$HEATPAD_PIN` value.

### ok-heatpad-pwm_{0,25,50,75,100}
Set power-width modulation to 0% on GPIO `$HEATPAD_PIN`.

### ok-sensor-cpu-temp
Return CPU temp in celcius.

### ok-sensor-gpu-temp
Return GPU temp in celcius.

### ok-sensor-rht03-humi
Return RHT03 humidity sensor in %.

Require ok-rht03 daemon.

### ok-sensor-rht03-temp
Return RHT03 temperature sensor in celcius.

Requires ok-rht03 daemon.

### ok-sunrise-launcher
Launch another command at sunrise. Format: ok-sunrise-launcher command-to-launch offset-in-minutes

Examples:

This cron job will start at 5am but the command will only be launch 30 minutes before sunrise based on current GPS coordinates.
    
    0  5	* * *	root /usr/local/bin/ok-sunrise-launcher /usr/local/bin/ok-timelapse-test -30

### ok-timelapse-sunrise
Create a timelapse, upload the video to youtube and post the link on twitter.

    SLEEP=5000
    TIME=6000000

With this config, a the pictures will be taken every 5 seconds for 100 minutes. The pictures are then used to create a video at 24 frame per seconds.

See ok.conf.sample for all options.

Requires mencoder, raspistill, [ptt](http://mike.verdone.ca/twitter/) and [youtube-upload](https://code.google.com/p/youtube-upload/) commands.

## ok-rht03d daemon
Save RHT03 sensor values to the filesystem at regular interval.

Simply run `make rht03` within ok-rht03d to compile.

## Munin stats
Plugins to monitor sensors.

## License
MIT

## Source
https://github.com/northox/overkill

## Author
Danny Fullerton - Mantor Organization
