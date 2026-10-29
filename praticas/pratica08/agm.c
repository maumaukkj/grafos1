#include "agm.h"

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

static void adicionar_aresta_direcionada(GrafoListaPonderado *g, int u, int v, int peso) {
    NoAdj *novo = (NoAdj *)malloc(sizeof(NoAdj));
    if (novo == NULL) return;

    novo->vertice = v;
    novo->peso = peso;
    novo->prox = g->lista_adj[u];
    g->lista_adj[u] = novo;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL || u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices) {
        return;
    }

    // Grafo não-orientado para Árvore Geradora Mínima
    adicionar_aresta_direcionada(g, u, v, peso);
    adicionar_aresta_direcionada(g, v, u, peso);
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
 * Funções do DSU (Disjoint Set Union / Union-Find)
 * ======================================================== */

DSU* criar_dsu(int n) {
    if (n <= 0) return NULL;

    DSU *d = (DSU *)malloc(sizeof(DSU));
    if (d == NULL) return NULL;

    d->n = n;
    d->pai = (int *)malloc(n * sizeof(int));
    d->rank = (int *)malloc(n * sizeof(int));

    if (d->pai == NULL || d->rank == NULL) {
        if (d->pai) free(d->pai);
        if (d->rank) free(d->rank);
        free(d);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        d->pai[i] = i;  // Cada vértice inicia como seu próprio pai
        d->rank[i] = 0; // Rank inicial 0
    }

    return d;
}

void liberar_dsu(DSU *d) {
    if (d == NULL) return;
    if (d->pai != NULL) free(d->pai);
    if (d->rank != NULL) free(d->rank);
    free(d);
}

/* Find com compressão de caminho */
int find(DSU *d, int x) {
    if (d == NULL || x < 0 || x >= d->n) return -1;

    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]); // Compressão de caminho
    }
    return d->pai[x];
}

/* Unir com união por rank */
int unir(DSU *d, int x, int y) {
    if (d == NULL) return 0;

    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x == -1 || raiz_y == -1) return 0;
    if (raiz_x == raiz_y) {
        return 0; // Já pertencem à mesma componente conexa (formaria ciclo)
    }

    // União por rank: anexa a árvore mais rasa à mais profunda
    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }

    return 1;
}

/* ========================================================
 * Algoritmo de Kruskal: O(E log E)
 * ======================================================== */

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *aresta_a = (const Aresta *)a;
    const Aresta *aresta_b = (const Aresta *)b;
    return (aresta_a->peso - aresta_b->peso);
}

int kruskal(GrafoListaPonderado *g, Aresta *resultado_agm, int *tam_agm) {
    if (g == NULL) return 0;

    int V = g->num_vertices;
    int max_arestas = g->num_arestas;

    // Coleta as arestas não-direcionadas (considerando u < v para não duplicar)
    Aresta *todas_arestas = (Aresta *)malloc(max_arestas * sizeof(Aresta));
    if (todas_arestas == NULL) return 0;

    int total_arestas = 0;
    for (int u = 0; u < V; u++) {
        NoAdj *adj = g->lista_adj[u];
        while (adj != NULL) {
            int v = adj->vertice;
            if (u < v) {
                todas_arestas[total_arestas].u = u;
                todas_arestas[total_arestas].v = v;
                todas_arestas[total_arestas].peso = adj->peso;
                total_arestas++;
            }
            adj = adj->prox;
        }
    }

    // Ordenação das arestas por peso em O(E log E)
    qsort(todas_arestas, total_arestas, sizeof(Aresta), comparar_arestas);

    DSU *dsu = criar_dsu(V);
    if (dsu == NULL) {
        free(todas_arestas);
        return 0;
    }

    int peso_total = 0;
    int arestas_selecionadas = 0;

    for (int i = 0; i < total_arestas; i++) {
        int u = todas_arestas[i].u;
        int v = todas_arestas[i].v;
        int peso = todas_arestas[i].peso;

        // Se u e v estão em conjuntos diferentes, a aresta é segura
        if (unir(dsu, u, v)) {
            if (resultado_agm != NULL) {
                resultado_agm[arestas_selecionadas] = todas_arestas[i];
            }
            peso_total += peso;
            arestas_selecionadas++;

            if (arestas_selecionadas == V - 1) {
                break; // AGM completa
            }
        }
    }

    if (tam_agm != NULL) {
        *tam_agm = arestas_selecionadas;
    }

    liberar_dsu(dsu);
    free(todas_arestas);

    return peso_total;
}

/* ========================================================
 * Implementação do Min-Heap para o Algoritmo de Prim
 * ======================================================== */

