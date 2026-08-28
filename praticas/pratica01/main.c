#include <stdio.h>
#include <assert.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    int n = 5;

    // Testes para Matriz de Adjacência
    GrafoMatriz *gm = criar_grafo_matriz(n);
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 2);

    assert(sao_adjacentes_matriz(gm, 0, 1) == 1);
    assert(sao_adjacentes_matriz(gm, 0, 3) == 0);
    assert(grau_matriz(gm, 0) == 2);

    remover_aresta_matriz(gm, 0, 1);
    assert(sao_adjacentes_matriz(gm, 0, 1) == 0);
    assert(grau_matriz(gm, 0) == 1);

    liberar_grafo_matriz(gm);

    // Testes para Lista de Adjacência
    GrafoLista *gl = criar_grafo_lista(n);
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 2);

    assert(sao_adjacentes_lista(gl, 0, 1) == 1);
    assert(sao_adjacentes_lista(gl, 0, 3) == 0);
    assert(grau_lista(gl, 0) == 2);

    remover_aresta_lista(gl, 0, 1);
    assert(sao_adjacentes_lista(gl, 0, 1) == 0);
    assert(grau_lista(gl, 0) == 1);

    liberar_grafo_lista(gl);

    printf("Todos os testes passaram com sucesso!\n");
    return 0;
}