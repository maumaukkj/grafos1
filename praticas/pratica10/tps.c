#include "tps.h"

/* ========================================================
 * Funções do Grafo TSP
 * ======================================================== */

GrafoTSP* criar_grafo_tsp(int num_vertices) {
    if (num_vertices <= 0) return NULL;

    GrafoTSP *g = (GrafoTSP *)malloc(sizeof(GrafoTSP));
    if (g == NULL) return NULL;

    g->num_vertices = num_vertices;
    g->custo = (int **)malloc(num_vertices * sizeof(int *));
    if (g->custo == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->custo[i] = (int *)calloc(num_vertices, sizeof(int));
        if (g->custo[i] == NULL) {
            for (int j = 0; j < i; j++) free(g->custo[j]);
            free(g->custo);
            free(g);
            return NULL;
        }
    }

    return g;
}

void liberar_grafo_tsp(GrafoTSP *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->num_vertices; i++) {
        free(g->custo[i]);
    }
    free(g->custo);
    free(g);
}

GrafoTSP* gerar_grafo_tsp_aleatorio(int num_vertices, int custo_max) {
    GrafoTSP *g = criar_grafo_tsp(num_vertices);
    if (g == NULL) return NULL;

    for (int i = 0; i < num_vertices; i++) {
        g->custo[i][i] = 0;
        for (int j = i + 1; j < num_vertices; j++) {
            int peso = 1 + rand() % custo_max;
            g->custo[i][j] = peso;
            g->custo[j][i] = peso;
        }
    }

    return g;
}

/* ========================================================
 * Redução: Grafo Não-Ponderado -> TSP
 * Peso 1 para arestas existentes, peso 2 para inexistentes
 * ======================================================== */

GrafoTSP* grafo_para_tsp(const GrafoHamiltoniano *gh) {
    if (gh == NULL) return NULL;

    int n = gh->num_vertices;
    GrafoTSP *gt = criar_grafo_tsp(n);
    if (gt == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                gt->custo[i][j] = 0;
            } else if (gh->matriz[i][j] == 1) {
                gt->custo[i][j] = 1; // Aresta existente
            } else {
                gt->custo[i][j] = 2; // Aresta inexistente
            }
        }
    }

    return gt;
}

/* ========================================================
 * TSP: Força Bruta com Permutações ((n-1)!) e Origem Fixa
 * ======================================================== */

static void bt_tsp(const GrafoTSP *g, int *circuito_atual, int pos, int custo_atual,
                   int *visitado, int *melhor_circuito, int *melhor_custo) {
    int n = g->num_vertices;

    // Poda por limite (Branch-and-Bound): se o custo acumulado já é >= melhor, aborta
    if (custo_atual >= *melhor_custo) {
        return;
    }

    if (pos == n) {
        // Custo total fechando o ciclo de volta à origem (vértice 0)
        int custo_total = custo_atual + g->custo[circuito_atual[n - 1]][0];
        if (custo_total < *melhor_custo) {
            *melhor_custo = custo_total;
            for (int i = 0; i < n; i++) {
                melhor_circuito[i] = circuito_atual[i];
            }
            melhor_circuito[n] = 0;
        }
        return;
    }

    int anterior = circuito_atual[pos - 1];

    for (int v = 1; v < n; v++) {
        if (!visitado[v]) {
            int novo_custo = custo_atual + g->custo[anterior][v];
            if (novo_custo < *melhor_custo) {
                visitado[v] = 1;
                circuito_atual[pos] = v;

                bt_tsp(g, circuito_atual, pos + 1, novo_custo, visitado, melhor_circuito, melhor_custo);

                visitado[v] = 0; // Backtracking
            }
        }
    }
}

int tsp_forca_bruta(const GrafoTSP *g, int *melhor_circuito) {
    if (g == NULL || melhor_circuito == NULL || g->num_vertices <= 0) return 0;

    int n = g->num_vertices;
    if (n == 1) {
        melhor_circuito[0] = 0;
        melhor_circuito[1] = 0;
        return 0;
    }

    int *circuito_atual = (int *)malloc((n + 1) * sizeof(int));
    int *visitado = (int *)calloc(n, sizeof(int));

    if (circuito_atual == NULL || visitado == NULL) {
        if (circuito_atual) free(circuito_atual);
        if (visitado) free(visitado);
        return 0;
    }

    // Fixa o vértice 0 como origem
    circuito_atual[0] = 0;
    visitado[0] = 1;

    // Utiliza a heurística do vizinho mais próximo como limite superior inicial
    int melhor_custo = tsp_vizinho_mais_proximo(g, 0, melhor_circuito);

    bt_tsp(g, circuito_atual, 1, 0, visitado, melhor_circuito, &melhor_custo);

    free(circuito_atual);
    free(visitado);

    return melhor_custo;
}

