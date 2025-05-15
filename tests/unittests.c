#include "unittests.h"

void test_create_and_delete_database() {
    command_run("create database testdb");
    command_run("delete database testdb");
    
    mm_log("Test test_create_and_delete_database: OK\n");
}

int main() {
    printf("Running unit tests:\n");
    test_create_and_delete_database();
    // Aggiungi qui le chiamate ad altre tue funzioni di test
    printf("All tests finished.\n");
    return 0;
}