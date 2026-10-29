#include "cobertura.h"

/* ========================================================
 * Funções do Grafo
 * ======================================================== */

Grafo* criar_grafo(int num_vertices) {
    if (num_vertices <= 0) return NULL;

    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    if (g == NULL) return NULL;

    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->capacidade_arestas = 16;

    g->arestas = (Aresta *)malloc(g->capacidade_arestas * sizeof(Aresta));
    g->lista_adj = (NoAdj **)malloc(num_vertices * sizeof(NoAdj *));

    if (g->arestas == NULL || g->lista_adj == NULL) {
        if (g->arestas) free(g->arestas);
        if (g->lista_adj) free(g->lista_adj);
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
    }

    return g;
}

static void adicionar_na_lista(Grafo *g, int u, int v) {
    NoAdj *novo = (NoAdj *)malloc(sizeof(NoAdj));
    if (novo == NULL) return;
    novo->vertice = v;
    novo->prox = g->lista_adj[u];
    g->lista_adj[u] = novo;
}

void adicionar_aresta(Grafo *g, int u, int v) {
    if (g == NULL || u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices || u == v) {
        return;
    }

    // Grafo não-orientado na lista de adjacência
    adicionar_na_lista(g, u, v);
    adicionar_na_lista(g, v, u);

    // Armazena no array linear de arestas para validação rápida O(|E|)
    if (g->num_arestas >= g->capacidade_arestas) {
        g->capacidade_arestas *= 2;
        Aresta *temp = (Aresta *)realloc(g->arestas, g->capacidade_arestas * sizeof(Aresta));
        if (temp == NULL) return;
        g->arestas = temp;
    }

    g->arestas[g->num_arestas].u = (u < v) ? u : v;
    g->arestas[g->num_arestas].v = (u < v) ? v : u;
    g->num_arestas++;
}

void liberar_grafo(Grafo *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->num_vertices; i++) {
        NoAdj *atual = g->lista_adj[i];
        while (atual != NULL) {
            NoAdj *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }

    free(g->lista_adj);
    free(g->arestas);
    free(g);
}

Grafo* gerar_grafo_aleatorio(int num_vertices, double probabilidade) {
    Grafo *g = criar_grafo(num_vertices);
    if (g == NULL) return NULL;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r < probabilidade) {
                adicionar_aresta(g, i, j);
            }
        }
    }

    return g;
}

/* ========================================================
 * Verificação de Cobertura de Vértices
 * ======================================================== */

int eh_cobertura(const Grafo *g, const int *cobertura) {
    if (g == NULL || cobertura == NULL) return 0;

    for (int i = 0; i < g->num_arestas; i++) {
        int u = g->arestas[i].u;
        int v = g->arestas[i].v;

        // Se nenhum dos dois vértices da aresta estiver na cobertura, falhou
        if (!cobertura[u] && !cobertura[v]) {
            return 0;
        }
    }
    return 1;
}

/* ========================================================
 * Heurística Gulosa (Maior Grau Restante)
 * ======================================================== */

int cobertura_gulosa(const Grafo *g, int *cobertura) {
    if (g == NULL || cobertura == NULL) return 0;

    int n = g->num_vertices;
    int m = g->num_arestas;

    for (int i = 0; i < n; i++) {
        cobertura[i] = 0;
    }

    if (m == 0) return 0;

    int *arestas_cobertas = (int *)calloc(m, sizeof(int));
    if (arestas_cobertas == NULL) return 0;

    int total_cobertas = 0;
    int tamanho = 0;

    while (total_cobertas < m) {
        // Grau restante: apenas arestas que ainda não foram cobertas
        int *grau = (int *)calloc(n, sizeof(int));
        if (grau == NULL) {
            free(arestas_cobertas);
            return tamanho;
        }

        for (int i = 0; i < m; i++) {
            if (!arestas_cobertas[i]) {
                grau[g->arestas[i].u]++;
                grau[g->arestas[i].v]++;
            }
        }

        // Escolhe o vértice de maior grau restante
        int melhor_vertice = -1;
        int maior_grau = 0;
        for (int v = 0; v < n; v++) {
            if (!cobertura[v] && grau[v] > maior_grau) {
                maior_grau = grau[v];
                melhor_vertice = v;
            }
        }

        free(grau);

        if (melhor_vertice == -1 || maior_grau == 0) {
            break;
        }

        // Adiciona à cobertura
        cobertura[melhor_vertice] = 1;
        tamanho++;

        // Marca todas as arestas incidentes como cobertas
        for (int i = 0; i < m; i++) {
            if (!arestas_cobertas[i] &&
                (g->arestas[i].u == melhor_vertice || g->arestas[i].v == melhor_vertice)) {
                arestas_cobertas[i] = 1;
                total_cobertas++;
            }
        }
    }

    free(arestas_cobertas);
    return tamanho;
}

/* ========================================================
 * Força Bruta com Backtracking e Poda por Limite
 * ======================================================== */

static void bt_helper(const Grafo *g, int idx, int *cobertura_atual, int tamanho_atual,
                      int *melhor_cobertura, int *melhor_tamanho) {
    // Poda por limite: se o tamanho atual já alcançou o melhor encontrado, aborta este ramo
    if (tamanho_atual >= *melhor_tamanho) {
        return;
    }

    // Se todos os vértices foram processados
    if (idx == g->num_vertices) {
        if (eh_cobertura(g, cobertura_atual)) {
            if (tamanho_atual < *melhor_tamanho) {
                *melhor_tamanho = tamanho_atual;
                for (int i = 0; i < g->num_vertices; i++) {
                    melhor_cobertura[i] = cobertura_atual[i];
                }
            }
        }
        return;
    }

    // Poda de viabilidade para exclusão de idx:
    // Se não incluirmos idx (cobertura_atual[idx] = 0), qualquer vizinho v < idx
    // precisa obrigatoriamente estar na cobertura para não deixar a aresta (v, idx) descoberta
    int pode_excluir = 1;
    NoAdj *adj = g->lista_adj[idx];
    while (adj != NULL) {
        int v = adj->vertice;
        if (v < idx && !cobertura_atual[v]) {
            pode_excluir = 0;
            break;
        }
        adj = adj->prox;
    }

    // Ramo 1: Incluir vértice idx na cobertura
    cobertura_atual[idx] = 1;
    bt_helper(g, idx + 1, cobertura_atual, tamanho_atual + 1, melhor_cobertura, melhor_tamanho);

    // Ramo 2: Não incluir vértice idx (somente se não quebrar arestas com vértices passados)
    if (pode_excluir) {
        cobertura_atual[idx] = 0;
        bt_helper(g, idx + 1, cobertura_atual, tamanho_atual, melhor_cobertura, melhor_tamanho);
    }

    // Restaura estado para o backtracking
    cobertura_atual[idx] = 0;
}

int cobertura_minima_bt(const Grafo *g, int *melhor_cobertura) {
    if (g == NULL || melhor_cobertura == NULL) return 0;

    int n = g->num_vertices;
    int *cobertura_atual = (int *)calloc(n, sizeof(int));
    if (cobertura_atual == NULL) return 0;

    // A solução gulosa serve como limite superior inicial (Upper Bound) para podas imediatas
    int melhor_tamanho = cobertura_gulosa(g, melhor_cobertura);

    bt_helper(g, 0, cobertura_atual, 0, melhor_cobertura, &melhor_tamanho);

    free(cobertura_atual);
    return melhor_tamanho;
}