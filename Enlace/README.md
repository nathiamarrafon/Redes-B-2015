Camada de Enlace
================

## enlace.c

A biblioteca Enlace representa a Camada de Enlace na Emulação da Topologia da Rede. Os enlaces especificados no arquivo serão emulados através da troca de datagramas UDP entre os processos que emulam os nós conectados na rede virtual. Por exemplo, se o nó 1 estiver conectado ao  nó 2, qualquer mensagem que o nó 1 enviar para o nó 2 deverá ser encapsulada em um datagrama UDP e enviada para o processo que emula o nó 2 através de um soquete UDP. No exemplo acima, o envio de um quadro do nó 1 para o nó 2 corresponderá ao envido de um datagrama UDP para o par (IP,Porta) = (10.0.0.2, 5001).

Considerando que a rede local é bastante confiável, não se espera a ocorrência de erros de rede. Para observar-se erros na rede será introduzido um módulo adulterador de pacotes imediatamente antes de enviar um datagrama UDP. O adulterador irá introduzir os seguintes tipos de erros, de acordo com uma dada probabilidade:
- Perda do pacote
- Corrupção de um ou mais bits do pacote
- Duplicação do pacote

Para usar o adulterador, deve-se trocar todas as instâncias de chamadas à primitiva sendto, da API sockets, por sendto_garbled. Ambas funções têm exatamente os mesmos argumentos.

A biblioteca Enlace possui métodos de leitura de arquivo do tipo .cnf, impressão na tela dos nós e enlaces lidos no arquivo, verificar MTU e checksum, criar checksum, inicializar camada de enlace, enviar mensagem para camada física e receber mensagem da camada física.

#### void readFile(char []);
```text
// topologia1.cnf

Nós
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
#### void printDataLink();

#### int  checkMTU(char [], int );
#### void createChecksum(char []);
#### int  checkChecksum(char []);
#### int  initDataLink(int , char []);
#### void *sendPhy();
#### void *receivePhy(void *arg);