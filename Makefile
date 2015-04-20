# Overkill ok-rth03d Daemon
# rht03 sensor file daemon
# Danny Fullerton - Mantor Organization
#
# This code was originally part of wiringPi but have been extensively modified. 
# See: https://projects.drogon.net/wiring-pi

PREFIX	= /usr/local

#DEBUG	= -g -O0 -DDEBUG
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

###############################################################################

TMP	=	/var/cache/overkill

#TODO
#test mencoder
#test twitter

SRC	=	ok-rht03d.c
OBJ	=	$(SRC:.c=.o)
BINS	=	$(SRC:.c=) 
SCRIPTS = 	ok-heatpad-read ok-heatpad-pwm_0 ok-timelapse-sunrise \
		ok-heatpad-pwm_100 ok-sensor-cpu-temp ok-timelapse-test \
		ok-heatpad-pwm_25 ok-sensor-gpu-temp ok-sunrise-launcher \
		ok-heatpad-pwm_50 ok-sensor-rht03-humi ok-heatpad-off \
		ok-heatpad-on ok-heatpad-pwm_75 ok-sensor-rht03-temp

all: ok-rht03d

ok-rht03d: ok-rht03d.o
	$(CC) -o $@ ok-rht03d.o $(LDFLAGS) $(LDLIBS) 

install: install-bins install-scripts setup

setup:
	mkdir -p $(TMP)/timelapse
	mkdir -p $(TMP)/rht03
	@echo
	@echo "Don't forget to setup and copy ok.conf.sample to /etc/ok.conf"

install-bins: ok-rht03d
	$(foreach bin, $(BINS), install -m 0755 $(bin) $(PREFIX)/bin;)

install-scripts:
	$(foreach script, $(SCRIPTS), install -m 0755 $(script) $(PREFIX)/bin;)

clean:
	rm -f $(OBJ) *~ core $(BINS)
