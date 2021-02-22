#include <stdio.h>
#include <stdlib.h>

#include "unionfind.c"

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
	int caso = 1;
    int n, m;
    int a, b;
    int c;
    unionFind *uf;

	// Ler entrada
	while (1) {

        c = readToBuf(buf);
        if ( c != ' ' ) {
            printf("Esperava um ' ' recebi '%c'(%d)\n", c, c);
        }
        n = atol(buf);

        c = readToBuf(buf);
        if ( c != '\n' ) {
            printf("Esperava um '\\n' recebi '%c'(%d)\n", c, c);
        }
        m = atol(buf);

        if ( n == 0 && m == 0 ) break;

        uf = newUF(n);

        while ( m > 0 ) {
            c = readToBuf(buf);
            if ( c != ' ' ) {
                printf("Esperava um ' ' recebi '%c'(%d)\n", c, c);
            }
            a = atol(buf);

            c = readToBuf(buf);
            if ( c != '\n' ) {
                printf("Esperava um '\\n' recebi '%c'(%d)\n", c, c);
            }
            b = atol(buf);

            c = unionUF(uf, a-1, b-1);
            if ( c < 0 ) {
                puts("Algo deu errado na união!");
            }

            m -= 1;
        }

        c = countUF(uf);

        printf("Case %d: %d\n", caso, c);
        caso += 1;

        freeUF(uf);
    }

	return 0;
}