MinHeap* criar_min_heap(int capacidade) {
    if (capacidade <= 0) return NULL;

    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (heap == NULL) return NULL;

    heap->capacidade = capacidade;
    heap->tamanho = 0;

    heap->dados = (NoHeap *)malloc(capacidade * sizeof(NoHeap));
    heap->posicao = (int *)malloc(capacidade * sizeof(int));

    if (heap->dados == NULL || heap->posicao == NULL) {
        if (heap->dados) free(heap->dados);
        if (heap->posicao) free(heap->posicao);
        free(heap);
        return NULL;
    }

    for (int i = 0; i < capacidade; i++) {
        heap->posicao[i] = -1;
    }

    return heap;
}

void liberar_min_heap(MinHeap *heap) {
    if (heap == NULL) return;
    if (heap->dados != NULL) free(heap->dados);
    if (heap->posicao != NULL) free(heap->posicao);
    free(heap);
}

static void trocar_nos(MinHeap *heap, int i, int j) {
    NoHeap temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;

    heap->posicao[heap->dados[i].vertice] = i;
    heap->posicao[heap->dados[j].vertice] = j;
}

static void subir_no_heap(MinHeap *heap, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap->dados[i].chave < heap->dados[pai].chave) {
            trocar_nos(heap, i, pai);
            i = pai;
        } else {
            break;
        }
    }
}

static void descer_no_heap(MinHeap *heap, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < heap->tamanho && heap->dados[esq].chave < heap->dados[menor].chave) {
        menor = esq;
    }

    if (dir < heap->tamanho && heap->dados[dir].chave < heap->dados[menor].chave) {
        menor = dir;
    }

    if (menor != i) {
        trocar_nos(heap, i, menor);
        descer_no_heap(heap, menor);
    }
}

static void inserir_min_heap(MinHeap *heap, int vertice, int chave) {
    if (heap == NULL || heap->tamanho >= heap->capacidade) return;

    int i = heap->tamanho;
    heap->dados[i].vertice = vertice;
    heap->dados[i].chave = chave;
    heap->posicao[vertice] = i;
    heap->tamanho++;

    subir_no_heap(heap, i);
}

NoHeap extrair_minimo(MinHeap *heap) {
    NoHeap vazio = {-1, INF};
    if (heap == NULL || heap->tamanho <= 0) return vazio;

    NoHeap raiz = heap->dados[0];
    heap->posicao[raiz.vertice] = -1; // Marcado como fora do heap

    heap->tamanho--;
    if (heap->tamanho > 0) {
        NoHeap ultimo = heap->dados[heap->tamanho];
        heap->dados[0] = ultimo;
        heap->posicao[ultimo.vertice] = 0;
        descer_no_heap(heap, 0);
    }

    return raiz;
}

void diminuir_chave(MinHeap *heap, int vertice, int nova_chave) {
    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) return;

    int i = heap->posicao[vertice];
    if (i == -1 || i >= heap->tamanho) return;

    if (nova_chave >= heap->dados[i].chave) return;

    heap->dados[i].chave = nova_chave;
    subir_no_heap(heap, i);
}

int esta_no_min_heap(MinHeap *heap, int vertice) {
    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) return 0;
    return (heap->posicao[vertice] != -1 && heap->posicao[vertice] < heap->tamanho);
}

int heap_vazio(MinHeap *heap) {
    return (heap == NULL || heap->tamanho == 0);
}

/* ========================================================
 * Algoritmo de Prim: O((V + E) log V)
 * ======================================================== */

int prim(GrafoListaPonderado *g, int origem, int *pai) {
    if (g == NULL || origem < 0 || origem >= g->num_vertices || pai == NULL) {
        return 0;
    }

    int V = g->num_vertices;
    int *chave = (int *)malloc(V * sizeof(int));
    if (chave == NULL) return 0;

    MinHeap *heap = criar_min_heap(V);
    if (heap == NULL) {
        free(chave);
        return 0;
    }

    for (int v = 0; v < V; v++) {
        chave[v] = INF;
        pai[v] = -1;
    }

    chave[origem] = 0;

    for (int v = 0; v < V; v++) {
        inserir_min_heap(heap, v, chave[v]);
    }

    int peso_total = 0;

    while (!heap_vazio(heap)) {
        NoHeap min = extrair_minimo(heap);
        int u = min.vertice;

        if (min.chave == INF) {
            break; // Componente desconectada
        }

        peso_total += min.chave;

        NoAdj *adj = g->lista_adj[u];
        while (adj != NULL) {
            int v = adj->vertice;
            int peso = adj->peso;

            // Atualização de chaves em O(log V)
            if (esta_no_min_heap(heap, v) && peso < chave[v]) {
                chave[v] = peso;
                pai[v] = u;
                diminuir_chave(heap, v, peso);
            }
            adj = adj->prox;
        }
    }

    free(chave);
    liberar_min_heap(heap);

    return peso_total;
}