#include "global.h"

pthread_t tDataLink;
pthread_t tPhysical;

char path[100];

void *dataLink(void *nodeSrc){
    initDataLink(atoi((char* )nodeSrc), path);
}

void *physical(void *threadid){
    initPhysical();
}

int main(int argc, char* argv[]){
	strcpy(path, argv[2]);
	
	if(pthread_mutex_init(&mutex0, NULL)!=0){
		printf("Erro ao inicializar mutex 0\n");
		return -1;
	}

	if(pthread_mutex_init(&mutex1, NULL)!=0){
		printf("Erro ao inicializar mutex 1\n");
		return -1;
	}

	if(pthread_mutex_init(&mutex2, NULL)!=0){
		printf("Erro ao inicializar mutex 2\n");
		return -1;
	}

	if(pthread_mutex_init(&mutex3, NULL)!=0){
		printf("Erro ao inicializar mutex 3\n");
		return -1;
	}

	if(pthread_mutex_init(&mutex4, NULL)!=0){
		printf("Erro ao inicializar mutex 4\n");
		return -1;
	}

    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex3);

	
	pthread_create(&tPhysical, NULL, physical, (void *)1);
	usleep(1000);
	nodeSrc = atoi(argv[1]);
	pthread_create(&tDataLink, NULL, dataLink, (void *)argv[1]);
	

    pthread_join(tDataLink, NULL);
    pthread_join(tPhysical, NULL);

	
	if(pthread_mutex_destroy(&mutex0)!=0){
		printf("Erro ao destruir mutex 0\n");
		return -1;
	}

	if(pthread_mutex_destroy(&mutex1)!=0){
		printf("Erro ao destruir mutex 1\n");
		return -1;
	}

	if(pthread_mutex_destroy(&mutex2)!=0){
		printf("Erro ao destruir mutex 2\n");
		return -1;
	}

	if(pthread_mutex_destroy(&mutex3)!=0){
		printf("Erro ao destruir mutex 3\n");
		return -1;
	}

	if(pthread_mutex_destroy(&mutex4)!=0){
		printf("Erro ao destruir mutex 4\n");
		return -1;
	}
}