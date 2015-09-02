/**
  * 10109031 Francesco Abrahão
  * 11742244 Marcello da Silva Sousa 
  * 11742251 Náthia de Souza Marrafon
  * 11078359 Paolla Catherine S. M. Agard
***/

#include "global.h"

typedef struct {
	int id;
	char ip[17];
	int port;
	int neighbors[6];
}node;


node nodus[6];
struct sockaddr_in physicalSend, physicalReceive, teste;



void readFile(char []);
void printDataLink();
int  checkMTU(char [], int );
void createChecksum(char []);
int  checkChecksum(char []);
int  initDataLink(int , char []);

void *sendPhy();
void *receivePhy(void *arg);


/**
  * Função para ler o arquivo,exibir na tela os nós e seus vizinhos
  *
  * param path: caminho do arquivo
***/
void readFile(char path[]) {
	FILE *file;
	char charFile;
	char strAux[4] = "ooo";

	int noRead = 0, i = 0, neighbors = 0, indexA, indexB;

	if ((file = fopen(path, "r")) == NULL) {
		perror("Impossivel abrir o arquivo!\n");
		exit(1);
	}

/* ===== START STRUCT ENLACE ===================================== */
	for (indexA = 0; indexA < 6; indexA++) {
		nodus[indexA].id = 0;
		nodus[indexA].ip[0] = '\0';
		nodus[indexA].port = 0;
		for (indexB = 0; indexB < 6; indexB++) {
			(indexA == indexB) ? (nodus[indexA].neighbors[indexB] = -1) : (nodus[indexA].neighbors[indexB] = 0);
		}
	}

/* ===== Verifica o comando ====================================== */
	while ((charFile = fgetc(file)) != EOF) {
		char auxStruct[17];

		for (indexA = 1; indexA< strlen(strAux); indexA++) {
			strAux[indexA - 1] = strAux[indexA];
		}
		(charFile > 96 && charFile < 123) ?	(strAux[strlen(strAux) - 1] = charFile) : (strAux[strlen(strAux) - 1] = charFile+32);

		if (!strcmp(strAux, "nos")) {
			noRead = 1;
		}
		else if (!strcmp(strAux, "enl")) {
			noRead = 100;
		}
		else if (!strcmp(strAux, "fim")) {
			fclose(file);
			break;
		}

/* ===== Lê os nós =============================================== */
		if (charFile >= 48 && charFile <= 57 && noRead == 1) {
			nodus[i].id = charFile - 48;
		}
		else if (charFile == '=' && noRead == 1) {
			noRead++;
		}
		else if (noRead > 1 && noRead < 18 && charFile != ',') {
			if (charFile != ' ') {
				auxStruct[noRead - 2] = charFile;
				noRead++;
			}
		}
		else if (charFile == ',' && noRead < 100) {
			auxStruct[noRead - 2] = '\0';
			noRead = 20;
			strcpy(nodus[i].ip, auxStruct);
			strcpy(auxStruct, "");
		}
		else if (charFile == '=' && noRead == 20) {
			noRead++;
		}
		else if (noRead > 20 && noRead < 100 && charFile != ';') {
			if (charFile != ' ') {
				auxStruct[noRead - 21] = charFile;
				noRead++;
			}
		}
		else if (charFile == ';' && noRead < 100) {
			auxStruct[noRead - 21] = '\0';
			noRead = 1;
			nodus[i].port = atoi(auxStruct);
			i++;
		}

/* ===== Lê os enlaces =========================================== */
		else if (charFile >= 48 && charFile <= 57 && noRead == 100) {
			i = charFile - 48;
		}
		else if (charFile == '>' && noRead == 100) {
			noRead++;
		}
		else if (charFile != ',' && noRead == 101) {
			if (charFile != ' ' && charFile != '>') {
				neighbors = charFile - 48;
				noRead = 102;
			}
		}
		else if (charFile == '=' && noRead == 102) {
			noRead++;
		}
		else if (charFile != ';' &&  noRead > 102) {
			if (charFile != ' ') {
				auxStruct[noRead - 103] = charFile;
				noRead++;
			}
		}
		else if (charFile == ';' &&  noRead > 103) {
			auxStruct[noRead - 103] = '\0';
			nodus[i - 1].neighbors[neighbors - 1] = atoi(auxStruct);
			noRead = 100;
		}
	}
	// printDataLink();
}


/**
  * Função para exibir na tela os nós e seus vizinhos
***/
void printDataLink() {
	int indexA, indexB;
	
	for (indexA = 0; indexA < 6; indexA++) {
		if (nodus[indexA].id != 0) {
			printf("%d: IP = %s, Porta = %d\n", nodus[indexA].id, nodus[indexA].ip, nodus[indexA].port);
			for (indexB = 0; indexB < 6; indexB++) {
				if (nodus[indexA].neighbors[indexB] > 0) {
					printf("     %d -> %d, MTU = %d\n", nodus[indexA].id, indexB+1, nodus[indexA].neighbors[indexB]);
				}
			}
			printf("\n");
		}
	}
}


/**
  * Função para verificar o MTU
  *
  * param package: Pacote
  * param mtu: MTU máximo que pode ser recebido/enviado
***/
int checkMTU(char package[], int mtu) {
	return (strlen(package) > mtu) ? -1 : 0;
}

