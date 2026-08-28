#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    printf("=== Teste Grafo (Matriz de Adjacencia) ===\n");
    GrafoMatriz *gm = criar_grafo_matriz(5);
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 1, 2);
    
    printf("Grau do vertice 1: %d (Esperado: 2)\n", grau_matriz(gm, 1));
    printf("0 e 1 sao adjacentes? %d (Esperado: 1)\n", sao_adjacentes_matriz(gm, 0, 1));
    
    remover_aresta_matriz(gm, 0, 1);
    printf("0 e 1 adjacentes apos remocao? %d (Esperado: 0)\n", sao_adjacentes_matriz(gm, 0, 1));
    liberar_grafo_matriz(gm);

    printf("\n=== Teste Grafo (Lista de Adjacencia) ===\n");
    GrafoLista *gl = criar_grafo_lista(5);
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 1, 2);
    
    printf("Grau do vertice 1: %d (Esperado: 2)\n", grau_lista(gl, 1));
    printf("0 e 1 sao adjacentes? %d (Esperado: 1)\n", sao_adjacentes_lista(gl, 0, 1));
    
    remover_aresta_lista(gl, 0, 1);
    printf("0 e 1 adjacentes apos remocao? %d (Esperado: 0)\n", sao_adjacentes_lista(gl, 0, 1));
    liberar_grafo_lista(gl);

    return 0;
}