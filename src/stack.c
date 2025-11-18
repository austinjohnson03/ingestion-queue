#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "stack.h"
// TODO: Add error messages.
Stack *create_stack(void) {
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) {
        fprintf(stderr, "Failed to allocate for new stack.\n");
        return NULL;
    }

    stack->top = NULL;
    stack->count = 0;

    if (pthread_mutex_init(&stack->lock, NULL)!= 0) {
        free(stack);
        return NULL;
    }

    if (pthread_cond_init(&stack->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&stack->lock);
        free(stack);
        return NULL;
    }
    return stack;
}

void push(Stack *stack, void *data, size_t size) {
    Node *node = create_node(data, size);
    if (!node) return;

    int ret;
    ret = pthread_mutex_lock(&stack->lock);
    CHECK_PTHREAD(ret);
    if (ret != 0) {
        free_node(node);
        return NULL;
    }

    node->next = stack->top;
    stack->top = node;
    stack->count++;

    pthread_cond_signal(&stack->not_empty);
    pthread_mutex_unlock(&stack->lock);
}

Node *pop(Stack *stack) {
    pthread_mutex_lock(&stack->lock);
    if (!stack->top) {
        pthread_mutex_unlock(&stack->lock);
        fprintf(stderr, "Stack underflow.\n");
        return NULL;
    }

    Node *tmp = stack->top;
    stack->top = tmp->next;
    tmp->next = NULL;
    stack->count--;

    pthread_mutex_unlock(&stack->lock);
    return tmp;
}

Node *top(Stack *stack) {
    pthread_mutex_lock(&stack->lock);
    if (!stack->top) {
        pthread_mutex_unlock(&stack->lock);
        fprintf(stderr, "Stack underflow.\n");
        return NULL;
    }

    Node *tmp = stack->top;

    pthread_mutex_unlock(&stack->lock);
    return tmp;
}

bool is_empty(Stack *stack) {
    pthread_mutex_lock(&stack->lock);
    bool empty = (stack->top == NULL);
    pthread_mutex_unlock(&stack->lock);
    return empty;
}

void destroy_stack(Stack *stack) {
    pthread_mutex_lock(&stack->lock);
    Node *tmp = stack->top;
    while (tmp) {
        Node *next = tmp->next;
        free_node(tmp);
        tmp = next;
    }

    pthread_mutex_unlock(&stack->lock);

    pthread_mutex_destroy(&stack->lock);
    pthread_cond_destroy(&stack->not_empty);

    free(stack);
}
