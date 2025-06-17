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

    //insert the new key
    node->keys[i] = key;
    node->num_keys++;
}

void insert_in_internal(BPlusTree *tree, BPlusTreeNode *parent, int promoted_key,
                        BPlusTreeNode *left, BPlusTreeNode *right) {
    int i;

    // Trova la posizione in cui inserire la chiave promossa
    for (i = 0; i < parent->num_keys && parent->keys[i] < promoted_key; i++);

    // Sposta chiavi e puntatori per fare spazio
    for (int j = parent->num_keys; j > i; j--) {
        parent->keys[j] = parent->keys[j - 1];
        parent->pointers[j + 1] = parent->pointers[j];
    }

    // Inserisci la nuova chiave e il nuovo puntatore destro
    parent->keys[i] = promoted_key;
    parent->pointers[i + 1] = right;
    parent->num_keys++;

    right->parent = parent;

    // 🔁 Se il nodo è ora pieno, bisogna splittarlo e propagare ancora
    if (parent->num_keys == tree->order) {
        int mid = parent->num_keys / 2;
        int new_promoted = parent->keys[mid];

        BPlusTreeNode *new_internal = create_node(tree, false);
        new_internal->parent = parent->parent;

        // Copia metà destra delle chiavi e dei figli nel nuovo nodo
        new_internal->num_keys = 0;
        for (int j = mid + 1, k = 0; j < parent->num_keys; j++, k++) {
            new_internal->keys[k] = parent->keys[j];
            new_internal->pointers[k] = parent->pointers[j];
            if (new_internal->pointers[k]) {
                ((BPlusTreeNode *)new_internal->pointers[k])->parent = new_internal;
            }
            new_internal->num_keys++;
        }
        // Copia l’ultimo puntatore
        new_internal->pointers[new_internal->num_keys] = parent->pointers[parent->num_keys];
        if (new_internal->pointers[new_internal->num_keys]) {
            ((BPlusTreeNode *)new_internal->pointers[new_internal->num_keys])->parent = new_internal;
        }

        // Riduci il nodo originale
        parent->num_keys = mid;

        // Se il nodo era la root
        if (parent == tree->root) {
            BPlusTreeNode *new_root = create_node(tree, false);
            new_root->keys[0] = new_promoted;
            new_root->pointers[0] = parent;
            new_root->pointers[1] = new_internal;
            new_root->num_keys = 1;

            parent->parent = new_root;
            new_internal->parent = new_root;
            tree->root = new_root;
        } else {
            // Altrimenti, ricorsivamente inserisci nel padre
            insert_in_internal(tree, parent->parent, new_promoted, parent, new_internal);
        }
    }
}


void split_leaf_node(BPlusTree *tree, BPlusTreeNode *node, int key) {
    // Inserisci la nuova chiave nel nodo esistente prima dello split
    insert_in_leaf(node, key);

    //find the node to promote
    int split_index = node->num_keys / 2;

    BPlusTreeNode *right_node_new = create_node(tree, true);
    right_node_new->parent = node->parent;

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
    if (!tree || !tree->root) {
        printf("(tree is empty)\n");
        return;
    }

    // Coda per BFS
    BPlusTreeNode **queue = malloc(sizeof(BPlusTreeNode*) * 1000);
    int front = 0, back = 0;

    queue[back++] = tree->root;
    int nodes_in_level = 1;
    int next_level_count = 0;
    int level = 0;

    printf("\n--- B+Tree ---\n");

    while (front < back) {
        printf("Level %d: ", level);

        int printed = 0;
        for (int i = 0; i < nodes_in_level; i++) {
            BPlusTreeNode *node = queue[front++];

            printf("[");
            for (int j = 0; j < node->num_keys; j++) {
                printf("%d", node->keys[j]);
                if (j < node->num_keys - 1)
                    printf(" | ");
            }
            printf("] ");

            // Se non è foglia, aggiungiamo i figli alla coda
            if (!node->is_leaf) {
                for (int j = 0; j <= node->num_keys; j++) {
                    queue[back++] = (BPlusTreeNode *)node->pointers[j];
                    next_level_count++;
                }
            }

            printed++;
        }

        printf("\n");

        // Vai al livello successivo
        nodes_in_level = next_level_count;
        next_level_count = 0;
        level++;
    }

    free(queue);
    printf("--- end ---\n\n");
}

