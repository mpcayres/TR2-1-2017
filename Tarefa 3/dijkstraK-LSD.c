/*

TELEINFORMATICA E REDES 2
Link State Distribuído

INGRID SANTANA LOPES     -  140083065
MARCOS PAULO CAYRES ROSA -  140027131

*/

#include <stdio.h>
#include <stdlib.h>

#define INF         99999
#define TAM_NSF     14
#define ATIVO       1
#define INATIVO     0

#define SIM         1
#define NAO         0

#define NAO_PRONTO  1
#define PRONTO      0

static int PRONTIDAO = NAO_PRONTO;

typedef struct recebido {
    int num;
    int boolR;
    struct recebido *next;
} recebido;

typedef struct LSA {
    int source;
    int IDorig;
    int IDdest;
    int cost;
    int sequence;   /* para aquela informação em particular*/
    int age;        /* é para todo o sistema*/
    recebido *receb;
} LSA;

typedef struct database {
    int IDorig;
    int IDdest;
    int cost;
    int sequence;
    struct database *next;
} database;

typedef struct node {
    int num;
    int state;
    struct database *data;
    struct node *next;
} node;

recebido *inicializaRecebido(recebido *head, int pos, int sn){
    if(head == NULL){
        head = (recebido*) malloc(sizeof(recebido));
        head->num = pos;
        head->boolR = sn;
        head->next = NULL;
    } else {
        recebido *current = head;
        while(current->next != NULL){
            if(current->num == pos){
                current->boolR = sn;
                return head;
            }
            current = current->next;
        }

        current->next = malloc(sizeof(recebido));
        current->next->num = pos;
        current->next->boolR = sn;
        current->next->next = NULL;
    }

    return head;
}

database *inicializaDatabase(database *head, int orig, int dest, int custo){
    database *current = head;

    if(head == NULL){
        head = malloc(sizeof(database));
        head->IDorig = orig;
        head->IDdest = dest;
        head->cost = custo;
        head->sequence = 1;
        head->next = NULL;
    } else {
        while(current->next != NULL){
            current = current->next;
        }

        current->next = malloc(sizeof(database));
        current->next->IDorig = orig;
        current->next->IDdest = dest;
        current->next->cost = custo;
        current->next->sequence = 1;
        current->next->next = NULL;
    }

    return head;
}

node *inicializaNode(node *head, int val){
    if(head == NULL){
        head = malloc(sizeof(node));
        head->num = val;
        head->state = ATIVO;
        head->data = inicializaDatabase(head->data, val, val, 0);
        head->next = NULL;

    } else {
        node * current = head;
        while (current->next != NULL){
            current = current->next;
        }

        current->next = malloc(sizeof(node));
        current->next->num = val;
        current->next->state = ATIVO;
        current->next->data = inicializaDatabase(current->next->data, val, val,  0);
        current->next->next = NULL;
    }

    return head;
}

void inicializaMatriz(int **matriz, int tam, node *NFSNET){
    int i, j;
    node *aux = NFSNET;
    database *auxDB = aux->data;
    for(i = 0; i < tam; i++){
        for(j = 0; j < tam; j++){
            matriz[i][j] = INF;
        }
    }

    while(auxDB!=NULL){
        matriz[auxDB->IDorig][auxDB->IDdest] = auxDB->cost;
        matriz[auxDB->IDdest][auxDB->IDorig] = auxDB->cost;
        auxDB = auxDB->next;
    }
}

node *adicionaDatabase(node *head, int val, int i, int j){
    node * current = head;
    while(current->next != NULL && current->num != j){
        current = current->next;
    }
    current->data = inicializaDatabase(current->data, j, i, val);

    return head;
}

void limpaData(database * head){
    database * current = head, *aux;

    while(current != NULL){
        aux = current;
        if (current->cost<INF && current->cost!=0)
        printf("\t(%d->%d) custo: %d", current->IDorig, current->IDdest, current->cost);
        current = current->next;
        free(aux);
    }
}

void limpaNode(node * head){
    node* current = head, *aux;

    while (current != NULL){
        aux = current;
        printf("\nnode: %d\n", current->num);
        limpaData(current->data);
        current = current->next;
        free(aux);
        getchar();
    }
}

int getTamanho(node *NFSNET){
    int tam = 0;
    node *aux = NFSNET;

    while(aux != NULL){
        aux = aux->next;
        tam++;
    }

    return tam;
}

