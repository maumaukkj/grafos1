#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

void imprimir_cores(int *cor, int n, int num_cores) {
    printf("  -> Total de cores utilizadas: %d\n", num_cores);
    for(int i = 0; i < n; i++) {
        printf("     Vertice %d: Cor %d\n", i, cor[i]);
    }
}

int main() {
    printf("=== Pratica 05: Coloracao de Grafos ===\n\n");
    
    // Grafo 1: Ciclo C5 (Não Bipartido, precisa de 3 cores)
    printf("--- Analisando Grafo 1 (Ciclo C5) ---\n");
    GrafoLista *g1 = criar_grafo(5);
    inserir_aresta(g1, 0, 1);
    inserir_aresta(g1, 1, 2);
    inserir_aresta(g1, 2, 3);
    inserir_aresta(g1, 3, 4);
    inserir_aresta(g1, 4, 0);
    
    int cores_gulosa, cores_welsh;
    
    int *cor_g = coloracao_gulosa(g1, &cores_gulosa);
    printf("[Algoritmo Guloso]\n");
    imprimir_cores(cor_g, g1->n, cores_gulosa);
    free(cor_g);
    
    int *cor_w = coloracao_welsh_powell(g1, &cores_welsh);
    printf("\n[Algoritmo Welsh-Powell]\n");
    imprimir_cores(cor_w, g1->n, cores_welsh);
    free(cor_w);
    
    printf("\n  -> Eh bipartido (2-colorivel)? %s\n\n", eh_bipartido(g1) ? "Sim" : "Nao");
    
    // Grafo 2: Estrela (Bipartido, precisa de 2 cores)
    // Nó 0 ligado a 1, 2, 3
    printf("--- Analisando Grafo 2 (Estrela) ---\n");
    GrafoLista *g2 = criar_grafo(4);
    inserir_aresta(g2, 0, 1);
    inserir_aresta(g2, 0, 2);
    inserir_aresta(g2, 0, 3);
    
    int *cor_w2 = coloracao_welsh_powell(g2, &cores_welsh);
    printf("[Algoritmo Welsh-Powell]\n");
    imprimir_cores(cor_w2, g2->n, cores_welsh);
    free(cor_w2);
    
    printf("\n  -> Eh bipartido (2-colorivel)? %s\n", eh_bipartido(g2) ? "Sim" : "Nao");
    
    liberar_grafo(g1);
    liberar_grafo(g2);
    return 0;
}