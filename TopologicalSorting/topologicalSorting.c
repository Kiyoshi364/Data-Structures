#include <stdio.h>
#include <stdlib.h>

#include "grafo.c"

#define TAMBUFFER 100

#define IS_SPACE(x)	( (x) == ' ' || (x) == '\n' )
#define IS_NUM(x)	( '0' <= (x) && (x) <= '9' )

int readToBuf(char *buf) {
	for (int i = 0; i < TAMBUFFER; ) {
		int c = getchar();

		if ( IS_SPACE(c) ) {
			buf[i] = '\0';
			return c;
		} else if ( IS_NUM(c) ) {
			buf[i] = c;
			i++;
		} else {
			printf("Not Found: %c\n", c);
		}
	}
	return -1;
}

int* topoSort(grafo *g) {
	int graus[g->V];
	int end = 0;

	int *queue = malloc(sizeof(*queue)*(g->V));
	if ( !queue ) {
		puts("topoSort: Out of Memory");
		exit(1);
	}

	for (int i = 0; i < g->V; i++) {
		graus[i] = 0;
	}

	// Calcular graus
	for (int i = 0; i < g->V; i++) {
		clist *adj = (g->adjs)[i];
		clist *now = adj->next;

		while ( now != adj ) {
			graus[ now->val ] += 1;
			now = now->next;
		}
	}

	// "Push" Primeiros Sources
	for (int i = 0; i < g->V; i++) {
		if ( graus[i] == 0 ) {
			queue[end] = i;
			end++;
		}
	}

	// Remover Vertices e Atualizar Queue
	for (int i = 0; i < end && end < g->V; i++) {
		clist *adj = (g->adjs)[ queue[i] ];
		clist *now = adj->next;

		while ( now != adj ) {
			int val = now->val;

			graus[val] -= 1;
			if ( graus[val] == 0 ) {
				queue[end] = val;
				end++;
			}

			now = now->next;
		}
	}

	// Checar Ciclos
	if ( end < g->V ) {
		printf("TopoSort: Queue is Empty Cicle Found\n");
		printGrafo(g);
		exit(0);
	}

	return queue;
}

int main(int argc, char **argv) {
	int V, A;

	char buf[TAMBUFFER];
	int c;

	grafo *graf;

	// Ler entrada
	c = readToBuf(buf);
	V = atoll(buf);

	c = readToBuf(buf);
	A = atoll(buf);

	graf = newGrafo(V);

	for (int i = 0, node = 0; i < A || node < V; i++) {
		int other;
		c = readToBuf(buf);

		other = atoll(buf);

		if ( other > 0 ) {
			addAresta(graf, node, other-1);
		}

		if ( c == '\n' ) {
			node += 1;
		}
	}

	// Organizar
	int *indexes = topoSort(graf);

	for (int i = 0; i < V; i++) {
		printf("%d%c", indexes[i]+1, V-i-1?32:10);
	}

	free(indexes);

	freeGrafo(graf);

	return 0;
}
