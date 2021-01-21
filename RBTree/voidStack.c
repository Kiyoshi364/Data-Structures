typedef struct __voidStack {
	void *val;
	struct __voidStack *tail;
} voidStack;

voidStack* newVoidStack(void *val) {
	voidStack *head = malloc(sizeof(*head));
	if ( !head ) {
		puts("newVoidStack: Out of Memory.");
		exit(1);
	}
	head->val = val;
	head->tail = NULL;
	return head;
}

void freeNodeVoidStack(voidStack *head) {
	free(head);
}

void freeVoidStack(voidStack *head) {
	voidStack *tail;

	while ( head ) {
		tail = head->tail;
		freeNodeVoidStack(head);
		head = tail;
	}
}

voidStack* pushVoidStack(voidStack *head, void *val) {
	voidStack *newHead = newVoidStack(val);
	newHead->tail = head;
	return newHead;
}

voidStack* popVoidStack(voidStack *head, void **r_val) {
	if ( !head ) {
		*r_val = NULL;
		return NULL;
	}

	if ( r_val ) *r_val = head->val;
	else *r_val = NULL;
	return head->tail;
}

voidStack* popFreeVoidStack(voidStack *head, void **r_val) {
	voidStack *tail = popVoidStack(head, r_val);
	if ( head ) freeNodeVoidStack(head);
	return tail;
}

voidStack* dropNVoidStack(voidStack *head, size_t times) {
	while ( times > 0 ) {
		head = popVoidStack(head, NULL);
		times -= 1;
	}

	return head;
}

void printVoidStack(voidStack *head) {
	printf("voidStack: ");
	if ( !head ) {
		puts("(nil)");
		return;
	}

	while ( head ) {
		printf("%p -> ", head->val);
		head = head->tail;
	}
	puts("NIL");
}
