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
#include <string.h>

#include "display.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "knob.h"

unsigned char *parlcd_mem_base;

struct Unit {
	int number;
	char name[16];
	uint32_t ceiling;
	uint32_t wall;
	uint16_t graphics[16][16];
};

int main(int argc, char *argv[])
{
	
  //int i, j, offX, offY;
  //unsigned c;
  int NoUnits = 4;
  //int selection = 1;
  uint32_t rgb_knobs_value;

  unsigned char *mem_base;

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);

  grafclear(0xf);
  
  printf("Hello world\n");
  
  	int it = 0;
	char unit[16];
	struct Unit list[NoUnits];
	
	while(!redPushed((int) rgb_knobs_value) && !bluePushed((int) rgb_knobs_value) && !greenPushed((int) rgb_knobs_value)){
		rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		
		for(it = 0; it < NoUnits; it++){
			sprintf(unit, "Jednotka %d", it+1);
			list[it].number = it+1;
			strcpy(list[it].name, unit);
			list[it].ceiling = rgb_knobs_value;
			list[it].wall = rgb_knobs_value;
			writeText(list[it].name, 0, it*16);
		}
		
		writeText("Pridat jednotku", 0, it*16);
		grafShow();
		sleep(1);
	}
	
	printf("Goodbye world\n");

  return 0;
}
