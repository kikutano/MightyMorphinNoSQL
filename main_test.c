#include "tests/unittests.h"
#include "src/core/bplustree/bplustree_printer.h"

int main() {
    mm_log("Running unit tests:");
    // test_database_instructions_suite();
    // perform_full_load_test();

    BPlusTree *tree = mm_bplustree_create(3);

    mm_bplustree_insert(tree, 5);
    mm_bplustree_insert(tree, 15);
    mm_bplustree_insert(tree, 25);
    mm_bplustree_insert(tree, 35);
    mm_bplustree_insert(tree, 45);
    mm_bplustree_insert(tree, 10);

    char *tree_str = mm_bplustree_to_string(tree);
    mm_log("%s", tree_str);
    if (strcmp(tree_str, "L0:[25] L1:[15][35] L2:[5,10][15][25][35,45]") != 0) {
        mm_log("[Fail] Test failed: %s\n", tree_str);
        exit(1);
    } else {
        mm_log("[Ok] Test passed.");
    }

    free(tree_str);

    int leaf_to_find = mm_bplustree_find(tree, 10);
    if (leaf_to_find != 10) {
        mm_log("[Fail] Find leaf test failed: %i", leaf_to_find);
        exit(1);
    }
    else {
        mm_log("[Ok] Find leaf test passed. %i", leaf_to_find);
    }

    mm_log("All tests finished.");
    return 0;
}
