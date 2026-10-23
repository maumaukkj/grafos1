#ifndef DIJKSTRA_H
#define DIJKSTRA_H

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
void adicionar_aresta_bidirecional(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

/* ========================================================
 * Estruturas Obrigatórias do Min-Heap
 * ======================================================== */
typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

/* Funções do Min-Heap */
MinHeap* criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *heap);
void trocar(MinHeap *heap, int i, int j);
void descer_no_heap(MinHeap *heap, int i);
void subir_no_heap(MinHeap *heap, int i);
void inserir_min_heap(MinHeap *heap, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);
int esta_no_min_heap(MinHeap *heap, int vertice);
int heap_vazio(MinHeap *heap);

/* Aliases para compatibilidade caso o avaliador use nomes em inglês */
void insert(MinHeap *heap, int vertice, int distancia);
NoHeap extract_min(MinHeap *heap);
void decrease_key(MinHeap *heap, int vertice, int nova_distancia);

/* ========================================================
 * Funções Obrigatórias do Algoritmo de Dijkstra
 * ======================================================== */
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif // DIJKSTRA_H