#include <stdio.h>
#include "conectividade.h"
#include "planaridade.h"

int main() {
    printf("=== Pratica 04: Conectividade e Planaridade ===\n\n");

    // Grafo 1: Dois triângulos conectados por uma ponte (3)---(4)
    printf("--- Analisando Grafo 1 (Com Ponte e Articulacoes) ---\n");
    GrafoLista *g1 = criar_grafo(6);
    inserir_aresta(g1, 0, 1); inserir_aresta(g1, 1, 2); inserir_aresta(g1, 2, 0);
    inserir_aresta(g1, 3, 4); 
    inserir_aresta(g1, 4, 5); inserir_aresta(g1, 5, 3);
    inserir_aresta(g1, 2, 3); // Ponte

    encontrar_articulacoes_e_pontes(g1);
    printf("  -> Planar por Euler? %s\n", eh_planar_euler(g1) ? "Sim" : "Nao");

    // Grafo 2: K_5 (Grafo completo com 5 vértices - Nao Planar)
    printf("\n--- Analisando Grafo 2 (K_5) ---\n");
    GrafoLista *g2 = criar_grafo(5);
    for(int i = 0; i < 5; i++) {
        for(int j = i + 1; j < 5; j++) {
            inserir_aresta(g2, i, j);
        }
    }
    
    encontrar_articulacoes_e_pontes(g2);
    printf("  -> Planar por Euler (m <= 3n-6)? %s\n", eh_planar_euler(g2) ? "Sim" : "Nao");
    printf("  -> Passou na heuristica de Kuratowski? %s\n", heuristica_kuratowski(g2) ? "Sim" : "Nao (Possui K_5 ou K_3,3)");

    liberar_grafo(g1);
    liberar_grafo(g2);
    return 0;
}