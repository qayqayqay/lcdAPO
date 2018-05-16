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
#define  SECTOR_SIZE_2  3
#define  SECTOR_SIZE_3  5
#define  RADIUS 20
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

uint16_t BLACK = 0x00;			//mozna je to bila, kdovi... kazdopadne tady bude cerna

void fillBasicUnit(Unit *u){			//new void; uint32_t jako parametr pro ikonu jednotky, ktera ma uint16_t? mozna bude nutnost upravy
	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(i < 4){
				if(i == j || i + j == 15){
					u->graphics[i][j] = BLACK;
				} else if(i < j && i + j < 15){
					u->graphics[i][j] = u.ceiling;
				} else {
					u->graphics[i][j] = 0x1fff;
				}
			} else {
				if(i == 4 && j > 3 && j < 12){
					u->graphics[i][j] = BLACK;
				} else if(j == 4 || j == 11){
					u->graphics[i][j] = BLACK;
				} else if(i == 14 && j > 1 && j < 14){
					u->graphics[i][j] = BLACK;
				} else if(i == 15){
					u->graphics[i][j] = BLACK;
				} else {
					u->graphics[i][j] = 0x1fff;
				}
			}
			/*if(i > j){
				u->graphics[i][j] = 0x1fff;
			} else {
				u->graphics[i][j] = 0x0;
			}	*/
		}
	}
}

int main(int argc, char *argv[])
{
  int changingSelected = 0;	
  int newR = 0;
  int newG = 0;
  int newB = 0;
  
  int NoUnits = 4;
  
  unsigned char *mem_base;

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  uint32_t rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

  
  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);

  //grafclear(0xffff);
  
  printf("Hello world\n");
    
  	int it = 0;
	char unit[16];
	Unit list[20]; // =(* Unit) malloc(sizeof(Unit)*NoUnits);		//pozn. vyresit alokaci a realokaci pameti pro dynamiku jednotek
	char strop[30];
	uint32_t color = 0x0;
	char zed[30];
	
	for(it = 0; it < NoUnits; it++){
			sprintf(unit, "Jednotka %d", it+1);
			list[it].number = it+1;
			strcpy(list[it].name, unit);
			list[it].ceiling = rgb_knobs_value;
			list[it].wall = rgb_knobs_value;
			fillBasicUnit(&list[it]);			//new; fills all basic units
			writeText(list[it].name, 0, it*16);
		}
	
	
	
	while(1){
		grafclear(0xff);
		if (redPushed((int) rgb_knobs_value) && bluePushed((int) rgb_knobs_value) && greenPushed((int) rgb_knobs_value)){
		break;
		}
		
	
		rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	
		
		
		
		
		
		for(int i =0; i< NoUnits; i ++){
			writeText(list[i].name, 0, i*16);
		
		}
		writeText("Pridat jednotku", 0, NoUnits*16);
		writeText("Nastaveni", 160, 0);
		writeText("Smazat", 160, 16);
		writeText("Zpet",160, 32);
		
		writeText("Strop", 320, 0);
		writeText("Stena", 320,16);
		writeText("Vypnout",320, 32);
		writeText("Zapnout",320,48);
		writeText("Zpet",320,64);
		
		int r = red(list[selection-1].ceiling);
		int g = green(list[selection-1].ceiling);
		int b = blue(list[selection-1].ceiling);
		if (sector > 1){
			r = red(list[selected-1].ceiling);
			g = green(list[selected-1].ceiling);
			b = blue(list[selected-1].ceiling);
		}	
		
		if(!((selection == NoUnits + 1) && (sector == 1)) ){
		sprintf(strop," Strop: R = %d , G = %d, B = %d",r ,g ,b);
		writeText(strop,170,170);
		}
		
		if(selected > 0){
		color = list[selected -1].ceiling;
		} else {
		color = list[selection -1].ceiling;

		}
		drawCircle(color, RADIUS, 160,420);
		
		
		if(sector > 1){
			r = red(list[selected-1].wall);
			g = green(list[selected-1].wall);
			b = blue(list[selected-1].wall);
		}
		if(!((selection == NoUnits + 1) && (sector == 1)) ){
			sprintf(zed," Zed: R = %d , G = %d, B = %d",r ,g ,b);
			writeText(zed,170,186);
		}
		
		if(selected > 0){
		color = list[selected -1].wall;
		} else {
		color = list[selection -1].wall;

		}
		drawCircle(color, RADIUS, 210,420);
		
		if(redPushed((int)rgb_knobs_value)){												//new; posun ve vyberu o pozici vys (predchozi jednotka)
			
			if(selection == 1){
				switch (sector){
					case 1:{ selection = NoUnits + 1; break;}
					case 2:{ selection = SECTOR_SIZE_2; break;}
					case 3:{ selection = SECTOR_SIZE_3; break;}
				}
				
				
			} else{
				selection--;
			}
		
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
							for(int i = selected -1; i < NoUnits; i++){   // V mallocku freealloc( OLDUNIT)
								list[i] = list[i+1];
							
							}
							NoUnits--;
							sector = 1;
							selected = -1;
							selection = 1;
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
							
							if(changingSelected == 1){
								changingSelected = 0;
								list[selected -1].ceiling = rgb_knobs_value;
							} else {
							changingSelected = 1;
							}
									
							
							/*
							char tmp[50]
							sprintf(tmp," Nove hdonoty jsou red = %d , green = %d, blue = %d",newR ,newG ,newB);
							writeText(tmp,170,210);
							*/
							
							break;
						}
						case 2:{
							//wall
							
							if(changingSelected == 1){
								changingSelected = 0;
								list[selected -1].wall = rgb_knobs_value;
							} else {
							changingSelected = 1;
							}						
							
							break;
						}
						case 3:{
							//vypnout
							list[selected - 1].ceiling = 0x0;
							list[selected - 1].wall = 0x0;
							break;
						}
						case 4:{
							//zapnout
							
							list[selected - 1].ceiling = 0xffffff;
							list[selected - 1].wall = 0xffffff;
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
				switch (sector){
					case 1:{ if(selection > NoUnits + 1) {selection = 1;} break;}
					case 2:{ if(selection > SECTOR_SIZE_2){selection = 1;} break;}
					case 3:{ if(selection > SECTOR_SIZE_3){selection = 1;} break;}
				}		
		}
	
	
	}
	}	
	
	if(selected < 0){
		getIcon(list[selection - 1]);
	} else {
		getIcon(list[selected - 1]);
	}
		
		if (changingSelected == 1){
			newR = red((int)rgb_knobs_value);
			newG = green((int)rgb_knobs_value);
			newB = blue((int)rgb_knobs_value);
			char tmp[50]; 
			sprintf(tmp," Nove hodnoty jsou R = %d , G = %d, B = %d",newR ,newG ,newB);
			writeText(tmp,170,240);
			
			drawCircle(rgb_knobs_value,RADIUS,280,420);
			}
		grafShow();
		usleep(100000);
	}
	
	printf("Goodbye world\n");

  return 0;
}
