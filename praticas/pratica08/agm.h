#ifndef AGM_H
#define AGM_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

/* ========================================================
 * Estruturas Obrigatórias do Enunciado
 * ======================================================== */

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

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

/* Funções do Grafo */
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

/* ========================================================
 * Estruturas do Min-Heap para o Algoritmo de Prim
 * ======================================================== */

typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

MinHeap* criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_chave);
NoHeap extrair_minimo(MinHeap *heap);
int esta_no_min_heap(MinHeap *heap, int vertice);
int heap_vazio(MinHeap *heap);

/* ========================================================
 * Funções Obrigatórias do DSU (Union-Find)
 * ======================================================== */

DSU* criar_dsu(int n);
void liberar_dsu(DSU *d);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);

/* ========================================================
 * Funções Obrigatórias da Árvore Geradora Mínima (AGM)
 * ======================================================== */

int kruskal(GrafoListaPonderado *g, Aresta *resultado_agm, int *tam_agm);
int prim(GrafoListaPonderado *g, int origem, int *pai);

#endif // AGM_H