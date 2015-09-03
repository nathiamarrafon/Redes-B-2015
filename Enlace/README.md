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
#### void printDataLink();
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


#### int  checkMTU(char [], int );
#### void createChecksum(char []);
#### int  checkChecksum(char []);
#### int  initDataLink(int , char []);
#### void *sendPhy();
#### void *receivePhy(void *arg);


Cada nó da rede virtual será emulado por uma instância de execução do programa desenvolvido e endereçado por um par (<endereço da máquina>,<porta UDP>). Uma mesma estação poderá, portanto, executar vários nós virtuais.

	

Figura 1	

Um arquivo com uma Configuração Inicial (CI) especificará os nós da rede virtual e as ligações entre eles. Por exemplo, a topologia ilustrada pela figura 1, seria descrita por um arquivo com o seguinte conteúdo:

O arquivo com a CI compreende duas seções. Na seção Nós são identificados os nós e os endereços IP e porta UDP associados a cada um. Na seção enlaces são feitas as ligações entre os nós e os o MTU (Maximum Transfer Unit) de cada enlace.

O arquivo de configuração poderá incluir um número arbitrário de espaços entre tokens sucessivos além de linhas em branco, não sendo feita distinção entre caracteres maiúsculos e minúsculos.

Ao executar o programa que implementa a pilha de protocolos, serão passados dois argumentos na linha de comando, o nome de um arquivo de configuração inicial e o identificador do nó a emular.

A CI define a topologia inicial da rede. Esta topologia pode mudar dinamicamente em virtude de:

1.     Falha de um nó, decorrentes do cancelamento do processo que o emula;
2.     Falha de um enlace, decorrente da remoção lógica do enlace, conforme será discutido na seção 6.
Ambas falhas podem ser permanentes ou temporárias. Ou seja, um nó ou enlace que foram removidos podem ser reiniciados. Entretanto, não haverá a introdução dinâmica de nós ou enlaces que não estejam presentes na topologia inicial.

Assuma que a rede poderá ter até 6 nós, e que cada um poderá estar ligado a até outros 3.