# overkill
An overkill Raspberry Pi project.

## commands

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
Launch another command at sunrise. Format: ok-sunrise-launcher <command> <offset in minutes> [ok-gpsd]

Ex: ok-sunrise-launcher ok-timeslapse-sunrise -30 will start a timelapse 30 minutes before the sunrise.

### ok-timelapse-sunrise
Create a timelapse, upload the video to youtube and post the link on twitter.

See ok.conf.

Requires [ptt](http://mike.verdone.ca/twitter/) and [youtube-upload](https://code.google.com/p/youtube-upload/) commands.

## Daemons

### ok-rht03
Save RHT03 sensor values to the filesystem at regular interval.

### ok-gpsd
Save GPS values tn the filesystem at regular interval.

## License
MIT

## Source
https://github.com/northox/overkill

## Author
Danny Fullerton - Mantor Organization
