
#include "global.h"

typedef struct {
	int id;
	char ip[12];
	int port;
	int neighbors[6];
}node;


node nodus[6];
struct sockaddr_in physicalSend, physicalReceive;


void reafFile(char path[]);
void printDataLink();
int  checkMTU(char package[], int mtu);
void createChecksum(char package[]);
int  checkChecksum(char package[]);

void *sendPhy(void *arg);
void *receivePhy(void *arg);

int main(int argc, char *argv[]) {
	char dados[100];
	strcpy(dados, "Paolla");



	reafFile(argv[1]);
	printDataLink();
	createChecksum(dados);
	printf("%d\n", checkChecksum(dados));
	receivePhy(&argv);
}


void reafFile(char path[]) {
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

/* ===== VERIFY COMMAND ========================================== */
	while ((charFile = fgetc(file)) != EOF) {
		char auxStruct[16];

		for (indexA = 1; indexA< strlen(strAux); indexA++) {
			strAux[indexA - 1] = strAux[indexA];
		}
		(charFile > 96 && charFile < 123) ?	(strAux[strlen(strAux) - 1] = charFile) : (strAux[strlen(strAux) - 1] = charFile+32);

		if (!strcmp(strAux, "nodus")) {
			noRead = 1;
		}
		else if (!strcmp(strAux, "enl")) {
			noRead = 100;
		}
		else if (!strcmp(strAux, "fim")) {
			fclose(file);
			break;
		}

/* ===== READ nodus ================================================ */
		if (charFile >= 48 && charFile <= 57 && noRead == 1) {
			nodus[i].id = charFile - 48;
		}
		else if (charFile == '=' && noRead == 1) {
			noRead++;
		}
		else if (noRead > 1 && noRead < 17 && charFile != ',') {
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

/* ===== READ ENLACES ============================================ */
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
}

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

int checkMTU(char package[], int mtu) {
	return (strlen(package) > mtu) ? -1 : 0;
}


void createChecksum(char package[]) {
	char checksum[14];

	strcpy(checksum, crypt(package, "keycrypt"));
	checksum[3] = '\0';
	strcat(package, checksum);
}


int checkChecksum(char package[]) {
	char checksum1[4], checksum2[14];
	int indexA, sizePackge = strlen(package);

	for(indexA=3; indexA>=0; indexA--, sizePackge--){
		checksum1[indexA]=package[sizePackge];
	}

	package[sizePackge+1]='\0';
	strcpy(checksum2, crypt(package, "keycrypt"));
	checksum2[3] = '\0';

	return (strcmp(checksum1, checksum2)==0) ? 0: -1;
}

/*
void *sendPhy(void *arg){
	char dados[100];
	int mtu, no_destino, erro, random, soc;
	char no_dest[2];
	int l_no_emulado=(int)arg;

	while(1){

		erro=0;

	
		if(pthread_mutex_lock(&mutex[2])!=0){
			printf("Erro travar");
			return (-1);
		}
	

		if(pthread_mutex_lock(&mutex[1])!=0){
			printf("Erro travar");
			return (-1);
		}
		memcpy(dados, rede_enlace, 100);

	

		no_dest[0]=dados[0];
		no_dest[1]='\0';
		no_destino=atoi(no_dest);

		

        mtu = enlaces[l_no_emulado][no_destino];
		if(mtu==-1){
			erro=-10;
		}
    
   

		checksum_criar(dados);

		

		if(verificaMTU(dados, mtu)==-1 && erro == 0){
			erro=-20;
		}

		

		if(erro>=0){
			if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
				perror("socket()");
				exit(1);
			}

			/* Define o endereço IP e a porta do destino */
			/* fisica_env.sin_family      = AF_INET;										/* Tipo do endereço         */
			/* fisica_env.sin_port        = htons(atoi(nos[no_destino].porta));	/* Porta do servidor        */
			/*fisica_env.sin_addr.s_addr = inet_addr(nos[no_destino].ip);			/* Endereço IP do servidor  */

			/*if(sendto_garbled(soc, dados, 100,0, (struct sockaddr *)&fisica_env,sizeof(fisica_env))<0){
				erro = -30;
			}
			close(soc);
		}

        if(pthread_mutex_unlock(&mutex[0])!=0){
			printf("Erro destravar");
			return (-1);
		}
		



		erro_enlace_rede=erro;

		if(pthread_mutex_unlock(&mutex[2])!=0){
			printf("Erro destravar");
			return (-1);
		}
	}
}*/



void *receivePhy(void *arg){

	char package[100];
	int address_size;

	int l_soc= (int )arg;
	printf("%d \n",l_soc );
	while(1){
		if((pthread_mutex_lock(&mutex[4]))!=0){
			printf("Erro ao travar o mutex 4");
			break;
			// return (-1);
		}

		if(recvfrom(l_soc, package, 100, 0, (struct sockaddr *)&physicalReceive, &address_size) <0){
			perror("recvfrom()");
			exit(1);
		}

		if(checkChecksum(package)==-1){
			printf("Checksum nao correspondente\n");
		} else {
			memcpy(DataLink_Physical, package, 100);
		}

    	if(pthread_mutex_unlock(&mutex[3])!=0){
		    printf("Erro ao destravar o mutex 3");
		    break;
		    // return (-1);
		}
	}
}