#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "unit.h"


#ifdef __cplusplus
extern "C" {
#endif

extern int selection;		//new; ukazatel na konkretni jednotku

extern int selected;		//new; oznaceni KONKRETNI jednotky, ktera je vybrana jednotky, mozna nebude treba

extern int sector;			//new; ukazatel na konkretni ze tri sektoru

extern int choosingPreset;

extern uint16_t grafika[320][480];

extern void grafclear(unsigned char c);

extern void grafShow();

extern void getIcon(Unit u);
extern void drawCircle(uint32_t color, int radius, int x, int y);


extern int writeLetter(int ascii, int posX, int posY);
extern void  writeText(char text[], int posX, int posY);


#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*DISPLAY_H*/
