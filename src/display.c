#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "unit.h"
#include "display.h"
#include "knob.h"

int selection = 1;
int selected = -1;
int sector = 1;
int choosingPreset = 0;
uint16_t grafika[320][480];
uint16_t sectorUnit[320][160];
uint16_t sectorOptions[320][160];
uint16_t sectorParams[320][160];
uint16_t LETTER = 0x0000;
uint16_t BCG = 0xffff;
uint16_t SELECTED = 0x00ff;

void grafclear(unsigned char c)
{
int i;
for (i = 0; i < 320 ; i++) { 
	memset(grafika[i],c,480* sizeof(uint16_t));
  }
}

void getIcon(Unit u){							//new; prekresli ikonu vybrane jednotky na displej
	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			grafika[299+i][451+j] = u.graphics[i][j];
		}
	}
	
}

void grafShow(){
	int i,j;
	for (i = 0; i < 320 ; i++) {
		for (j = 0; j < 480 ; j++) {
			
			if(choosingPreset == 0){
			
			if((i > 16*(selection-1) && j > (sector-1)*160) && (i < 16*(selection) && j < (sector)*160)){	//new; invertuje text a pozadi na vyberu jed
				if(grafika[i][j] == LETTER){
					grafika[i][j] = BCG;
				} else {
					grafika[i][j] = LETTER;
				}
			}
			
			
			
			if(selected > 0){
				if((i > 16*(selected-1) && j > 0) && (i < 16*(selected ) && j < 160)){	//new; invertuje text a pozadi na vyberu jed
					
					if(grafika[i][j] == LETTER){
						grafika[i][j] = BCG;
					} else {
						grafika[i][j] = SELECTED;
					}
				}
			}
			
			}

			
			parlcd_write_data(parlcd_mem_base, grafika[i][j]);
		}
	}
 }


int writeLetter(int ascii, int posX, int posY){
		int start = (ascii - 32)*16;
		int width = font_winFreeSystem14x16.width[ascii - 32];
		for(int i = 0; i< 16; i++){
			for(int j = 0; j < width ; j++){
			
			int shift = 1 << (15-j);
			
			int tmp  = font_winFreeSystem14x16.bits[start + i] & shift;
			
				if(tmp > 0 ) {
					grafika[posY + i][posX + j] = LETTER;             // KTERA SOURADNICE> JE X A KT>ERA Y<<<"""""
				} else {
					grafika[posY + i][posX + j] = BCG;
				}
			}
		}
		return width;
}

void  writeText(char text[], int posX, int posY){
	int offX = 0;
	int offY = 0;
	for(int i = 0; i < strlen(text); i++){
		offX += writeLetter(text[i],posX + offX,posY + offY);
		if(posX + offX > 466){
			offX = 0;
			offY += 16;
		}
	
	}	
}

void drawCircle(uint32_t color, int radius, int x, int y){
	
	for (int i = 0; i < 320 ; i++) {
		for (int j = 0; j < 480 ; j++) {
			if(((i-x)*(i-x) + (j-y)*(j-y)) < (radius+2)*(radius+2)){
				grafika[i][j] = 0x0;
			}
			if( ((i-x)*(i-x) + (j-y)*(j-y)) < radius*radius  ){
			uint16_t R = (uint16_t)(red(color) >> 3) & 0xff;
			uint16_t G = (uint16_t)(green(color) >> 2) & 0xff;
			uint16_t B = (uint16_t)(blue(color) >> 3) & 0xff;
			grafika[i][j] = (R << 11) + (G << 5) + (B) ;
			
			//grafika[i][j] = 0x1fff;
			}
		}
	}
	}
