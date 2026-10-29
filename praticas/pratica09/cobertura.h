#ifndef COBERTURA_H
#define COBERTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========================================================
 * Estruturas do Grafo
 * ======================================================== */

typedef struct {
    int u, v;
} Aresta;

typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int num_vertices;
    int num_arestas;
    int capacidade_arestas;
    Aresta *arestas;
    NoAdj **lista_adj;
} Grafo;

/* Funções de manipulação do Grafo */
Grafo* criar_grafo(int num_vertices);
void adicionar_aresta(Grafo *g, int u, int v);
void liberar_grafo(Grafo *g);
Grafo* gerar_grafo_aleatorio(int num_vertices, double probabilidade);

/* ========================================================
 * Funções Obrigatórias da Atividade
 * ======================================================== */

int eh_cobertura(const Grafo *g, const int *cobertura);
int cobertura_minima_bt(const Grafo *g, int *melhor_cobertura);
int cobertura_gulosa(const Grafo *g, int *cobertura);

#endif // COBERTURA_H