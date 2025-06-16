#include "bplustree.h"
#include <stdio.h>
#include <stdlib.h>

BPlusTreeNode *create_node(BPlusTree *tree, bool is_leaf) {
    BPlusTreeNode *node = malloc(sizeof(BPlusTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    node->parent = NULL;
    node->next = NULL;

    for (int i = 0; i < ORDER - 1; i++) {
        node->keys[i] = 0;
    }

    for (int i = 0; i < ORDER; i++) {
        node->pointers[i] = NULL;
    }

    return node;
}

void insert_in_leaf(BPlusTreeNode *node, int key) {
    int i = 0;

    //find the position to place the key
    for (i = 0; i < node->num_keys && node->keys[i] < key; i++);

    //shift the remaining keys on right
    for (int j = node->num_keys; j > i; j--) {
        node->keys[j] = node->keys[j - 1];
        node->pointers[j] = node->pointers[j - 1]; // swap the values
    }

    //insert the new key
    node->keys[i] = key;
    node->pointers[i] = NULL;
    node->num_keys++;
}

void insert_in_internal(BPlusTree *tree, BPlusTreeNode *parent, int promoted_key, BPlusTreeNode *left,
                        BPlusTreeNode *right) {
    if (parent->num_keys == tree->order - 1) {
        //we need to split the parent node! good luck!
        return;
    } else {
        insert_in_leaf(parent, promoted_key);
    }
}

void split_leaf_node(BPlusTree *tree, BPlusTreeNode *node, int key) {
    // Inserisci la nuova chiave nel nodo esistente prima dello split
    insert_in_leaf(node, key);

    //find the node to promote
    int split_index = node->num_keys / 2;

    BPlusTreeNode *right_node_new = create_node(tree, true);

    // copy all right values on right node
    for (int i = 0; i < node->num_keys - split_index; i++) {
        right_node_new->keys[i] = node->keys[i + split_index];
        right_node_new->num_keys++;
    }

    // copy all left values on existing node (left)
    node->num_keys = 0;
    for (int i = 0; i < split_index; i++) {
        node->keys[i] = node->keys[i];
        node->num_keys++;
    }

    for (int i = split_index; i < (split_index * 2) + 1; i++) {
        node->keys[i] = 0;
    }

    node->next = right_node_new;

    // La chiave da promuovere è la prima del nuovo nodo destro
    int promoted_key = right_node_new->keys[0];

    if (node == tree->root) {
        BPlusTreeNode *new_root = create_node(tree, false);
        new_root->keys[0] = promoted_key;
        new_root->pointers[0] = node;
        new_root->pointers[1] = right_node_new;
        new_root->num_keys = 1;

        node->parent = new_root;
        right_node_new->parent = new_root;

        tree->root = new_root;
    } else {
        // Promuovi la chiave nel padre esistente
        BPlusTreeNode *parent = node->parent;
        insert_in_internal(tree, parent, promoted_key, node, right_node_new);
    }
}

BPlusTreeNode *find_leaf_node(BPlusTree *tree, int key) {
    BPlusTreeNode *curr_node = tree->root;

    if (curr_node->is_leaf)
        return curr_node;

    // find the right path pointer
    int i = 0;
    while (1) {
        //find the correct pointer
        for (i = 0; i < curr_node->num_keys && key > curr_node->keys[i]; i++);
        if (!curr_node->is_leaf) {
            curr_node = curr_node->pointers[i];
        } else {
            return curr_node;
        }
    }
}

BPlusTree *mm_bplustree_create(int order) {
    BPlusTree *tree = malloc(sizeof(BPlusTree));
    tree->order = order;
    tree->root = NULL;
    return tree;
}

void mm_bplustree_insert(BPlusTree *tree, int key) {
    if (tree->root == NULL) {
        tree->root = create_node(tree, true);
        insert_in_leaf(tree->root, key);
        return;
    }

    BPlusTreeNode *leaf_node = find_leaf_node(tree, key);
    if (leaf_node->num_keys == tree->order - 1) {
        split_leaf_node(tree, leaf_node, key);
    } else {
        insert_in_leaf(leaf_node, key);
    }
}

void mm_bplustree_print(BPlusTree *tree) {
    BPlusTreeNode *curr_node = tree->root;

    unsigned int level = 0;
    printf("level %i: ", level);
    while (1) {
        for (int i = 0; i < curr_node->num_keys; i++) {
            BPlusTreeNode *pointer = NULL;
            pointer = curr_node->pointers[i];

            int pointer_first_key = -1;
            if (pointer != NULL)
                pointer_first_key = pointer->keys[0];

            printf("(pV: %i)[k: %i]", pointer_first_key, curr_node->keys[i]);
        }

        int last_pointer_last_key = -1;
        BPlusTreeNode *pointer = curr_node->pointers[curr_node->num_keys];
        if (pointer != NULL) {
            last_pointer_last_key = pointer->keys[0];
        }

        printf("(pV: %i)", last_pointer_last_key);
        if (curr_node->is_leaf & curr_node->next != NULL) {
            curr_node = curr_node->next;
            printf((" --> "));
        } else if (curr_node->is_leaf && curr_node->next == NULL) {
            break;
        } else {
            curr_node = curr_node->pointers[0];
            level++;
            printf("\n");
            printf("level %i: ", level);
        }
    }
    printf("\n");
}
