#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <pthread.h>
#include <stddef.h>

#include "node.h"

typedef struct {
    Node *top;
    size_t count;

    pthread_mutex_t lock;
    pthread_cond_t not_empty;
} Stack;

Stack *create_stack(void);
void push(Stack *stack, void *data, size_t size);
Node *pop(Stack *stack);
Node *top(Stack *Stack);
bool is_empty(Stack *stack);

void destory_stack(Stack *stack);

#endif // STACK_H