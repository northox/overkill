# Overkill ok-rth03d Daemon
# rht03 sensor file daemon
# Danny Fullerton - Mantor Organization
#
# This code was originally part of wiringPi but have been extensively modified. 
# See: https://projects.drogon.net/wiring-pi

#DEBUG	= -g -O0 -DDEBUG
DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

###############################################################################

SRC	=	ok-rht03.c
OBJ	=	$(SRC:.c=.o)
BINS	=	$(SRC:.c=)

ok-rht03:	ok-rht03.o
	@$(CC) -o $@ ok-rht03.o $(LDFLAGS) $(LDLIBS) 

clean:
	@rm -f $(OBJ) *~ core $(BINS)
