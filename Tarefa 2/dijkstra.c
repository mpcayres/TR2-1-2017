#include <stdio.h>

#define INF		9999
#define TAM		14
#define PRIM	0

int NSF[TAM][TAM] = {
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
 
void dijkstra(int primNo){
 
    int custo[TAM][TAM], visited[TAM], dist[TAM], antNo[TAM];
	int i, j, cont, proxNo, minDist;
    
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(NSF[i][j] == 0) custo[i][j] = INF;
            else custo[i][j] = NSF[i][j];
		}
	}
    
    for(i = 0; i < TAM; i++){
		visited[i] = 0;
        dist[i] = custo[primNo][i];
        antNo[i] = primNo;
    }
    
    for(cont = 0, visited[primNo] = 1, dist[primNo] = 0; cont < TAM; cont++){
        minDist = INF;
        
        for(i = 0; i < TAM; i++){
            if(!visited[i] && dist[i] < minDist){
                minDist = dist[i];
                proxNo = i;
            }
        }
        
        for(i = 0, visited[proxNo] = 1; i < TAM; i++){
            if(!visited[i]){
                if((minDist + custo[proxNo][i]) < dist[i]){
                    dist[i] = minDist + custo[proxNo][i];
                    antNo[i] = proxNo;
                }
			}
		}
    }
 
	printf("\nNo de inicio = #%d", primNo);
    for(i = 0; i < TAM; i++){
        if(i != primNo){
            printf("\nDistancia #%d = %d", i, dist[i]);
            for(j = i; j != primNo; j = antNo[j]){
				if(j == i) printf("\n%d", i);
                else printf("<-%d", j);
            }
			printf("<-%d", j);
		}
    }
	printf("\n");

}
 
int main() {
    dijkstra(PRIM);
    
    return 0;

}
