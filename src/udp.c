#DEFINE PORT 55555

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>


int sockfd;

int createSockfd(){
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    return sockfd;
    
}

