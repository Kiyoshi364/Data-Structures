#include <stdio.h>
#include <stdlib.h>

#include "rbtree.c"

#define TAMBUFFER 100

#define IS_SPACE(x)	( (x) == ' ' || (x) == '\n' )
#define IS_NUM(x)	( '0' <= (x) && (x) <= '9' )

int readToBuf(char *buf) {
	for (int i = 0; i < TAMBUFFER; ) {
		int c = getchar();

		if ( c == '\r' ) {
		} else if ( IS_SPACE(c) || c == EOF ) {
			buf[i] = '\0';
			return c;
		} else if ( IS_NUM(c) ) {
			buf[i] = c;
			i++;
		} else {
			printf("Not Found: %c (%d)\n", c, c);
		}
	}
	return -1;
}

int main(int argc, char **argv) {
	char buf[TAMBUFFER];
	int c;

	rbt *tree = newRBT();

	// Ler entrada
	while ( (c = readToBuf(buf)) != EOF ) {
		int val = atoll(buf);

		// printf("\nMain: To insert %d\n", val);

		if ( insertRBT(tree, val) ) {
			printf("Main: insertRBT error while inserting %d\n", val);
		}

		// printf("Main: Inserted %d\n", val);

		// printPrePRBT(tree->root);
	}

	// puts("Main: Fora do While\n\n");

	/*
	printPrePRBT(tree->root);
	/*/
	printPreRBT(tree);
	//*/

	freeRBT(tree);

	return 0;
}
