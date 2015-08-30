#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
	int id;
	char ip[12];
	int port;
	int neighbors[6];
};

no nos[6];
void reafFile(char path[]);
void printEnlace();

int main(int argc, char *argv[]) {


	reafFile("topologia1.cnf");
	printEnlace();

	system("pause");
}


void reafFile(char path[]) {
	FILE *file;
	char charFile;
	char strAux[4] = "ooo";
	

	if ((file = fopen(path, "r")) == NULL) {
		fprintf(stderr, "Impossivel abrir o arquivo!\n");
		exit(1);
	}

	int noRead = 0;
	int i = 0, neighbors = 0;

/* ===== START ENLACE ============================================ */
	for (int indiceA = 0; indiceA < 6; indiceA++) {
		for (int indiceB = 0; indiceB < 6; indiceB++) {
			indiceA == indiceB ? nos[indiceA].neighbors[indiceB] = -1 : nos[indiceA].neighbors[indiceB] = -0;
		}
	}

	while ((charFile = fgetc(file)) != EOF) {
		char auxStruct[16];

		for (int indice = 1; indice < strlen(strAux); indice++) {
			strAux[indice - 1] = strAux[indice];
		}
		strAux[strlen(strAux) - 1] = charFile;

		if (!strcmp(strAux, "Nos")) {
			noRead = 1;
		}
		else if (!strcmp(strAux, "Enl")) {
			noRead = 100;
		}
		else if (!strcmp(strAux, "Fim")) {
			fclose(file);
			break;
		}

/* ===== READ NOS ================================================ */
		if (charFile >= 48 && charFile <= 57 && noRead == 1) {
			nos[i].id = charFile - 48;
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
			strcpy(nos[i].ip, auxStruct);
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
			nos[i].port = atoi(auxStruct);
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
			nos[i - 1].neighbors[neighbors - 1] = atoi(auxStruct);
			noRead = 100;
		}
	}
}

void printEnlace() {
	for (int indiceA = 0; indiceA < 6; indiceA++) {
		if (nos[indiceA].id != 0) {
			printf("%d: IP = %s, Porta = %d\n", nos[indiceA].id, nos[indiceA].ip, nos[indiceA].port);
			for (int indiceB = 0; indiceB < 6; indiceB++) {
				if (nos[indiceA].neighbors[indiceB] > 0) {
					printf("     %d -> %d, MTU = %d\n", nos[indiceA].id, indiceB+1, nos[indiceA].neighbors[indiceB]);
				}
			}
			printf("\n");
		}
	}
}