/**
  * Função para adicionar a detecção de erros no final do pacote.
  * Foi utilizado a função crypto <crypt.h> para criar uma chave única
  * para cada pacote, essa chave é concatenada ao final do pacote
  *
  * param package: Pacote
***/
void createChecksum(char package[]) {
	char checksum[14];

	strcpy(checksum, crypt(package, "keycrypt"));
	checksum[3] = '\0';
	strcat(package, checksum);
}

/**
  * Função para verificar a detecção de erros no final do pacote.
  * A chave presente no final do pacote é retirada e verifica-se 
  * se é a mesma que foi adicionada anteriomente
  *
  * param package: Pacote
***/
int checkChecksum(char package[]) {
	char checksum1[4], checksum2[14];
	int indexA, sizePackge = strlen(package);

	for(indexA=3; indexA>=0; indexA--, sizePackge--){
		checksum1[indexA]=package[sizePackge];
	}

	package[sizePackge+1]='\0';
	strcpy(checksum2, crypt(package, "keycrypt"));
	checksum2[3] = '\0';

	return (strcmp(checksum1, checksum2)==0) ? 0 : -1;
}

/**
  * Função para enviar para camada Física. 
  *
  * param arg: Nó destino
***/
void *sendPhy(){
	char package[100];
	int mtu, error, sock;

	while(1){
		error = 0;
		if(pthread_mutex_lock(&mutex2)!=0){
			printf("Erro ao travar o mutex 2");
			break;
			//return (-1);
		}
	
		if(pthread_mutex_lock(&mutex1)!=0){
			printf("Erro ao travar o mutex 1");
			break;
			//return (-1);
		}
		memcpy(package, DataLink_Physical, 100);

		// CATHERINE
		nodeDst=package[0]-48;
		
        mtu = nodus[nodeSrc-1].neighbors[nodeDst-1];
        if (nodus[nodeSrc-1].id == 0) {
        	error = 202;
        } else if(mtu == -1){
			error = -303;
		} else if (mtu == 0){
			error = -404;
		}
    
		createChecksum(package);

		if(checkMTU(package, mtu)==-1 && error == 0){
			error = -505;
		}
		
		if(error >= 0){
			if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
				perror("socket()");
				exit(1);
			}

			physicalSend.sin_family      = AF_INET;										
			physicalSend.sin_port        = htons(nodus[nodeDst-1].port);	  
			physicalSend.sin_addr.s_addr = inet_addr(nodus[nodeDst-1].ip);

			if(set_garbler(0,0,0) < 0){
				printf("set_garbler()");
				break;
				//return -1;
			}

			if((sendto_garbled(sock, package, 100, 0, (struct sockaddr *)&physicalSend, sizeof(physicalSend))) < 0){
				if (error >=0 ){
					error = -606;
				}
			}
			close(sock);
		}

        if(pthread_mutex_unlock(&mutex0)!=0){
			printf("Erro ao destravar o mutex 0");
			break;
			//return (-1);
		}
		
		errorDataLink=error;

		if(pthread_mutex_unlock(&mutex2)!=0){
			printf("Erro ao destravar o mutex 2");
			break;
			//return (-1);
		}
	}
}


/**
  * Função para inicializar a camada Enlace. 
  *
  * param nodeSrc: Nó destino
  * param path: Caminho do arquivo
***/
int initDataLink(int nodeSrc, char path[] ){

	
	readFile(path);



	if(pthread_create(&tSend, NULL, sendPhy, NULL)){
		printf("Erro ao criar a thread para enviar\n");
		return -1;
	}

	if(pthread_create(&tReceive, NULL, receivePhy, NULL)){
		printf("Erro ao criar a thread para receber\n");
		return -1;
	}

	if(pthread_join(tSend, NULL)!=0){
		printf("Erro Thread Join\n");
	}

	if(pthread_join(tReceive, NULL)!=0){
		printf("Erro Thread Join\n");
	}
}


/**
  * Função para recever da camada Física. 
  *
  * param arg: Nó origem
***/
void *receivePhy(void *arg){
	nodeDst=1;
	char package[100];
	int address_size, sock, bla;

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket()");
		exit(1);
	}

	physicalReceive.sin_family      = AF_INET;										
	physicalReceive.sin_port        = htons(nodus[nodeDst-1].port);	
	physicalReceive.sin_addr.s_addr = INADDR_ANY; // inet_addr(nodus[nodeSrc].ip);	

	if (bind(sock, (struct sockaddr *)&physicalReceive, sizeof(physicalReceive)) < 0){
		perror("bind()");
		exit(1);
	}

	while(1){
		if((pthread_mutex_lock(&mutex4))!=0){
			printf("Erro ao travar o mutex 4");
			break;
			// return (-1);
		}

 		// CATHERINE
		fflush(stdout); 	

		bla = sizeof(physicalReceive);
	    if (getsockname(sock, (struct sockaddr *) &physicalReceive, &bla) < 0) {
	        perror("getsockname()");
	        exit(1);
	    }

		address_size = sizeof(teste);
		if((recvfrom(sock, &package, sizeof(package), 0, (struct sockaddr *)&teste, &address_size)) < 0){
			perror("recvfrom()");
			exit(1);
		}

		if(checkChecksum(package)==-1){
			printf("Checksum nao correspondente\n");
		} else {
			memcpy(DataLink_Physical, package, 100);
		}

    	if(pthread_mutex_unlock(&mutex3)!=0){
		    printf("Erro ao destravar o mutex 3");
		    break;
		    // return (-1);
		}

	}
	close(sock);
}