database *achaCerto(node *head, int certo){
    node* current = head;
    while(current->next != NULL && current->num != certo){
            current = current->next;
    }

    return current->data;
}

database *checaCerto(database *alvo, database *vizinho){
    database *alvoAux = alvo;
    database *vizinhoAux = vizinho;
    database *current;
    int checa = 0;

    while(vizinhoAux != NULL){
            while(alvoAux != NULL){
                if(alvoAux->IDorig == vizinhoAux->IDorig && alvoAux->IDdest == vizinhoAux->IDdest && 
                    (alvoAux->cost == INF || alvoAux->cost == vizinhoAux->cost)){
                    alvoAux->cost = vizinhoAux->cost;
                    checa++;
                }
                if(alvoAux->IDdest == vizinhoAux->IDorig && alvoAux->IDorig == vizinhoAux->IDdest &&
                    (alvoAux->cost == INF|| alvoAux->cost == vizinhoAux->cost)){
                    alvoAux->cost = vizinhoAux->cost;
                    checa++;
                }
                alvoAux=alvoAux->next;
            }
            if (checa == 0){
                current = alvo;
                while(current->next != NULL){
                    current = current->next;
                }
                current->next = malloc(sizeof(database));
                current->next->IDorig = vizinhoAux->IDorig ;
                current->next->IDdest = vizinhoAux->IDdest;
                current->next->cost = vizinhoAux->cost;
                current->next->sequence = vizinhoAux->sequence;
                current->next->next = NULL;
            }
            checa = 0;


            alvoAux = alvo;
            vizinhoAux = vizinhoAux->next;
    }
    return alvo;
}

database *checaLSA(database *alvo, LSA *mensagem){
    database *alvoAux = alvo;
    LSA *mensagemAux = mensagem;
    database *current;
    int checa = 0;

    if(mensagemAux != NULL){
            
            checa = 0;
            while(alvoAux != NULL){
                if(alvoAux->IDorig == mensagemAux->IDorig && alvoAux->IDdest == mensagemAux->IDdest && 
                    (alvoAux->cost == INF || alvoAux->cost == mensagemAux->cost)){
                    alvoAux->cost = mensagemAux->cost;
                    checa++;
                    break;
                }
                if(alvoAux->IDdest == mensagemAux->IDorig && alvoAux->IDorig == mensagemAux->IDdest &&
                    (alvoAux->cost == INF|| alvoAux->cost == mensagemAux->cost)){
                    alvoAux->cost = mensagemAux->cost;
                    checa++;
                    break;
                }
                alvoAux = alvoAux->next;
            }
            if (checa == 0){

                PRONTIDAO = NAO_PRONTO;
                current = alvo;
                while(current->next != NULL){
                    current = current->next;
                }
                current->next = malloc(sizeof(database));
                    current->next->IDorig = mensagemAux->IDorig ;
                    current->next->IDdest = mensagemAux->IDdest;
                    current->next->cost = mensagemAux->cost;
                    current->next->sequence = mensagemAux->sequence;
                current->next->next = NULL;
            }
    }
    return alvo;
}

node *infoVizinho(node *head, int *vizinhos, int quantidade, int certo){
    node *current = head;
    database *alvo;
    int i = 0;

    alvo = achaCerto (head, certo);
    for(i = 0; i <= quantidade ; i++){
        while(current !=  NULL){
                if(current->num == vizinhos[i])
                    break;
            current = current->next;
        }
                    
        if(current != NULL && current->num == vizinhos[i]){
            alvo = checaCerto(alvo, current->data);
        }
        current = head;
    }

    return head;
}

