************************************************************
				UNIVERSIDADE DE BRASILIA
************************************************************
TELEINFORMÁTICA E REDES 2
		K SHORTEST PATH
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
comando "gcc -ansi -Wall dijkstraK.c -o dijkstraK"

************************************************************

Instruções de execução:

o executável de nome "dijkstraK" aparecerá na pasta
src após a compilação, basta executar com ./dijkstraK

No terminal será requisitado informar o nó de origem e
o de destino e o quantidade K de caminhos que se deseja
obter

É possível passar como parâmetro uma arquivo txt (presente
na mesma pasta) que contenha o tamanho da matriz na 
primeira linha e os custo presentes nela separados por 
vírgula (custo infinito = -1), conforme o arquivo de 
exemplo "nsf.txt"

Caso não haja nenhum parâmetro, o programa será executado
baseado no na rede NSFNET declarada no próprio, incluindo
a nomeação das cidades, neste caso
