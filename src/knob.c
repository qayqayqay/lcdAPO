#include <stdlib.h>
#include <stdio.h>

int redPushed(int rgb){
	return (rgb>>26)&0x0001;
}

int bluePushed(int rgb){
	return (rgb>>24)&0x0001;
}

int greenPushed(int rgb){
	return (rgb>>25)&0x0001;
}


/*int redRaw(int rgb);
int blueRaw(int rgb);
int greenRaw(int rgb);*/

int red(int rgb){
	return (rgb>>16 & 0xff);
}

int blue(int rgb){
	return (rgb & 0xff);
}

int green(int rgb){
	return (rgb>>8 & 0xff);
}