node *receive(LSA *msgLSA, node *listHello, int pos){
    node *aux, *auxRec;
    database *auxDB, *auxDB2;
    recebido *rec;
    LSA *auxMsg;
    int auxBool;

    for(aux = listHello; aux != NULL; aux = aux->next){
        if(aux->num == pos){

            int boolLSA = 0;
            for(auxDB = aux->data; auxDB != NULL; auxDB = auxDB->next){ /*encontra database do vizinho certo*/
               
                if((auxDB->IDorig == msgLSA->IDorig && auxDB->IDdest == msgLSA->IDdest)
                || (auxDB->IDdest == msgLSA->IDorig && auxDB->IDorig == msgLSA->IDdest)){ /*receive (normal)*/
                   

                    if(auxDB->sequence > msgLSA->sequence){ /*envia LSA de volta para msgLSA->orig*/
                        boolLSA = 2;
                        LSA *msgNova = (LSA*) malloc(sizeof(LSA));
                        msgNova->source = pos;
                        for(auxRec = listHello; auxRec != NULL; auxRec = auxRec->next){
                            if(msgLSA->source == auxRec->num || pos == auxRec->num)
                                msgNova->receb = inicializaRecebido(msgNova->receb, auxRec->num, SIM);
                            else
                                msgNova->receb = inicializaRecebido(msgNova->receb, auxRec->num, NAO);
                        }
                        msgNova->IDorig = auxDB->IDorig;
                        msgNova->IDdest = auxDB->IDdest;
                        msgNova->cost = auxDB->cost;
                        msgNova->sequence = auxDB->sequence;
                        listHello = receive(msgNova, listHello, msgLSA->source);
                    } else{ /*salva novos dados e passa adiante*/
                        boolLSA = 1;
                        auxDB = checaLSA(auxDB, msgLSA);
                    }
                    break;
                }
            }
            if(boolLSA == 1){
                for(auxDB = aux->data; auxDB != NULL; auxDB = auxDB->next){
					/*manda mensagem para os vizinhos*/
                    if(auxDB->IDorig != auxDB->IDdest && auxDB->IDorig == pos && auxDB->cost < INF){
                        for(rec = msgLSA->receb, auxBool = SIM; rec != NULL; rec = rec->next){
                            if(rec->num == auxDB->IDdest){
                                auxBool = rec->boolR;
                                break;
                            }
                        }
                        if(auxBool == NAO){
                            msgLSA->receb = inicializaRecebido(msgLSA->receb, auxDB->IDdest, SIM);
                            auxMsg = msgLSA;
                            for(auxDB2 = aux->data; auxDB2 != NULL; auxDB2 = auxDB2->next){
                                if((auxDB2->IDorig != auxDB2->IDdest &&
                                    auxDB2->IDorig == auxMsg->source && auxDB2->cost < INF)){
                                    auxMsg->receb = inicializaRecebido(auxMsg->receb, auxDB2->IDdest, SIM);
                                }
                            }
                            listHello = receive(auxMsg, listHello, auxDB->IDdest);
                        }
                    }
				}
            } else if(boolLSA == 0){ /*receive (new)*/
                if(auxDB==NULL)
                    auxDB = aux->data;
                auxDB = checaLSA(auxDB, msgLSA); 
            }
            break;
        }
    }

    return listHello;
}

node *LinkStateAdvertisement(node *listHello, int tam, int src){

    node *aux, *auxRec;
    database *auxDB, *auxDB2;
   
    for(aux = listHello; aux != NULL; aux = aux->next){
        printf("\n%d: ", aux->num);

        for(auxDB = aux->data; auxDB != NULL; auxDB = auxDB->next){
            LSA *msgLSA = (LSA*) malloc(sizeof(LSA));
            msgLSA->source = aux->num;

            for(auxRec = listHello; auxRec != NULL; auxRec = auxRec->next){
                if(msgLSA->source == auxRec->num) msgLSA->receb = inicializaRecebido(msgLSA->receb, auxRec->num, SIM);
                else msgLSA->receb = inicializaRecebido(msgLSA->receb, auxRec->num, NAO);
            }
            for(auxDB2 = aux->data; auxDB2 != NULL; auxDB2 = auxDB2->next){
                if((auxDB2->IDorig != auxDB2->IDdest &&
                    auxDB2->IDorig == msgLSA->source && auxDB2->cost < INF)){
                    msgLSA->receb = inicializaRecebido(msgLSA->receb, auxDB2->IDdest, SIM);
                }
            }
           
            if((auxDB->IDorig != auxDB->IDdest &&
                auxDB->IDorig == msgLSA->source && auxDB->cost < INF)){ /*manda a mensagem para os vizinhos*/
                printf("\nPacote #%d->%d (%d)", auxDB->IDorig, auxDB->IDdest, auxDB->cost);
                for(auxDB2 = aux->data; auxDB2 != NULL; auxDB2 = auxDB2->next){
                    if((auxDB2->IDorig != auxDB2->IDdest &&
                        auxDB2->IDorig == msgLSA->source && auxDB2->cost < INF)){ 
                        msgLSA->IDorig = auxDB->IDorig;
                        msgLSA->IDdest = auxDB->IDdest;
                        msgLSA->cost = auxDB->cost;
                        msgLSA->sequence = auxDB->sequence++;
                        listHello = receive(msgLSA, listHello, auxDB2->IDdest);
                    }
                }
            }
        }
    }

    return listHello;
}

