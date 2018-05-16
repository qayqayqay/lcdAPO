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
#include "unit.h"

unsigned char *parlcd_mem_base;

uint16_t BLACK = 0xffff;			//mozna je to bila, kdovi... kazdopadne tady bude cerna

/*extern struct {
	int number;
	char name[16];
	uint32_t ceiling;
	uint32_t wall;
	uint16_t graphics[16][16];
} Unit;*/

void fillBasicUnit(Unit u, uint32_t cl, uint32_t wall){			//new void; uint32_t jako parametr pro ikonu jednotky, ktera ma uint16_t? mozna bude nutnost upravy
	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(i < 4){
				if(i == j || i + j == 15){
					u.graphics[i][j] = BLACK;
				} else if(i < j && i + j < 15){
					u.graphics[i][j] = cl;
				} else {
					u.graphics[i][j] = wall;
				}
			} else {
				if(i == 4 && j > 3 && j < 12){
					u.graphics[i][j] = BLACK;
				} else if(j == 4 || j == 11){
					u.graphics[i][j] = BLACK;
				} else if(i == 14 && j > 1 && j < 14){
					u.graphics[i][j] = BLACK;
				} else if(i == 15){
					u.graphics[i][j] = BLACK;
				} else {
					u.graphics[i][j] = wall;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	
  //int i, j, offX, offY;
  //unsigned c;
  int NoUnits = 4;
  selection = 1;				//pdkaz na vybranou jednotku
  sector = 1;					//odkaz na sektor, ve kterem se uzivatel pohybuje
  selected = -1;
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
	Unit list[20]; // =(* Unit) malloc(sizeof(Unit)*NoUnits);		//pozn. vyresit alokaci a realokaci pameti pro dynamiku jednotek
	
	while(!redPushed((int) rgb_knobs_value) && !bluePushed((int) rgb_knobs_value) && !greenPushed((int) rgb_knobs_value)){
	
	
		rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	
		
		
		for(it = 0; it < NoUnits; it++){
			sprintf(unit, "Jednotka %d", it+1);
			list[it].number = it+1;
			strcpy(list[it].name, unit);
			list[it].ceiling = rgb_knobs_value;
			list[it].wall = rgb_knobs_value;
			fillBasicUnit(list[it], rgb_knobs_value, rgb_knobs_value);			//new; fills all basic units
			writeText(list[it].name, 0, it*16);
		}
		
		writeText("Pridat jednotku", 0, it*16);
		
		writeText("Nastaveni", 160, 0);
		writeText("Smazat", 160, 16);
		writeText("Zpet",160, 32);
		
		writeText("Strop", 320, 0);
		writeText("Stena", 320,16);
		writeText("Vypnout",320, 32);
		writeText("Zapnout",320,48);
		writeText("Zpet",320,64);
		
		
		if(redPushed((int)rgb_knobs_value)){												//new; posun ve vyberu o pozici vys (predchozi jednotka)
			if(selection == 1){
				selection = NoUnits + 2;
			}
			selection--;
		} else {
		
		if(greenPushed((int)rgb_knobs_value)){											//new; zvoleni vyberu; na tlacitko "zpet" se musi vratit o sekci zpatky - zapracovat int sector
			switch(sector){
				case 1:{
					if(selection != NoUnits+1){	
						sector++;
						selected = selection;
						selection = 1;
					} else {
						NoUnits++;
					}
					break;
				}
				case 2:{
					switch(selection){
						case 1:{
							sector++;
							break;
						}
						case 2:{
							//delete
							break;
						}
						case 3:{
							sector--;
							selection = 1;
							selected = -1;
							break;
						}
					}
					break;
				}
				case 3:{
					switch(selection){
						case 1:{
							//ceiling
							break;
						}
						case 2:{
							//wall
							break;
						}
						case 3:{
							//vypnout
							break;
						}
						case 4:{
							//zapnout
							break;
						}
						case 5:{
							sector--;
							selection = 1;
							break;
						}
					}
					break;
				}
			}
			
			
		} else {
		
	
		
		if(bluePushed((int)rgb_knobs_value)){												//new; posun ve vyberu o pozici niz (nasledujici jednotka)
			selection++;
			if(selection == NoUnits + 2){
				selection = 1;
			}
		}
	}
	}	
	
	if(selected < 0){
		getIcon(list[selection - 1]);
	} else {
		getIcon(list[selected - 1]);
	}
		
		grafShow();
		sleep(1);
	}
	
	printf("Goodbye world\n");

  return 0;
}
