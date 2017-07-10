#include <stdio.h>
#include <stdlib.h>

#define INF			99999
#define TAM_NSF		14
 
int** carrega(char *nomearq, int *t){
	int **matriz, tam, i, j, aux;

	FILE *fp = fopen(nomearq, "r+");
	if(!fp){
		printf("\nNão foi possível abrir o arquivo.\n");
		return NULL;
	}

	fscanf(fp, "%d", &tam);
	*t = tam;
	matriz = (int**) malloc(tam*sizeof(int*));
	if(!matriz){
		printf("\nNão foi possível alocar memória.\n");
		return NULL;
	}
	for(i = 0; i < tam; i++){
		matriz[i] = (int*) malloc(tam*sizeof(int));
		if(!matriz[i]){
			printf("\nNão foi possível alocar memória.\n");
			return NULL;
		}
	}
	
	for(j = 0; j < tam; j++){
		for(i = 0; i < tam; i++){
			if(feof(fp)) return NULL;
			fscanf(fp, "%d,", &aux);
			matriz[i][j] = (aux == -1) ? INF : aux;
		}
	}

	fclose(fp);

	return matriz;
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

int dijkstra(int **matriz, int primNo, int fimNo, int k, int tam, int boolNom){
 
    int custo[tam][tam], visited[tam], dist[tam], antNo[tam];
	int i, j, cont, proxNo, minDist;
    
    for(i = 0; i < tam; i++){
        for(j = 0; j < tam; j++){
            if(matriz[i][j] == 0) custo[i][j] = INF;
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
		if(boolNom) nomeia(j);
		else printf("%d", j);
    }
	printf("<-");
    if(boolNom) nomeia(j);
	else printf("%d", j);
	printf("\n");

	matriz[antNo[fimNo]][fimNo] = matriz[fimNo][antNo[fimNo]] = INF;

    return EXIT_SUCCESS;
}
 
int main(int argc, char *argv[]) {
	int k, PRIM, FIM, KPATH, **matriz = NULL, i, j, tam, boolNom = 1;
	int NSF[TAM_NSF][TAM_NSF] = {
		{0,420,606,INF,INF,INF,INF,1000,INF,INF,INF,INF,INF,INF},
		{420,0,242,318,INF,INF,INF,INF,INF,INF,INF,INF,INF,INF},
		{606,242,0,INF,INF,723,INF,INF,INF,INF,INF,INF,INF,INF},
		{INF,318,INF,0,210,INF,INF,INF,INF,INF,846,INF,INF,INF},
		{INF,INF,INF,210,0,500,236,INF,INF,INF,INF,INF,INF,INF},
		{INF,INF,723,INF,500,0,INF,INF,433,INF,INF,INF,INF,717},
		{INF,INF,INF,INF,236,INF,0,277,INF,INF,INF,INF,INF,INF},
		{1000,INF,INF,INF,INF,INF,277,0,INF,269,INF,INF,INF,INF},
		{INF,INF,INF,INF,INF,433,INF,INF,0,326,INF,INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,INF,269,326,0,INF,142,164,INF},
		{INF,INF,INF,846,INF,INF,INF,INF,INF,INF,0,INF,314,INF},
		{INF,INF,INF,INF,INF,INF,INF,INF,INF,142,INF,0,INF,158},
		{INF,INF,INF,INF,INF,INF,INF,INF,INF,164,314,INF,0,INF},
		{INF,INF,INF,INF,INF,717,INF,INF,INF,INF,INF,158,INF,0}
	};

	if(argc > 1) matriz = carrega(argv[1], &tam);
	if(matriz != NULL){
		boolNom = 0;
		printf("\nInsira o número da cidade de ORIGEM [0-%d]: ", tam-1);
		scanf("%d", &PRIM);
		printf("\nInsira o número da cidade de DESTINO [0-%d]: ", tam-1);
		scanf("%d", &FIM);
	} else{
		if(argc > 1) printf("\nErro no arquivo de entrada.\n");
		tam = TAM_NSF;
		matriz = (int**) malloc(tam*sizeof(int*));
		if(!matriz){
			printf("\nNão foi possível alocar memória.\n");
			return EXIT_FAILURE;
		}
		for(i = 0; i < tam; i++){
			matriz[i] = (int*) malloc(tam*sizeof(int));
			if(!matriz[i]){
				printf("\nNão foi possível alocar memória.\n");
				return EXIT_FAILURE;
			}
		}
		for(j = 0; j < tam; j++){
			for(i = 0; i < tam; i++){
				matriz[i][j] = NSF[i][j];
			}
		}
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
	}

    printf("\nInsira o número de caminhos que deseja: ");
    scanf("%d", &KPATH);

	/*printf("\nNo inicial = #%d\nNo final = #%d\nK vezes = %d\n", PRIM, FIM, KPATH);*/
	if(PRIM == FIM){
		printf("\nDistância = 0\nNó final e de início são o mesmo: ");
		nomeia(PRIM);
		printf("\n");
		return EXIT_SUCCESS;
	}

	for(k = 1; k <= KPATH; k++)
		if(dijkstra(matriz, PRIM, FIM, k, tam, boolNom) == EXIT_FAILURE) break;

    return EXIT_SUCCESS;

}
