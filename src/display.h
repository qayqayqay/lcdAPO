#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t grafika[320][480];

extern void grafclear(unsigned char c);

extern void grafShow();


extern int writeLetter(int ascii, int posX, int posY);
extern void  writeText(char text[], int posX, int posY);


#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*DISPLAY_H*/
