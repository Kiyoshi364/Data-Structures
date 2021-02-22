typedef struct _unionFind {
    size_t len;
    unsigned int *elems;
    unsigned int *ranks;
} unionFind;

unionFind* newUF(size_t len) {
    unionFind *uf = malloc(sizeof(*uf));
    if ( !uf ) {
		puts("newUF: Out of Memory.");
		exit(1);
    }

    uf->len = len;
    uf->elems = malloc(sizeof(*uf->elems)*len);
    if ( !(uf->elems) ) {
		puts("newUF: Out of Memory (uf->elems).");
		exit(1);
    }

    uf->ranks = malloc(sizeof(*uf->ranks)*len);
    if ( !(uf->elems) ) {
		puts("newUF: Out of Memory (uf->ranks).");
		exit(1);
    }

    for ( size_t i = 0; i < len; i++ ) {
        uf->elems[i] = i;
        uf->ranks[i] = 0;
    }

    return uf;
}

void freeUF(unionFind *uf) {
    free(uf->elems);
    free(uf->ranks);
    free(uf);
}

int findUF(unionFind *uf, size_t elem) {
    size_t el = elem;

    while ( el != uf->elems[el] ) {
        el = uf->elems[el];
    }

    while ( elem != el ) {
        size_t next = uf->elems[elem];
        uf->elems[elem] = el;
        elem = next;
    }
    return el;
}

int unionUF(unionFind *uf, size_t x, size_t y) {
    if ( x == y || x >= uf->len || y >= uf->len ) {
        return -1;
    }

    size_t a = findUF(uf, x);
    size_t b = findUF(uf, y);

    if ( uf->ranks[a] < uf->ranks[b] ) {
        uf->elems[a] = b;
    } else {
        uf->elems[b] = a;
        if ( uf->ranks[a] == uf->ranks[b] ) {
            uf->ranks[a] += 1;
        }
    }
    return 0;
}

int countUF(unionFind *uf) {
    unsigned int elems[uf->len];
    size_t count = 0;
    for ( int i = 0; i < uf->len; i++ ) {
        elems[i] = 0;
    }

    for ( int i = 0; i < uf->len; i++ ) {
        elems[findUF(uf, i)] = 1;
    }

    for ( int i = 0; i < uf->len; i++ ) {
        count += elems[i];
    }
    return count;
}

void printUF(unionFind *uf) {
    printf("elems: ");
    for ( int i = 0; i < uf->len; i++ ) {
        printf("%d ", uf->elems[i]);
    }
    printf("\nranks: ");
    for ( int i = 0; i < uf->len; i++ ) {
        printf("%d ", uf->ranks[i]);
    }
    printf("\n");
}