/* ========================================================
 * Heurística 1: Vizinho Mais Próximo (Nearest Neighbor)
 * ======================================================== */

int tsp_vizinho_mais_proximo(const GrafoTSP *g, int origem, int *circuito) {
    if (g == NULL || circuito == NULL || origem < 0 || origem >= g->num_vertices) {
        return 0;
    }

    int n = g->num_vertices;
    int *visitado = (int *)calloc(n, sizeof(int));
    if (visitado == NULL) return 0;

    circuito[0] = origem;
    visitado[origem] = 1;

    int custo_total = 0;
    int atual = origem;

    for (int passo = 1; passo < n; passo++) {
        int proximo = -1;
        int menor_dist = INF_TSP;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && g->custo[atual][v] < menor_dist) {
                menor_dist = g->custo[atual][v];
                proximo = v;
            }
        }

        if (proximo == -1) break;

        circuito[passo] = proximo;
        visitado[proximo] = 1;
        custo_total += menor_dist;
        atual = proximo;
    }

    // Fecha o ciclo retornando à origem
    circuito[n] = origem;
    custo_total += g->custo[atual][origem];

    free(visitado);
    return custo_total;
}

/* ========================================================
 * Heurística 2: Inserção do Mais Próximo (Nearest Insertion)
 * ======================================================== */

int tsp_insercao(const GrafoTSP *g, int *circuito) {
    if (g == NULL || circuito == NULL || g->num_vertices <= 0) return 0;

    int n = g->num_vertices;
    if (n == 1) {
        circuito[0] = 0;
        circuito[1] = 0;
        return 0;
    }

    int *no_subtour = (int *)calloc(n, sizeof(int));
    int *tour = (int *)malloc((n + 1) * sizeof(int));
    if (no_subtour == NULL || tour == NULL) {
        if (no_subtour) free(no_subtour);
        if (tour) free(tour);
        return 0;
    }

    // Inicialização: Subtour com 0 e o vértice mais próximo de 0
    int v_prox = 1;
    int menor_d = g->custo[0][1];
    for (int j = 2; j < n; j++) {
        if (g->custo[0][j] < menor_d) {
            menor_d = g->custo[0][j];
            v_prox = j;
        }
    }

    tour[0] = 0;
    tour[1] = v_prox;
    tour[2] = 0;
    int tam_tour = 2;

    no_subtour[0] = 1;
    no_subtour[v_prox] = 1;

    // Insere os demais vértices
    while (tam_tour < n) {
        // Seleção: Vértice k fora do tour com menor distância a algum vértice do tour
        int melhor_k = -1;
        int menor_dist_ao_tour = INF_TSP;

        for (int k = 0; k < n; k++) {
            if (!no_subtour[k]) {
                for (int i = 0; i < tam_tour; i++) {
                    int no_tour = tour[i];
                    if (g->custo[k][no_tour] < menor_dist_ao_tour) {
                        menor_dist_ao_tour = g->custo[k][no_tour];
                        melhor_k = k;
                    }
                }
            }
        }

        if (melhor_k == -1) break;

        // Inserção: Encontra a aresta (tour[i], tour[i+1]) com menor custo adicional
        int melhor_pos = 0;
        int menor_aumento = INF_TSP;

        for (int i = 0; i < tam_tour; i++) {
            int u = tour[i];
            int v = tour[i + 1];
            int aumento = g->custo[u][melhor_k] + g->custo[melhor_k][v] - g->custo[u][v];
            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i;
            }
        }

        // Insere melhor_k na posição melhor_pos + 1
        for (int i = tam_tour + 1; i > melhor_pos + 1; i--) {
            tour[i] = tour[i - 1];
        }
        tour[melhor_pos + 1] = melhor_k;
        tam_tour++;
        no_subtour[melhor_k] = 1;
    }

    // Calcula o custo total e copia para o circuito de saída
    int custo_total = 0;
    for (int i = 0; i < n; i++) {
        circuito[i] = tour[i];
        custo_total += g->custo[tour[i]][tour[i + 1]];
    }
    circuito[n] = tour[n];

    free(no_subtour);
    free(tour);

    return custo_total;
}