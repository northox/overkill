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
#define SAMPLE_SIZE 4
#define SLEEP 20000
#define SLEEP_NODATA 5000
#define OFFCHART 10
 
char * temp_path = "/var/cache/overkill/rht03/t";
char * humi_path = "/var/cache/overkill/rht03/h";
 
int si; // sample index
 
typedef struct sensor {
  int old;
  int new;
  int sample[SAMPLE_SIZE];
  int avg;
  char * path;
} Sensor;

typedef struct rht03 {
  Sensor temp;
  Sensor humi;
} Rht03;

// TODO reuse file descriptors 
void writebuf(char *file, int *buf)
{
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    fprintf(stderr, "Can't open output file %s!\n", file);
    exit(1);
  }
 
  fprintf(f, "%1.1f\n", *buf / 10.0);
  fclose(f);
}

void step_si() 
{
  if (si == SAMPLE_SIZE - 1)
    si = 0;
  else
    si += 1;
}
 
void set(Sensor * s)
{
  if (s->new > s->old + OFFCHART || s->new < s->old - OFFCHART)
    s->new = s->avg;
     
  s->sample[si] = s->old = s->new;
  writebuf(s->path , &s->new);
}

void set_avg(Sensor * s)
{
  int i;
  int total;

  total = 0;
  for (i=0; i < SAMPLE_SIZE; i++)
    total += s->sample[i];
  
  s->avg = total / SAMPLE_SIZE;
}
 
void init(Rht03 * r)
{
  r->temp.path = &temp_path;
  r->humi.path = &humi_path;
  
  for(;;) {
    if (readRHT03 (GPIO_PIN, &r->temp.new, &r->humi.new))
      break;
    delay(SLEEP_NODATA);
  }

  r->temp.avg = r->temp.old = r->temp.new;
  r->humi.avg = r->humi.old = r->humi.new;
  
  int i;
  for (i=0; i<SAMPLE_SIZE; i++) {
    r->temp.sample[i] = r->temp.new;
    r->humi.sample[i] = r->humi.new;
  }
}
 
int main (void)
{
  pid_t process_id = 0;
  pid_t sid = 0;
  process_id = fork();
  if (process_id < 0) {
    fprintf(stderr, "Error: fork()\n");
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
 
  Rht03 r;
  
  wiringPiSetup();
  piHiPri(55);
 
  si = 0; 
  init(&r);

  for(;;) 
  {
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
    delay(SLEEP);
  }
  
  return(0);
}
