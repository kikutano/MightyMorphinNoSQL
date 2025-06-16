#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#include <stdbool.h>

#define ORDER 4

typedef struct BPlusTreeNode {
    bool is_leaf;
    int num_keys;
    int keys[ORDER - 1];
    void *pointers[ORDER]; //can be child or data if leaf
    struct BPlusTreeNode *parent;
    struct BPlusTree *next;
} BPlusTreeNode;

typedef struct BPlusTree {
    BPlusTreeNode *root;
    int order;
} BPlusTree;

BPlusTree *mm_bplustree_create(int order);

void mm_bplustree_insert(BPlusTree *tree, int key);

void mm_bplustree_print(BPlusTree *tree);

#endif
