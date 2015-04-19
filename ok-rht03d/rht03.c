/**
 * @title Overkill ok-rth03d Daemon
 * @descrition rht03 sensor file daemon
 * @author Danny Fullerton - Mantor Organization
 * @license MIT
 * @acknowledgements Orgininaly part of WiringPi Project from Gordon Henderson
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <wiringPi.h>
#include <maxdetect.h>

#define	GPIO_PIN 7
#define SAMPLE_SIZE 8
#define SLEEP 5 * 60 * 1000
#define SLEEP_NODATA 7 * 60 * 1000
#define OFFCHART 20
 
char * temp_path = "/var/cache/overkill/rht03/t";
char * humi_path = "/var/cache/overkill/rht03/h";
 
int si; // sample index
 
typedef struct sensor {
  int old;
  int new;
  int sample[SAMPLE_SIZE];
  int avg;
  FILE * fd;
} Sensor;

typedef struct rht03 {
  Sensor temp;
  Sensor humi;
} Rht03;

void step_si() 
{
  if (si == SAMPLE_SIZE - 1)
    si = 0;
  else
    si += 1;
#ifdef DEBUG 
    printf("%s: %i\n", __func__, si);
#endif
}
 
void set(Sensor * s)
{
  if (s->new > s->old + OFFCHART || s->new < s->old - OFFCHART) {
#ifdef DEBUG 
    printf("Sensor went off chart (%i) using avg (%i)\n", s->new, s->avg);
#endif
    s->new = s->avg;
  }
     
  s->sample[si] = s->old = s->new;
  if (s->fd == NULL) {
    fprintf(stderr, "fd is null\n");
    exit(1);
  }

  fprintf(s->fd, "%1.1f\n", s->new / 10.0);
  fflush(s->fd);
#ifdef DEBUG 
  printf("%s: %i\n", __func__, s->new);
#endif
}

void set_avg(Sensor * s)
{
  int i;
  int total;

  total = 0;
  for (i=0; i < SAMPLE_SIZE; i++)
    total += s->sample[i];
  
  s->avg = total / SAMPLE_SIZE;
#ifdef DEBUG 
  printf("%s: total:%i, avg:%i\n", __func__, total, s->avg);
#endif
}
 
void init(Rht03 * r)
{
  r->temp.fd = fopen(temp_path, "w");
  if (r->temp.fd == NULL) {
    fprintf(stderr, "Can't open temp file %s!\n", temp_path);
    exit(1);
  }

  r->humi.fd = fopen(humi_path, "w");
  if (r->humi.fd == NULL) {
    fprintf(stderr, "Can't open humi file %s!\n", humi_path);
    exit(1);
  }

  for(;;) {
    if (readRHT03 (GPIO_PIN, &r->temp.new, &r->humi.new))
      break;
    delay(SLEEP_NODATA);
  }

  r->temp.avg = r->temp.old = r->temp.new;
  r->humi.avg = r->humi.old = r->humi.new;

  fprintf(r->temp.fd, "%1.1f\n", r->temp.new / 10.0);
  fflush(r->temp.fd);
  fprintf(r->humi.fd, "%1.1f\n", r->humi.new / 10.0);
  fflush(r->humi.fd);

#ifdef DEBUG 
  printf("%s: temp:%i, humi:%i\n", __func__, r->temp.avg, r->humi.avg);
#endif
  
  int i;
  for (i=0; i<SAMPLE_SIZE; i++) {
    r->temp.sample[i] = r->temp.new;
    r->humi.sample[i] = r->humi.new;
  }
}
 
int main (void)
{
#ifndef DEBUG
  pid_t process_id = 0;
  pid_t sid = 0;
  process_id = fork();
  if (process_id < 0) {
    fprintf(stderr, "fork()\n");
    exit(1);
  } 
  if (process_id > 0)
    exit(0);
  
  umask(0);
  sid = setsid();
  if (sid < 0)
    exit(1);
  chdir("/");
  close(STDIN_FILENO);
  close(STDOUT_FILENO); 
  close(STDERR_FILENO);  
#endif
 
  Rht03 r;
  wiringPiSetup();
  piHiPri(55);
 
  si = 0; 
  init(&r);
  
  for(;;) 
  {
#ifdef DEBUG 
    printf("loop\n");
#endif
    if (!readRHT03 (GPIO_PIN, &r.temp.new, &r.humi.new)) {
      delay(SLEEP_NODATA);
      continue;
    }

    set_avg(&r.temp);
    set_avg(&r.humi);

    if (r.temp.old != r.temp.new)
      set(&r.temp);
    
    if (r.humi.old != r.humi.new)
      set(&r.humi);
    
    step_si();
    sync();
    delay(SLEEP);
  }
  
  return(0);
}
