#ifndef UNIT_H
#define UNIT_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
	int number;
	char name[16];
	uint32_t ceiling;
	uint32_t wall;
	uint16_t graphics[16][16];
} Unit;



#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /*UNIT_H*/