node *Hello(int matriz[TAM_NSF][TAM_NSF], int tam){
    int i, j, contV;
	node *NFSNET = NULL;

    for(j = 0; j < tam; ++j){
        NFSNET = inicializaNode(NFSNET, j);
        for(i = 0, contV = 0; i < tam; i++){
            if(i != j){
                NFSNET = adicionaDatabase(NFSNET, matriz[i][j], i, j);
                if(matriz[i][j] != INF && matriz[i][j] != 0)
                    contV++;
            }
        }
        int vizinhos[contV+1];
        for(i = 0, contV = 0; i < tam; i++){
            if(i != j && matriz[i][j] != INF){
                vizinhos[contV] = i;
                contV++;
            }
        }
        NFSNET = infoVizinho(NFSNET, vizinhos, contV-1, j);

    }
    return NFSNET;
}

void nomeia(int num){
    switch(num){
        case 0:  
            printf("Seatle");        
            break;
        case 1:  
            printf("Palo Alto");  
            break;
        case 2:   
            printf("San Diego"); 
            break;
        case 3:  
            printf("Salt Lake City"); 
            break;
        case 4:   
            printf("Boulder"); 
            break;
        case 5:  
            printf("Houston"); 
            break;
        case 6:  
            printf("Lincoln"); 
            break;
        case 7:  
            printf("Champaign"); 
            break;
        case 8:  
            printf("Atlanta"); 
            break;
        case 9:  
            printf("Pittsburgh"); 
            break;
        case 10:  
            printf("Ann Arbor"); 
            break;
        case 11:  
            printf("Ithaca"); 
            break;
        case 12:  
            printf("Princeton"); 
            break;
        case 13:  
            printf("College PK"); 
            break;

         }
}

