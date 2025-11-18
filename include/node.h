#ifndef NODE_H
#define NODE_H

typedef struct Node {
    void *data;
    size_t size;
    struct Node *next;
} Node;

Node* create_node(const void *data, size_t size);
void free_node(Node *node);

#endif // NODE_H