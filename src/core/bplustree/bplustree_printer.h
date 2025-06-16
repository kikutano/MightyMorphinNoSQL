#include <stdio.h>
#include <stdlib.h>
#include "bplustree.h"

#define QUEUE_SIZE 100

typedef struct NodeQueue {
    BPlusTreeNode *items[QUEUE_SIZE];
    int front, rear;
} NodeQueue;

void enqueue(NodeQueue *q, BPlusTreeNode *node) {
    if ((q->rear + 1) % QUEUE_SIZE == q->front) return; // full
    q->items[q->rear] = node;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
}

BPlusTreeNode *dequeue(NodeQueue *q) {
    if (q->front == q->rear) return NULL; // empty
    BPlusTreeNode *node = q->items[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return node;
}

int is_empty(NodeQueue *q) {
    return q->front == q->rear;
}

void print_tree(BPlusTreeNode *root) {
    if (root == NULL) {
        printf("L'albero è vuoto.\n");
        return;
    }

    NodeQueue queue = {.front = 0, .rear = 0};
    enqueue(&queue, root);
    enqueue(&queue, NULL); // fine livello

    int level = 0;
    printf("Livello %d: ", level);

    while (!is_empty(&queue)) {
        BPlusTreeNode *node = dequeue(&queue);

        if (node == NULL) {
            printf("\n");
            if (!is_empty(&queue)) {
                enqueue(&queue, NULL);
                level++;
                printf("Livello %d: ", level);
            }
            continue;
        }

        printf("[");
        for (int i = 0; i < node->num_keys; i++) {
            printf("%d", node->keys[i]);
            if (i < node->num_keys - 1) printf(" ");
        }
        printf("] ");

        if (!node->is_leaf) {
            for (int i = 0; i <= node->num_keys; i++) {
                enqueue(&queue, (BPlusTreeNode *) node->pointers[i]);
            }
        }
    }
}
