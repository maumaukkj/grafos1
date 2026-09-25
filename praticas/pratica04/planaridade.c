#include <stdio.h>
#include <stdlib.h>
#include "planaridade.h"

int contar_arestas(GrafoLista *g) {
    int arestas = 0;
    for(int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while(atual != NULL) {
            arestas++;
            atual = atual->prox;
        }
    }
    return arestas / 2; // Grafo não-direcionado conta ida e volta
}

int eh_planar_euler(GrafoLista *g) {
    int m = contar_arestas(g);
    int n = g->n;
    
    // Para grafos pequenos (n <= 2), sempre é planar
    if (n <= 2) return 1;
    
    // Formula de Euler: m <= 3n - 6
    if (m > 3 * n - 6) return 0; // Violou Euler, NÃO é planar
    
    return 1; // Pode ser planar
}

// Heurística baseada nos teoremas de Kuratowski (força bruta simplificada p/ n <= 10)
// Verifica se há vértices suficientes com graus que permitam conter K_5 ou K_{3,3}
int heuristica_kuratowski(GrafoLista *g) {
    if (g->n > 10 || g->n < 5) return 1; // Ignora se for muito pequeno ou fora do escopo

    int v_grau4 = 0; // K_5 precisa de 5 vértices de grau >= 4
    int v_grau3 = 0; // K_{3,3} precisa de 6 vértices de grau >= 3

    for(int i = 0; i < g->n; i++) {
        int grau = 0;
        No *atual = g->adj[i];
        while(atual != NULL) {
            grau++;
            atual = atual->prox;
        }
        if (grau >= 4) v_grau4++;
        if (grau >= 3) v_grau3++;
    }

    if (v_grau4 >= 5) {
        printf("  -> [Kuratowski] Potencial subgrafo K_5 detectado.\n");
        return 0; // Provavelmente não planar
    }
    if (v_grau3 >= 6) {
        printf("  -> [Kuratowski] Potencial subgrafo K_{3,3} detectado.\n");
        return 0; // Provavelmente não planar
    }

    return 1;
}