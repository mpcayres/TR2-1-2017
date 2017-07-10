************************************************************
				UNIVERSIDADE DE BRASILIA
************************************************************
TELEINFORMÁTICA E REDES 2 - Link State Distribuído
************************************************************
ALUNOS: 
INGRID SANTANA LOPES		14/0083065
MARCOS PAULO CAYRES ROSA	14/0027131
************************************************************

Ambiente de programação usado:

Ubuntu 14.04.5 LTS
g++ (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4

************************************************************

Instruções de compilação:

com o terminal dentro da pasta src, execute o
comando "gcc -ansi -Wall dijkstraK-LSD.c -o LSD"

************************************************************

Instruções de execução:

- O executável de nome "LSD" aparecerá na pasta src após 
a compilação, basta executar com ./LSD no terminal.

- Será requisitado informar o nó de origem e o de 
destino e a quantidade K de caminhos que se deseja obter.

- O programa será executado com base a rede NSFNET 
declarada no próprio, incluindo a nomeação das cidades.

- Em etapas, será apresentado no terminal informações sobre
as iterações e as correspondentes mensagens transmitidas
até que haja a convergência da topologia.

- Então, será apresentado os K caminhos obtidos a partir
da execução do Dijkstra no database estabelecido.

- Por fim se mostra o database de cada node ao final da 
operação.

************************************************************

Conforme o programa foi baseado na figura 3.11 do livro
do Medhi e o propósito não era a implementação completa
do algoritmo, mas do seu conceito geral, algumas adaptações
foram necessário. Por exemplo:

- O código se refere aos segmentos Inicialization, Link
State Advertisement, Receive e Compute, sem incluir os
casos especiais.

- O campo Age foi definido, mas não foi utilizado, já que
não foi necessário para os casos concebidos.

- A inicialização é feita a partir de uma matriz passada
como parâmetro que inclui os dados referentes ao NFSNET.

- Procura-se simular como a rede performaria, mesmo que
não haja a inclusão, deleção e falha de nós ou links
durante o tempo de execução.