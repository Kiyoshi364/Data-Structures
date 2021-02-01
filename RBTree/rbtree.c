#include "voidStack.c"

typedef unsigned char byte;

#define __RED_COLOR__ ~0
#define __BLACK_COLOR__ 0

#define COLOR(x) ((x) ? x->color : __BLACK_COLOR__)

typedef struct __rbt_node {
	char color;
	int val;
	struct __rbt_node *right;
	struct __rbt_node *left;
} _rbt_node;

_rbt_node* new_rbt_node(int val) {
	_rbt_node *n = malloc(sizeof(*n));
	if ( !n ) {
		puts("new_rbt_node: Out of Memory.");
		exit(1);
	}
	n->color = __RED_COLOR__;
	n->val = val;
	n->right = 0;
	n->left = 0;
	return n;
}

void free_rbt_node(_rbt_node *n) {
	if (n->right) {
		free_rbt_node(n->right);
	}
	if (n->left) {
		free_rbt_node(n->left);
	}
	free(n);
}

void print_rbt_node(_rbt_node *n) {
	if ( !n ) {
		printf("NN ");
	} else {
		printf("%d%c ", n->val, COLOR(n)==__RED_COLOR__?'R':'N');
	}
}

typedef struct _rbt {
	_rbt_node *root;
} rbt;

rbt* newRBT() {
	rbt *t = malloc(sizeof(*t));
	return t;
}

void freeRBT(rbt *t) {
	free_rbt_node(t->root);
	free(t);
}

void fixColors(_rbt_node **root, _rbt_node *next2, _rbt_node *next, _rbt_node *now) {
	if ( !(COLOR(now) == __RED_COLOR__ && COLOR(next) == __RED_COLOR__) ) {
		return;
	}

	if ( COLOR(next2->left) == COLOR(next2->right) ) {
		next2->left->color = __BLACK_COLOR__;
		next2->right->color = __BLACK_COLOR__;
		next2->color = __RED_COLOR__;
	} else { // Rotations
		// left
		if ( next2->left == next ) {
			// left left
			if ( next->left == now ) {
				next2->left = next->right;
				next->right = next2;

				next->color = __BLACK_COLOR__;
				next2->color = __RED_COLOR__;

				*root = next;
			// left right
			} else {
				next2->left = now->right;
				next->right = now->left;

				now->left = next;
				now->right = next2;

				now->color = __BLACK_COLOR__;
				next2->color = __RED_COLOR__;

				*root = now;
			}
		// right
		} else {
			// right right
			if ( next->right == now ) {
				next2->right = next->left;
				next->left = next2;

				next->color = __BLACK_COLOR__;
				next2->color = __RED_COLOR__;

				*root = next;
			// right left
			} else {
				next2->right = now->left;
				next->left = now->right;

				now->left = next2;
				now->right = next;

				now->color = __BLACK_COLOR__;
				next2->color = __RED_COLOR__;

				*root = now;
			}
		}
	}
}

int insertRBT(rbt *t, int val) {
	if ( !t ) {
		puts("insertRBT: t is NULL");
		return 1;
	} else if ( !(t->root) ) {
		t->root = new_rbt_node(val);
		return 0;
	}

	voidStack *nodeStack = NULL;

	_rbt_node *newNode = new_rbt_node(val);

	_rbt_node *now = t->root;
	_rbt_node *next = NULL, *next2 = NULL, *next3 = NULL;
	long dir = 0;

#ifdef DEBUG
	printf("Before search while:\n\t"); // DEBUG
	printVoidStack(nodeStack); // DEBUG
#endif // DEBUG
	// Find a path
	while ( now ) {

		if ( val <= now->val ) {
			dir = 0;
			next = now->left;
		} else {
			dir = 1;
			next = now->right;
		}

		nodeStack = pushVoidStack(nodeStack, (void *) now);
		now = next;

#ifdef DEBUG
		printf("End of search while:\n\t"); // DEBUG
		printVoidStack(nodeStack); // DEBUG
#endif // DEBUG
	}
#ifdef DEBUG
	printf("After search while:\n\t"); // DEBUG
	printVoidStack(nodeStack); // DEBUG
#endif // DEBUG

	nodeStack = popFreeVoidStack(nodeStack, (void **) &now);

	// Insert
	if ( dir == 0 ) {
		now->left = newNode;
	} else {
		now->right = newNode;
	}

	// Recolor
	next = now;
	now = newNode;
	
	// If depth is 2, color root to Black, return
	if ( !nodeStack ) {
		t->root->color = __BLACK_COLOR__;
		return 0;
	}

	nodeStack = popFreeVoidStack(nodeStack, (void **) &next2);

	nodeStack = popFreeVoidStack(nodeStack, (void **) &next3);

#ifdef DEBUG
	printf("Before recolor while:\n\t"); // DEBUG
	printVoidStack(nodeStack); // DEBUG
#endif // DEBUG
	// next3	= grand-grandfather
	// next2	= grandfather
	// next		= father
	// now		= son
	while ( next3 ) {
		if ( next3->left == next2 )
			fixColors(&(next3->left), next2, next, now);
		else
			fixColors(&(next3->right), next2, next, now);

		now = next;
		next = next2;
		next2 = next3;
		nodeStack = popFreeVoidStack(nodeStack, (void **) &next3);
		return 0;
	}
#ifdef DEBUG
	printf("After recolor while:\n\t"); // DEBUG
	printVoidStack(nodeStack); // DEBUG

	puts("Before Fix"); // DEBUG
#endif // DEBUG

	fixColors(&(t->root), next2, next, now);

#ifdef DEBUG
	puts("After Fix"); // DEBUG
#endif // DEBUG

	// Free stack
	freeVoidStack(nodeStack);

	t->root->color = __BLACK_COLOR__;

	return 0;
}

void printPrePRBT(_rbt_node *n) {
	if ( !n ) {
		printf("NN");
		return;
	}
	printf("(");
	print_rbt_node(n);

	printPrePRBT(n->left);
	printf(" ");

	printPrePRBT(n->right);
	printf(")");
}

void printPreRBT(rbt *t) {
	if ( !t ) {
		return;
	}

	voidStack *stack = newVoidStack(t->root);

	_rbt_node *now;

	while ( stack ) {
		stack = popFreeVoidStack(stack, (void **) &now);

		// Print now
		print_rbt_node(now);

		if ( now->right )
			stack = pushVoidStack(stack, (void **) now->right);

		if ( now->left )
			stack = pushVoidStack(stack, (void **) now->left);
	}
}
