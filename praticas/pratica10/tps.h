#ifndef TPS_H
#define TPS_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "hamiltoniano.h"

#define INF_TSP INT_MAX

/* ========================================================
 * Estrutura do Grafo Ponderado para TSP
 * ======================================================== */
typedef struct {
    int num_vertices;
    int **custo;
} GrafoTSP;

/* Funções de manipulação */
GrafoTSP* criar_grafo_tsp(int num_vertices);
void liberar_grafo_tsp(GrafoTSP *g);
GrafoTSP* gerar_grafo_tsp_aleatorio(int num_vertices, int custo_max);

/* ========================================================
 * Redução: Grafo Não-Ponderado -> TSP
 * Peso 1 para arestas existentes, peso 2 para inexistentes
 * ======================================================== */
GrafoTSP* grafo_para_tsp(const GrafoHamiltoniano *gh);

/* ========================================================
 * Algoritmos para o TSP
 * ======================================================== */
int tsp_forca_bruta(const GrafoTSP *g, int *melhor_circuito);
int tsp_vizinho_mais_proximo(const GrafoTSP *g, int origem, int *circuito);
int tsp_insercao(const GrafoTSP *g, int *circuito);

#endif // TPS_H