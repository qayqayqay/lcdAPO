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
uint16_t grafika[320][480];
uint16_t sectorUnit[320][160];
uint16_t sectorOptions[320][160];
uint16_t sectorParams[320][160];
uint16_t LETTER = 0x0000;
uint16_t BCG = 0xffff;

void grafclear(unsigned char c)
{
int i;
for (i = 0; i < 320 ; i++) { 
	memset(grafika[i],c,480* sizeof(uint16_t));
  }
}

void grafShow(){
	int i,j;
	grafika = sectorUnit+sectorOptions + sectorParams; 
	for (i = 0; i < 320 ; i++) {
		for (j = 0; j < 480 ; j++) {
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
				grafika[posY + i][posX + j ] = LETTER;             // KTERA SOURADNICE> JE X A KT>ERA Y<<<"""""
			} else {
				grafika[posY + i][posX + j ] = BCG;
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
