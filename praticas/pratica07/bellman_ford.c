#include "bellman_ford.h"

/* ========================================================
 * Funções do Grafo com Lista de Adjacência Ponderada
 * ======================================================== */

GrafoListaPonderado* criar_grafo(int num_vertices) {
    if (num_vertices <= 0) return NULL;

    GrafoListaPonderado *g = (GrafoListaPonderado *)malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) return NULL;

    g->num_vertices = num_vertices;
    g->num_arestas = 0;
    g->lista_adj = (NoAdj **)malloc(num_vertices * sizeof(NoAdj *));
    if (g->lista_adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
    }

    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL || u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices) {
        return;
    }

    NoAdj *novo = (NoAdj *)malloc(sizeof(NoAdj));
    if (novo == NULL) return;

    novo->vertice = v;
    novo->peso = peso;
    novo->prox = g->lista_adj[u];
    g->lista_adj[u] = novo;

    g->num_arestas++;
}

void liberar_grafo(GrafoListaPonderado *g) {
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
    free(g);
}

/* ========================================================
 * Reconstrução do Ciclo de Custo Negativo
 * ======================================================== */

void reconstruir_ciclo_negativo(int inicio, int *pred, int n) {
    if (pred == NULL || n <= 0 || inicio < 0 || inicio >= n) {
        printf("Parametros invalidos para reconstrucao do ciclo negativo.\n");
        return;
    }

    /*
     * 1. Retrocede n vezes pelo pred[] a partir do vertice relaxado.
     * Como o grafo tem n vertices, apos n passos e matematicamente garantido
     * que caimos dentro de um ciclo negativo.
     */
    int c = inicio;
    for (int i = 0; i < n; i++) {
        if (c < 0 || c >= n || pred[c] == -1) {
            break;
        }
        c = pred[c];
    }

    if (c < 0 || c >= n || pred[c] == -1) {
        printf("Nao foi possivel reconstruir o ciclo negativo a partir dos predecessores.\n");
        return;
    }

    /*
     * 2. A partir do vertice 'c' (que pertence ao ciclo),
     * retrocedemos gravando os vertices ate encontrar 'c' novamente.
     */
    int *ciclo = (int *)malloc((n + 2) * sizeof(int));
    if (ciclo == NULL) {
        printf("Erro de alocacao de memoria ao reconstruir ciclo negativo.\n");
        return;
    }

    int tam = 0;
    int atual = c;
    do {
        ciclo[tam++] = atual;
        atual = pred[atual];
    } while (atual != c && atual != -1 && tam <= n);

    if (atual != c) {
        printf("Erro: ciclo nao fechou corretamente.\n");
        free(ciclo);
        return;
    }

    // Adiciona o vertice de fechamento
    ciclo[tam++] = c;

    /*
     * 3. Imprime o ciclo na ordem direta (origem -> destino).
     * Como o array foi preenchido de filho para pai via pred[],
     * a impressao invertida restaura a orientacao correta das arestas.
     */
    printf("Ciclo de custo negativo detectado: ");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", ciclo[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(ciclo);
}

/* ========================================================
 * Reconstrução do Caminho Mínimo (sem ciclo negativo)
 * ======================================================== */

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (pred == NULL) {
        printf("Vetor de predecessores nulo.\n");
        return;
    }

    if (origem == destino) {
        printf("Caminho de %d a %d: %d\n", origem, destino, origem);
        return;
    }

    if (pred[destino] == -1) {
        printf("Nao ha caminho entre o vertice %d e o vertice %d.\n", origem, destino);
        return;
    }

    int capacidade = 32;
    int tam = 0;
    int *caminho = (int *)malloc(capacidade * sizeof(int));
    if (caminho == NULL) {
        printf("Erro de alocacao ao reconstruir caminho.\n");
        return;
    }

    int atual = destino;
    while (atual != -1) {
        if (tam >= capacidade) {
            capacidade *= 2;
            int *temp = (int *)realloc(caminho, capacidade * sizeof(int));
            if (temp == NULL) {
                free(caminho);
                return;
            }
            caminho = temp;
        }
        caminho[tam++] = atual;
        if (atual == origem) break;
        atual = pred[atual];
    }

    if (caminho[tam - 1] != origem) {
        printf("Nao ha caminho entre o vertice %d e o vertice %d.\n", origem, destino);
        free(caminho);
        return;
    }

    printf("Caminho de %d a %d: ", origem, destino);
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(caminho);
}

/* ========================================================
 * Algoritmo de Bellman-Ford
 * Retorna: 1 em caso de sucesso (sem ciclo negativo)
 *          0 se um ciclo de custo negativo for detectado
 * ======================================================== */

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || origem < 0 || origem >= g->num_vertices || dist == NULL || pred == NULL) {
        return 0;
    }

    int n = g->num_vertices;

    for (int v = 0; v < n; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }

    dist[origem] = 0;

    /*
     * Etapa 1: n - 1 iteracoes de relaxacao de todas as arestas
     */
    for (int i = 1; i <= n - 1; i++) {
        int mudou = 0;
        for (int u = 0; u < n; u++) {
            if (dist[u] == INF) continue;

            NoAdj *adj = g->lista_adj[u];
            while (adj != NULL) {
                int v = adj->vertice;
                int peso = adj->peso;

                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pred[v] = u;
                    mudou = 1;
                }
                adj = adj->prox;
            }
        }
        // Se nenhuma distancia foi atualizada nesta rodada, o algoritmo convergiu
        if (!mudou) {
            break;
        }
    }

    /*
     * Etapa 2: n-esima iteracao para deteccao de ciclo de custo negativo
     */
    for (int u = 0; u < n; u++) {
        if (dist[u] == INF) continue;

        NoAdj *adj = g->lista_adj[u];
        while (adj != NULL) {
            int v = adj->vertice;
            int peso = adj->peso;

            // Se ainda for possivel relaxar a aresta, existe ciclo negativo
            if (dist[u] + peso < dist[v]) {
                pred[v] = u;
                reconstruir_ciclo_negativo(v, pred, n);
                return 0; // Ciclo negativo detectado
            }
            adj = adj->prox;
        }
    }

    return 1; // Sucesso, sem ciclos negativos
}