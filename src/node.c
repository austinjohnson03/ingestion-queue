#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* create_node(const void *data, size_t size) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Unable to allocate memory for new Node.\n");
        return NULL;
    }

    node->data = malloc(size);
    if (!node->data) {
        free(node);
        fprintf(stderr, "Unable to allocate memory for Node data.\n");
        return NULL;
    }

    memcpy(node->data, data, size);
    node->size = size;
    node->next = NULL;
    return node;
}

void free_node(Node *node) {
    if (!node) return;

    if (node->data) {
        free(node->data);
        node->data = NULL;
    }

    node->next = NULL;
    free(node);
}