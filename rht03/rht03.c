/*
 * rht03.c:
 *	Driver for the MaxDetect series sensors
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <maxdetect.h>

#define	RHT03_PIN 7

const char * tfile = "/var/cache/overkill/rht03/t";
const char * hfile = "/var/cache/overkill/rht03/h";

void write(const char *file, int *buf)
{
  FILE *f = fopen(file, "w");
  if (f == NULL)
  {
    fprintf(stderr, "Can't open output file %s!\n", file);
    exit(1);
  }

  fprintf(f, "%1.1f\n", *buf / 10.0);
  fclose(f);
}

int main (void)
{
  int temp, rh;
  int newTemp, newRh;

  temp = rh = newTemp = newRh = 0;

  wiringPiSetup();
  piHiPri(55);

  for(;;)
  {
    delay(5000);

    // no new data
    if (!readRHT03 (RHT03_PIN, &newTemp, &newRh))
      continue;

    // some new data but equal to the old one
    if (temp != newTemp)
    {
      temp = newTemp;
      write(tfile, &temp);
    }
    if (rh != newRh)
    {
      rh   = newRh;
      write(hfile , &rh);
    }
  }

  return 0;
}
