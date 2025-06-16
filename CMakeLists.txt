cmake_minimum_required(VERSION 3.31)
project(MightyMorphinNoSQL C)

set(CMAKE_C_STANDARD 11)

# Common
set(SOURCE_FILES
        src/mightymorphinnosql.c
        src/core/database/database_manager.c
        src/core/database/mmdb_file_manager.c
        src/core/inserting/insert.c
        src/core/querying/query.c
        src/core/bplustree/bplustree.c
        src/commands/input_parser.c
        src/logs/mm_log.c
        src/utils/utils.c
)

# Main
add_executable(MightyMorphinNoSQL
        main.c
        ${SOURCE_FILES}
)

# Tests
add_executable(MMNoSQLTests
        main_test.c
        src/core/bplustree/bplustree_example.h
        src/core/bplustree/bplustree_printer.h
        ${SOURCE_FILES}
)