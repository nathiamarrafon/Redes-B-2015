#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <crypt.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>


#include "garbler.h"

pthread_mutex_t mutex0; 
pthread_mutex_t mutex1; 
pthread_mutex_t mutex2; 
pthread_mutex_t mutex3;
pthread_mutex_t mutex4;

pthread_t tSend;
pthread_t tReceive;

char DataLink_Physical[100];
int nodeSrc;
int nodeDst;

int errorDataLink;