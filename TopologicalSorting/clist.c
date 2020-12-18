#ifndef __CLIST__
#define __CLIST__

typedef struct _clist {
	int val;
	struct _clist *next;
} clist;

clist* newClist() {
	clist *l = malloc(sizeof(*l));
	if ( !l ) {
		puts("newClist: Out of Memory.");
		exit(1);
	}
	l->next = l;
	return l;
}

void freeClist(clist *l) {
	clist *now = l->next;
	clist *prev = l;

	while ( now != l ) {
		prev = now;
		now = now->next;
		free(prev);
	}

	free(l);
}

int emptyClist(clist *l) {
	return l == l->next;
}

clist* findClist(clist *l, int val) {
	clist *now = l->next;
	clist *prev = l;

	while ( now != l && now->val < val ) {
		prev = now;
		now = now->next;
	}

	return prev;
}

void pushInClist(clist *l, int val) {
	clist *prev = findClist(l, val);

	clist *new = newClist();
	new->val = val;
	new->next = prev->next;

	prev->next = new;
}

int popClist(clist *l) {
	clist *next = l->next;

	l->next = next->next;
	int val = next->val;

	free(next);
	return val;
}

int popOutClist(clist *l, int val) {
	clist *prev = findClist(l, val);

	if ( prev->next == l || prev->next->val != val ) {
		return 1;
	}

	clist *toBeRem = prev->next;

	prev->next = prev->next->next;

	toBeRem->next = toBeRem;
	freeClist(toBeRem);

	return 0;
}

void printClist(clist *l) {
	clist *now = l->next;

	if ( now == l) {
		putchar('\n');
	}

	while ( now != l ) {
		printf("%d%s", now->val,
				now->next==l ? "\n" : ", ");
		now = now->next;
	}
}

#endif // __CLIST__