int dijkstra(int **matriz, int primNo, int fimNo, int k, int tam){
    int custo[tam][tam], visited[tam], dist[tam], antNo[tam];
    int i, j, cont, proxNo, minDist;
    
    for(i = 0; i < tam; i++){
        for(j = 0; j < tam; j++){
            if(matriz[i][j] <= 0) custo[i][j] = INF;
            else custo[i][j] = matriz[i][j];
        }
    }
    
    for(i = 0; i < tam; i++){
        visited[i] = 0;
        dist[i] = custo[primNo][i];
        antNo[i] = primNo;
    }
    
    for(cont = 0, visited[primNo] = 1, dist[primNo] = 0; cont < tam; cont++){
        minDist = INF;
        
        for(i = 0; i < tam; i++){
            if(!visited[i] && dist[i] < minDist){
                minDist = dist[i];
                proxNo = i;
            }
        }
        
        for(i = 0, visited[proxNo] = 1; i < tam; i++){
            if(!visited[i]){
                if((minDist + custo[proxNo][i]) < dist[i]){
                    dist[i] = minDist + custo[proxNo][i];
                    antNo[i] = proxNo;
                }
            }
        }
    }
 
    if(dist[fimNo] == INF) return EXIT_FAILURE;
    printf("\nDistância #%d = %d\n", k, dist[fimNo]);
    for(j = fimNo; j != primNo; j = antNo[j]){
        if(j != fimNo) printf("<-");
        nomeia(j);
    }
    printf("<-");
    nomeia(j);
    printf("\n");

    matriz[antNo[fimNo]][fimNo] = matriz[fimNo][antNo[fimNo]] = INF;

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    int k, PRIM, FIM, KPATH, i, tam = TAM_NSF;
	int **matrizNEW = NULL;

    int matriz[TAM_NSF][TAM_NSF] = {
        {0,     420,    606,    INF,    INF,    INF,    INF,    1000,   INF,    INF,    INF,    INF,    INF,    INF},
        {420,   0,      242,    318,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF},
        {606,   242,    0,      INF,    INF,    723,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF},
        {INF,   318,    INF,    0,      210,    INF,    INF,    INF,    INF,    INF,    846,    INF,    INF,    INF},
        {INF,   INF,    INF,    210,    0,      500,    236,    INF,    INF,    INF,    INF,    INF,    INF,    INF},
        {INF,   INF,    723,    INF,    500,    0,      INF,    INF,    433,    INF,    INF,    INF,    INF,    717},
        {INF,   INF,    INF,    INF,    236,    INF,    0,      277,    INF,    INF,    INF,    INF,    INF,    INF},
        {1000,  INF,    INF,    INF,    INF,    INF,    277,    0,      INF,    269,    INF,    INF,    INF,    INF},
        {INF,   INF,    INF,    INF,    INF,    433,    INF,    INF,    0,      326,    INF,    INF,    INF,    INF},
        {INF,   INF,    INF,    INF,    INF,    INF,    INF,    269,    326,    0,      INF,    142,    164,    INF},
        {INF,   INF,    INF,    846,    INF,    INF,    INF,    INF,    INF,    INF,    0,      INF,    314,    INF},
        {INF,   INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    142,    INF,    0,      INF,    158},
        {INF,   INF,    INF,    INF,    INF,    INF,    INF,    INF,    INF,    164,    314,    INF,    0,      INF},
        {INF,   INF,    INF,    INF,    INF,    717,    INF,    INF,    INF,    INF,    INF,    158,    INF,    0}
    };

    printf("\nInsira o número da cidade de ORIGEM de acordo com a tabela abaixo:");
    printf("\n\t0 -\tSEATTLE\n\t1 -\tPALO ALTO\n\t2 -\tSAN DIEGO\n\t3 -\tSALT LAKE CITY\n\t4 -\tBOULDER");
    printf("\n\t5 -\tHOUSTON\n\t6 -\tLINCOLN\n\t7 -\tCHAMPAIGN\n\t8 -\tATLANTA\n\t9 -\tPITTSBURGH");
    printf("\n\t10 -\tANN ARBOR\n\t11 -\tITHACA\n\t12 -\tPRINCETON\n\t13 -\tCOLLEGE PK\n>>>> ");
    scanf("%d", &PRIM);

    printf("\nInsira o número da cidade de DESTINO de acordo com a tabela abaixo:");
    printf("\n\t0 -\tSEATTLE\n\t1 -\tPALO ALTO\n\t2 -\tSAN DIEGO\n\t3 -\tSALT LAKE CITY\n\t4 -\tBOULDER");
    printf("\n\t5 -\tHOUSTON\n\t6 -\tLINCOLN\n\t7 -\tCHAMPAIGN\n\t8 -\tATLANTA\n\t9 -\tPITTSBURGH");
    printf("\n\t10 -\tANN ARBOR\n\t11 -\tITHACA\n\t12 -\tPRINCETON\n\t13 -\tCOLLEGE PK\n>>>> ");
    scanf("%d", &FIM);

    printf("\nInsira o número de caminhos que deseja: ");
    scanf("%d", &KPATH);
	getchar();

    if(PRIM == FIM){
        printf("\nDistância = 0\nNó final e de início são o mesmo: ");
        nomeia(PRIM);
		printf("%d", PRIM);
        printf("\n");
        return EXIT_SUCCESS;
    }

    node *NFSNET = Hello(matriz, tam);
    for(i = 1; PRONTIDAO == NAO_PRONTO; i++){
        printf("\n\n\tIniciando Interação >> %d\n", i);
        getchar();
        PRONTIDAO = PRONTO;
        NFSNET = LinkStateAdvertisement(NFSNET, tam, PRIM);
    }
    printf("\nTodos os nodes convergiram na topologia! \n");

    tam = getTamanho(NFSNET);
    matrizNEW = (int**) malloc(tam*sizeof(int*));
    if(!matrizNEW){
            printf("\nNão foi possível alocar memória.\n");
            return EXIT_FAILURE;
    }

    for(i = 0; i < tam; i++){
        matrizNEW[i] = (int*) malloc(tam*sizeof(int));
        if(!matrizNEW[i]){
            printf("\nNão foi possível alocar memória.\n");
            return EXIT_FAILURE;
        }
    }

    inicializaMatriz(matrizNEW, tam, NFSNET);
    printf("\nExecutando Dijkstra para ver o melhor caminho...\n(aperte enter para ver as rotas estabelecidas):");
	
	getchar();

    for(k = 1; k <= KPATH; k++){
        if(dijkstra(matrizNEW, PRIM, FIM, k, tam) == EXIT_FAILURE) break;
    }
    printf("\nDatabase ao final da operação\n(aperte enter para ver o correspondente a cada node):\n");
	getchar();
    limpaNode(NFSNET);

    return EXIT_SUCCESS;

}
