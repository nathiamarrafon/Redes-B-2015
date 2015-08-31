#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int save_msg(int s);
int num_msg=0;

struct mensagem{
  char usuario[10];
  char msg[50];
  int controle;
  
} salvas[5], recebe;


/*
 * Servidor UDP
 */
int main(int argc, char *argv[]){
	int sockint,s, namelen, port;
	struct sockaddr_in client, server;
	
	/*
	* Cria um socket UDP (dgram). 
	*/
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket()");
		exit(1);
	}

   /*
    * Define a qual endereço IP e porta o servidor estará ligado.
    * Porta = 0 -> faz com que seja utilizada uma porta qualquer livre.
    * IP = INADDDR_ANY -> faz com que o servidor se ligue em todos
    * os endereços IP
    */
	port = atoi(argv[1]);
	
	server.sin_family      = AF_INET;   /* Tipo do endereço             */
	server.sin_port        = htons(port);         /* Escolhe uma porta disponível */
	server.sin_addr.s_addr = INADDR_ANY;/* Endereço IP do servidor      */
	
	/*
	* Liga o servidor à porta definida anteriormente.
	*/
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0){
		perror("bind()");
		exit(1);
	}

   /* Consulta qual porta foi utilizada. */
	namelen = sizeof(server);
	if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0){
		perror("getsockname()");
		exit(1);
	}
	
	/* Imprime qual porta foi utilizada. */
	printf("Porta utilizada é %d\n", ntohs(server.sin_port));
	
	/*
	* Recebe uma mensagem do cliente.
	* O endereço do cliente será armazenado em "client".
	*/
	
	while(1=1){
		save_msg(s);
	}
   
	/*
	* Fecha o socket.
	*/
	close(s);
}

int save_msg(int x){	
	
	struct sockaddr_in client; 
	static aux=0; 
	int client_address_size,s;
	
	s=x;
	
	client_address_size = sizeof(client);
	
	if(recvfrom(s, &recebe, sizeof(recebe), 0, (struct sockaddr *) &client, &client_address_size) <0){
		perror("recvfrom()");
		exit(1);
	}
	
	if (recebe.controle == 1){
		if (sendto(s, salvas, sizeof(salvas), 0, (struct sockaddr *)&client, sizeof(client)) < 0){
			perror("sendto()");
			exit(2);
		}
		
		recebe.controle == 0;
	} else if(num_msg >=5){
		printf("\nO Limite de mensagens gravadas é 5\nnao eh possivel gravar mais mensagens \n");
	} else {
		strcpy(salvas[aux].usuario , recebe.usuario);
		strcpy(salvas[aux].msg , recebe.msg);
		
		aux++;
			num_msg++;
		
		printf("Cadastro Efetuado com Sucesso\n");
	}   	      
}