#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

int main() {
    printf("=== Testes de Buscas e Algoritmos ===\n");
    
    // Grafo base: 0-1, 1-2, 3-4 
    // Tem 2 componentes conexos, é bipartido e não tem ciclo
    GrafoLista *g = criar_grafo(5);
    inserir_aresta(g, 0, 1);
    inserir_aresta(g, 1, 2);
    inserir_aresta(g, 3, 4);
    
    int dist[5], pred[5];
    bfs(g, 0, dist, pred);
    printf("BFS a partir do 0 -> Distancia de 0 a 2: %d (Esperado: 2)\n", dist[2]);
    printf("Componentes conexos: %d (Esperado: 2)\n", contar_componentes(g));
    printf("Tem ciclo? %s (Esperado: Nao)\n", tem_ciclo(g) ? "Sim" : "Nao");
    printf("Eh bipartido? %s (Esperado: Sim)\n", eh_bipartido(g) ? "Sim" : "Nao");
    
    // Inserindo um ciclo ímpar: 0-1-2-0
    printf("\nInserindo aresta (2,0)...\n");
    inserir_aresta(g, 2, 0);
    
    printf("Tem ciclo agora? %s (Esperado: Sim)\n", tem_ciclo(g) ? "Sim" : "Nao");
    printf("Eh bipartido agora? %s (Esperado: Nao)\n", eh_bipartido(g) ? "Sim" : "Nao");
    
    liberar_grafo(g);
    return 0;
}