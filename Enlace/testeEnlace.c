#include "global.h"

void *sendPhyDL(void* threadid){
    char package[100];
    int error;
    pthread_mutex_unlock(&mutex1);
    while(1){
        pthread_mutex_lock(&mutex0);
        printf("Mensagem: ");
        __fpurge(stdin);
        scanf("%s", package);
        __fpurge(stdin);

        memcpy(DataLink_Physical, package, 100);
        pthread_mutex_unlock(&mutex1);

        usleep(100);
        error = errorDataLink;
        if(error == -202) {
            printf("Erro %d: no nao valido.\n\n", error*(-1));
        } else if(error == -303) {
            printf("Erro %d: proprio no.\n\n", error*(-1));
        } else if(error == -404) {
            printf("Erro %d: no não vizinho.\n\n", error*(-1));
        } else if(error == -505){
            printf("Erro %d: dados excedem MTU.\n\n", error*(-1));
        } else if(error == -606){
            printf("Erro %d:sendto_garbled().\n\n", error*(-1));
        } else {
            printf("Enviado com sucesso.\n\n");
        }
        fflush(stdout);

        //__fpurge(stdout);
        usleep(100);
    }
}

void* receivePhyDL(void* threadid){
    while(1){
        pthread_mutex_lock(&mutex3); 

        printf("\nMensagem Recebida: %s\n", DataLink_Physical);
        fflush(stdout);
        memset(DataLink_Physical, 0, sizeof(DataLink_Physical));

        pthread_mutex_unlock(&mutex4);
    }
}

void initPhysical(){
    pthread_create(&tSend, NULL, sendPhyDL, (void* )1);
    usleep(100);
    pthread_create(&tReceive, NULL, receivePhyDL, (void* )1);
    usleep(100);
    pthread_join(tReceive, NULL);
    pthread_join(tSend, NULL);
}