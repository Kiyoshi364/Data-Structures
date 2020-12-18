#ifndef __GRAFO__
#define __GRAFO__

#include "clist.c"

typedef struct _grafo {
	int V, A;
	clist **adjs;
} grafo;

grafo* newGrafo(const int vertices) {
	grafo *g = malloc(sizeof(*g));
	if ( !g ) {
		puts("newGrafo: Out of Memory (grafo).");
		exit(1);
	}

	g->V = vertices;
	g->A = 0;
	g->adjs = malloc(sizeof(*(g->adjs))*vertices);
	if ( !(g->adjs) ) {
		puts("newGrafo: Out of Memory (g->adjs).");
		exit(1);
	}

	for (int i = 0; i < vertices; i++) {
		g->adjs[i] = newClist();
	}

	return g;
}

void freeGrafo(grafo *g) {
	for (int i = 0; i < g->V; i++) {
		freeClist(g->adjs[i]);
	}

	free(g);
}

void addAresta(grafo *g, int de, int para) {
	pushInClist(g->adjs[de], para);
	g->A += 1;
}

int remAresta(grafo *g, int de, int para) {
	int err = popOutClist(g->adjs[de], para);
	if (!err) {
		g->A -= 1;
	}
	return err;
}

void printGrafo(grafo *g) {
	printf("V=%d, A=%d\n", g->V, g->A);
	for (int i = 0; i < g->V; i++) {
		printf("%d: ", i);
		printClist(g->adjs[i]);
	}
	putchar('\n');
}

#endif // __GRAFO__
