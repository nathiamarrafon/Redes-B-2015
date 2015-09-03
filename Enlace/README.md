Camada de Enlace
================

## enlace.c

A biblioteca Enlace representa a Camada de Enlace na Emulação da Topologia da Rede. Os enlaces especificados no arquivo serão emulados através da troca de datagramas UDP entre os processos que emulam os nós conectados na rede virtual. Por exemplo, se o nó 1 estiver conectado ao  nó 2, qualquer mensagem que o nó 1 enviar para o nó 2 deverá ser encapsulada em um datagrama UDP e enviada para o processo que emula o nó 2 através de um soquete UDP. No exemplo acima, o envio de um quadro do nó 1 para o nó 2 corresponderá ao envido de um datagrama UDP para o par (IP,Porta) = (10.0.0.2, 5001).

Considerando que a rede local é bastante confiável, não se espera a ocorrência de erros de rede. Para observar-se erros na rede será introduzido um módulo adulterador de pacotes imediatamente antes de enviar um datagrama UDP. O adulterador irá introduzir os seguintes tipos de erros, de acordo com uma dada probabilidade:
- Perda do pacote
- Corrupção de um ou mais bits do pacote
- Duplicação do pacote

Para usar o adulterador, deve-se trocar todas as instâncias de chamadas à primitiva sendto, da API sockets, por sendto_garbled. Ambas funções têm exatamente os mesmos argumentos.

A biblioteca Enlace possui métodos de leitura de arquivo do tipo .cnf, exibição dos nós e seus enlaces lidos no arquivo, verificar MTU e checksum, criar checksum, inicializar camada de enlace, enviar mensagem para camada física e receber mensagem da camada física.

#### void readFile(char [])
Lê um arquivo do tipo .cnf como o exemplo abaixo, para isso, o método recebe o caminho do arquivo (char []), procura a palavra 'nos' e armazena o id, ip e porta na struct nodus do tipo node, em seguida, procura a palavra 'enl', lê os enlaces e com seus repectivos MTUs e armazena também na variável nodus no campo neighbors exemplo: 1 -> 2, MTU = 2000; no nó de id 1 no campo neighbors indexado por 2-1 já que os índices começam por 0 o valor 2000, que representa o MTU será armazenado, isso será feito até a palavra 'fim' ser encontrada. 
```text
// topologia1.cnf

Nos
   1: IP = 10.0.0.2, Porta = 5000;
   2: IP = 10.0.0.2, Porta = 5001;
   3: IP = 10.0.0.3, Porta = 5000;
   4: IP = 10.0.0.4, Porta = 5000;

Enlaces
   1 -> 2, MTU = 2000;
   2 -> 3, MTU = 5000;
   2 -> 4, MTU = 2000;
   3 -> 4, MTU = 2000;
Fim 
```

#### void printDataLink()
Exibe na tela os nós com seus respectivos enlaces, usando o arquivo acima como exemplo será exibido na tala o seguinte:
```text
1: IP = 10.0.0.2, Porta = 5000
     1 -> 2, MTU = 2000

2: IP = 10.0.0.2, Porta = 5001
     2 -> 3, MTU = 5000
     2 -> 4, MTU = 2000

3: IP = 10.0.0.3, Porta = 5000
     3 -> 4, MTU = 2000

4: IP = 10.0.0.4, Porta = 5000

```
Observação: O método printDataLink atualmente não está sendo utilizado.

#### int  checkMTU(char [], int )
Verifica se o MTU é maior ou igual ao tamanho do pacote que está sendo enviado, para isso, o método recebe o pacote (char []) e o valor do MTU, se o tamanho do pacote for menor retornará falso(-1), caso contrário retornará verdadeiro(0).

#### void createChecksum(char [])
Cria o checksum, um código usado para verificar a integridade de dados transmitidos através de um canal com ruídos, que será adicionado no final do pacote para verificações futuras, para isso, o método recebe o pacote (char []), é utilizado o método crypt da biblioteca crypt.h que gera uma chave a partir da combinação do pacote de dados a ser enviado e a palavra 'keycrypt', a chave gerada possui 14 caracteres mas apenas 3 são adicionados ao fim do pacote.

#### int  checkChecksum(char [])
Verifica se o checksum, um código usado para verificar a integridade de dados transmitidos através de um canal com ruídos, que está no final do pacote de dados que foi recebido é o mesmo que foi adicionado, para isso, o método recebe o pacote (char [])  os últimos 3 caracteres que foram adiiconados no final do pacote são retiradas e armazenados em uma variável, um novo checksum é gerado utilizando o método crypt da biblioteca crypt.h que gera novamente a chave a partir da combinação do pacote de dados a ser enviado e a palavra 'keycrypt', verifica-se se a chave gerada é a mesma que foi retirada anteriomenten do final do pacote, se as chaves forem iguais retornará verdadeiro(0), caso contrário retornará falso(-1).

#### int  initDataLink(int , char [])
Inicializa a camada de enlace, para isso, o método recebe como parâmetro o nó destino (int) e o caminho do arquivo (char []), o método readFile recebe o caminho do arquivo, [readFile](https://github.com/nathiamarrafon/Redes-B-2015/tree/master/Enlace#void-readfilechar-)

Obsercação: Nó destino (int) parâmetro não tilizado, será corrigo ou utilizado na sequência do projeto.
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


#### void *sendPhy();
#### void *receivePhy(void *arg);