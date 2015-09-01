#include "global.h"

pthread_t tSendPhy;
pthread_t tReceivePhy;

void *sendPhyDL(void* threadid){
    char package[100];
    int error;

    while(1){
        pthread_mutex_lock(&mutex0);
        printf("Mensagem: ");
        __fpurge(stdin);
        scanf("%s[^\n]", package);
        __fpurge(stdin);
        memcpy(DataLink_Physical, package, 100);
        pthread_mutex_unlock(&mutex1);

        usleep(100);
        error = errorDataLink;
        if(error != 0){
            printf("Erro %d: ", error);
            if(error == -404) {
                printf("no nao encontrado.\n");
            } else if(error == -505){
                printf("dados excedem MTU.\n");
            }
            // } else if(error == -606){
            //     printf("sendto_garbled().\n");
            // }
            fflush(stdout);
        }
        usleep(100);
    }
}

void* receivePhyDL(void* threadid){
    char package[100];
    
    while(1){
        pthread_mutex_lock(&mutex3);
        memcpy(package, DataLink_Physical, 100);
        printf("\nMensagem: %s\n", package);
        fflush(stdout);
        pthread_mutex_unlock(&mutex4);
    }
}

void initPhysical(){
    pthread_create(&tSendPhy, NULL, sendPhyDL, (void* )1);
    usleep(20);
    pthread_create(&tReceivePhy, NULL, receivePhyDL, (void* )1);
    pthread_join(tSendPhy, NULL);
    pthread_join(tReceivePhy, NULL);
}