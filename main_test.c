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

    mm_bplustree_print(tree);

    mm_log("All tests finished.");
    return 0;
}
