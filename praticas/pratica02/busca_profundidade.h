#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h" // Reaproveita a struct GrafoLista

// Pilha (LIFO)
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

// Funções da Prática
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo_entrada, int *tempo_saida, int *tempo);
int contar_componentes(GrafoLista *g);
int tem_ciclo(GrafoLista *g);

#endif