#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================
 * Estrutura do Grafo para Caminho/Ciclo Hamiltoniano
 * Matriz de Adjacência para verificação O(1) de arestas
 * ======================================================== */
typedef struct {
    int num_vertices;
    int **matriz;
} GrafoHamiltoniano;

/* Funções do Grafo */
GrafoHamiltoniano* criar_grafo_h(int num_vertices);
void adicionar_aresta_h(GrafoHamiltoniano *g, int u, int v);
void liberar_grafo_h(GrafoHamiltoniano *g);

/* Funções Obrigatórias de Busca */
int encontrar_caminho_hamiltoniano_bt(const GrafoHamiltoniano *g, int *caminho);
int encontrar_ciclo_hamiltoniano_bt(const GrafoHamiltoniano *g, int *ciclo);

#endif // HAMILTONIANO_H