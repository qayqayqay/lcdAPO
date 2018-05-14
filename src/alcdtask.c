/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "display.h"
#include <string.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

unsigned char *parlcd_mem_base;

int main(int argc, char *argv[])
{
	
  int i, j, offX, offY;
  unsigned c;
  int NoUnits = 4;
  int selection;

  unsigned char *mem_base;

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = 0;
      parlcd_write_data(parlcd_mem_base, c);
    }
  }

  while(0)
  {
  uint32_t rgb_knobs_value;

  rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

  offX  = 160 +(rgb_knobs_value & 0x1f);
  offY = 240;
  printf("offX %d \n",offX);
  
  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      if(((i-offX)*(i-offX))+((j-offY)*(j-offY)) < 4000){
	  c = 0xffff;
	  } else {
	  c = 0x0;
	  }
      
      /*c = ((i & 0x1f) << 11) | (j & 0x1f);
      if (i < 10)
        c |= 0x3f << 5;
      if (j < 10)
        c |= 0x3f << 5;*/
      parlcd_write_data(parlcd_mem_base, c);
    }
  }

  while (0) {
     struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 200 * 1000 * 1000};

     *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0001;
     *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0002;
     *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0004;
     *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0008;
     *(volatile uint32_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0010;
     *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0020;

     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }
  //printf("reset je %d \n",rgb_knobs_value <<15);
  if((rgb_knobs_value & 0x1f) > 32){
  break;
  }
  }//konci WHILE
  grafclear(0xf);
  //writeLetter(53, 120, 120); 
  
  printf("Hello world\n");
  //writeText("extremne dlouhy text, ktery urcite musi vypsat na vice nez jeden radek, aspon doufam teda", 120,120);
  
  //added writing units
  int it;
  char unit[11];
  for(it = 0; it < NoUnits; it++){
	  sprintf(unit, "Jednotka %d", it+1);
	  writeText(unit, 0, it*16);
  }
  writeText("Pridat jednotku", 0, it*16);
  grafShow();
  sleep(4);

  printf("Goodbye world\n");

  return 0;
}
