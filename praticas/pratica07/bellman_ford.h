#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

/* ========================================================
 * Estruturas do Grafo com Lista de Adjacência Ponderada
 * ======================================================== */
typedef struct NoAdj {
    int vertice;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int num_vertices;
    int num_arestas;
    NoAdj **lista_adj;
} GrafoListaPonderado;

/* Funções utilitárias do Grafo */
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

/* ========================================================
 * Funções Obrigatórias do Algoritmo de Bellman-Ford
 * ======================================================== */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(int inicio, int *pred, int n);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif // BELLMAN_FORD_H