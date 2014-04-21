# overkill
An overkill Raspberry Pi project.

Currently used for timelapse. Still tweaking the heatpad to support very low temperature - Yeah it's Canada here (can go down to -30c).

Here's some [pictures](https://plus.google.com/photos/113754991837021958718/albums/5970466140155217569) and some [timelapses](https://twitter.com/RasPimelapse)

## Commands

### ok-heatpad-on
Send 3.3v on GPIO $HEATPAD_PIN.

### ok-heatpad-off
Stop sending 3.3v on GPIO $HEATPAD_PIN.

### ok-heatpad-read
Return GPIO $HEATPAD_PIN value.

### ok-heatpad-pwm_{0,25,50,75,100}
Set power-width modulation to 0% on GPIO $HEATPAD_PIN.

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
Launch another command at sunrise. Format: ok-sunrise-launcher <command> <offset in minutes>

Examples:

This cron job will start at 5am but the command will only be launch at sunrise based on current GPS coordinates.
    
    0  5	* * *	root /usr/local/bin/ok-sunrise-launcher /usr/local/bin/ok-timelapse-test -30

Requires ok-gpsd daemon.

### ok-timelapse-sunrise
Create a timelapse, upload the video to youtube and post the link on twitter.

See ok.conf.

Requires [ptt](http://mike.verdone.ca/twitter/) and [youtube-upload](https://code.google.com/p/youtube-upload/) commands.

## Daemons

### ok-rht03d
Save RHT03 sensor values to the filesystem at regular interval.

### ok-gpsd
Save GPS values tn the filesystem at regular interval.

## Munin
Plugins to monitor sensors.

## License
MIT

## Source
https://github.com/northox/overkill

## Author
Danny Fullerton - Mantor Organization
