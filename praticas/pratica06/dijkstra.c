#include "dijkstra.h"

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

void adicionar_aresta_bidirecional(GrafoListaPonderado *g, int u, int v, int peso) {
    adicionar_aresta(g, u, v, peso);
    adicionar_aresta(g, v, u, peso);
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
 * Implementação do Min-Heap
 * ======================================================== */

MinHeap* criar_min_heap(int capacidade) {
    if (capacidade <= 0) return NULL;

    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (heap == NULL) return NULL;

    heap->capacidade = capacidade;
    heap->tamanho = 0;

    heap->dados = (NoHeap *)malloc(capacidade * sizeof(NoHeap));
    if (heap->dados == NULL) {
        free(heap);
        return NULL;
    }

    heap->posicao = (int *)malloc(capacidade * sizeof(int));
    if (heap->posicao == NULL) {
        free(heap->dados);
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

void trocar(MinHeap *heap, int i, int j) {
    NoHeap temp = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temp;

    // Atualiza o mapeamento vertice -> indice no heap
    heap->posicao[heap->dados[i].vertice] = i;
    heap->posicao[heap->dados[j].vertice] = j;
}

void subir_no_heap(MinHeap *heap, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap->dados[i].distancia < heap->dados[pai].distancia) {
            trocar(heap, i, pai);
            i = pai;
        } else {
            break;
        }
    }
}

void descer_no_heap(MinHeap *heap, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < heap->tamanho && heap->dados[esq].distancia < heap->dados[menor].distancia) {
        menor = esq;
    }

    if (dir < heap->tamanho && heap->dados[dir].distancia < heap->dados[menor].distancia) {
        menor = dir;
    }

    if (menor != i) {
        trocar(heap, i, menor);
        descer_no_heap(heap, menor);
    }
}

void inserir_min_heap(MinHeap *heap, int vertice, int distancia) {
    if (heap == NULL || heap->tamanho >= heap->capacidade) {
        return;
    }

    int i = heap->tamanho;
    heap->dados[i].vertice = vertice;
    heap->dados[i].distancia = distancia;
    heap->posicao[vertice] = i;
    heap->tamanho++;

    subir_no_heap(heap, i);
}

NoHeap extrair_minimo(MinHeap *heap) {
    NoHeap vazio = {-1, INF};
    if (heap == NULL || heap->tamanho <= 0) {
        return vazio;
    }

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

void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia) {
    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) {
        return;
    }

    int i = heap->posicao[vertice];
    if (i == -1 || i >= heap->tamanho) {
        return;
    }

    // Só atualiza se a nova distância for estritamente menor
    if (nova_distancia >= heap->dados[i].distancia) {
        return;
    }

    heap->dados[i].distancia = nova_distancia;
    subir_no_heap(heap, i); // Executa em O(log V) graças ao índice direto em posicao[]
}

int esta_no_min_heap(MinHeap *heap, int vertice) {
    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) {
        return 0;
    }
    return (heap->posicao[vertice] != -1 && heap->posicao[vertice] < heap->tamanho);
}

int heap_vazio(MinHeap *heap) {
    return (heap == NULL || heap->tamanho == 0);
}

/* Aliases */
void insert(MinHeap *heap, int vertice, int distancia) {
    inserir_min_heap(heap, vertice, distancia);
}

NoHeap extract_min(MinHeap *heap) {
    return extrair_minimo(heap);
}

void decrease_key(MinHeap *heap, int vertice, int nova_distancia) {
    diminuir_chave(heap, vertice, nova_distancia);
}

/* ========================================================
 * Algoritmo de Dijkstra
 * ======================================================== */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || origem < 0 || origem >= g->num_vertices || dist == NULL || pred == NULL) {
        return;
    }

    int V = g->num_vertices;
    MinHeap *heap = criar_min_heap(V);
    if (heap == NULL) return;

    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }

    dist[origem] = 0;

    // Inicializa o heap com todos os vértices
    for (int v = 0; v < V; v++) {
        inserir_min_heap(heap, v, dist[v]);
    }

    while (!heap_vazio(heap)) {
        NoHeap min = extrair_minimo(heap);
        int u = min.vertice;

        // Se a menor distância for infinita, os vértices restantes são inalcançáveis
        if (min.distancia == INF) {
            break;
        }

        // Percorre os vizinhos de u
        NoAdj *adj = g->lista_adj[u];
        while (adj != NULL) {
            int v = adj->vertice;
            int peso = adj->peso;

            // Relaxamento da aresta (u, v) com custo não-negativo
            if (esta_no_min_heap(heap, v) && dist[u] != INF && (dist[u] + peso < dist[v])) {
                dist[v] = dist[u] + peso;
                pred[v] = u;
                diminuir_chave(heap, v, dist[v]);
            }
            adj = adj->prox;
        }
    }

    liberar_min_heap(heap);
}

/* ========================================================
 * Reconstrução do Caminho Mínimo
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