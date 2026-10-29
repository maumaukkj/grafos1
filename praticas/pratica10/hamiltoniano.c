#include "hamiltoniano.h"

GrafoHamiltoniano* criar_grafo_h(int num_vertices) {
    if (num_vertices <= 0) return NULL;

    GrafoHamiltoniano *g = (GrafoHamiltoniano *)malloc(sizeof(GrafoHamiltoniano));
    if (g == NULL) return NULL;

    g->num_vertices = num_vertices;
    g->matriz = (int **)malloc(num_vertices * sizeof(int *));
    if (g->matriz == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->matriz[i] = (int *)calloc(num_vertices, sizeof(int));
        if (g->matriz[i] == NULL) {
            for (int j = 0; j < i; j++) free(g->matriz[j]);
            free(g->matriz);
            free(g);
            return NULL;
        }
    }

    return g;
}

void adicionar_aresta_h(GrafoHamiltoniano *g, int u, int v) {
    if (g == NULL || u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices || u == v) {
        return;
    }
    g->matriz[u][v] = 1;
    g->matriz[v][u] = 1;
}

void liberar_grafo_h(GrafoHamiltoniano *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->num_vertices; i++) {
        free(g->matriz[i]);
    }
    free(g->matriz);
    free(g);
}

/* ========================================================
 * Backtracking para Caminho Hamiltoniano
 * ======================================================== */

static int bt_caminho(const GrafoHamiltoniano *g, int *caminho, int pos, int *visitado) {
    if (pos == g->num_vertices) {
        return 1; // Todos os vértices foram visitados
    }

    int u = caminho[pos - 1];
    for (int v = 0; v < g->num_vertices; v++) {
        if (g->matriz[u][v] && !visitado[v]) {
            caminho[pos] = v;
            visitado[v] = 1;

            if (bt_caminho(g, caminho, pos + 1, visitado)) {
                return 1;
            }

            visitado[v] = 0; // Backtracking
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const GrafoHamiltoniano *g, int *caminho) {
    if (g == NULL || caminho == NULL || g->num_vertices <= 0) return 0;

    int n = g->num_vertices;
    int *visitado = (int *)calloc(n, sizeof(int));
    if (visitado == NULL) return 0;

    // Testa cada vértice como possível início do caminho
    for (int inicio = 0; inicio < n; inicio++) {
        caminho[0] = inicio;
        visitado[inicio] = 1;

        if (bt_caminho(g, caminho, 1, visitado)) {
            free(visitado);
            return 1;
        }

        visitado[inicio] = 0;
    }

    free(visitado);
    return 0;
}

/* ========================================================
 * Backtracking para Ciclo Hamiltoniano
 * ======================================================== */

static int bt_ciclo(const GrafoHamiltoniano *g, int *ciclo, int pos, int *visitado) {
    if (pos == g->num_vertices) {
        // Verifica se há aresta conectando o último vértice de volta ao primeiro
        if (g->matriz[ciclo[pos - 1]][ciclo[0]]) {
            ciclo[pos] = ciclo[0]; // Fecha o ciclo
            return 1;
        }
        return 0;
    }

    int u = ciclo[pos - 1];
    for (int v = 0; v < g->num_vertices; v++) {
        if (g->matriz[u][v] && !visitado[v]) {
            ciclo[pos] = v;
            visitado[v] = 1;

            if (bt_ciclo(g, ciclo, pos + 1, visitado)) {
                return 1;
            }

            visitado[v] = 0; // Backtracking
        }
    }
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(const GrafoHamiltoniano *g, int *ciclo) {
    if (g == NULL || ciclo == NULL || g->num_vertices <= 0) return 0;

    int n = g->num_vertices;
    int *visitado = (int *)calloc(n, sizeof(int));
    if (visitado == NULL) return 0;

    // Fixa o vértice 0 como origem por simetria cíclica
    ciclo[0] = 0;
    visitado[0] = 1;

    int encontrou = bt_ciclo(g, ciclo, 1, visitado);

    free(visitado);
    return encontrou;
}