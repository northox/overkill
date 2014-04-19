/*
 * GNU Lesser General Public License
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <wiringPi.h>
#include <maxdetect.h>

#define	RHT03_PIN 7
#define SAMPLE_SIZE 4
#define SLEEP 20000
#define SLEEP_NODATA 5000
#define OFFCHART 10

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

// move index across the array
void step (int *pi)
{
  if (*pi == SAMPLE_SIZE - 1)
    *pi = 0;
  else
    *pi += 1;
}

int main (void)
{
  // TODO use a struct and get rid of duplication
  int temp, humi, ntemp, nhumi, ttemp, thumi, atemp, ahumi;
  int index, i;
  bool first;
  int temps[SAMPLE_SIZE];
  int humis[SAMPLE_SIZE];
  int *pindex = &index;

  temp = humi = ntemp = nhumi = ttemp = thumi = atemp = ahumi = 0;
  i = index = 0;
  first = true;
  
  wiringPiSetup();
  piHiPri(55);

  for(;;)
  {
    // no new data
    if (!readRHT03 (RHT03_PIN, &ntemp, &nhumi)) {
      delay(SLEEP_NODATA);
      continue;
    }
    
    // initialize samples
    if (first == true)
    {
      for (i=0; i<SAMPLE_SIZE; i++)
      {
        temps[i] = ntemp;
        humis[i] = nhumi;
      }
      first = false;
    } 

    // Compute average
    ttemp = thumi = 0;
    for (i=0; i<SAMPLE_SIZE; i++)
    {
      ttemp += temps[i];
      thumi += humis[i];
    }
    atemp = ttemp / SAMPLE_SIZE;
    ahumi = thumi / SAMPLE_SIZE;

    // some new data but not equal to lastest
    if (temp != ntemp)
    {
      // use average if offchart
      if (ntemp > temp + OFFCHART || ntemp < temp - OFFCHART)
        ntemp = atemp;
      
      temps[index] = temp = ntemp;
      write(tfile, &temp);
    }
    if (humi != nhumi)
    {
      if (nhumi > humi + OFFCHART || nhumi < humi - OFFCHART) 
        nhumi = ahumi;
      
      humis[index] = humi = nhumi;
      write(hfile , &humi);
    }
    
    step(pindex);
    delay(SLEEP);
  }

  return 0;
}
