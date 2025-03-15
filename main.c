#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

FILE *current_db_file;
char *db_file_name;
unsigned long current_id = 0;

void open_db_connection_write(char *name)
{
    db_file_name = name;
    current_db_file = fopen(name, "a");
}

void open_db_connection_read(char *name)
{
    db_file_name = name;
    current_db_file = fopen(name, "r");

    if (current_db_file == NULL) {
        printf("Error opening file %s\n", name);
        exit(1);
    }
}

void close_db_connection()
{
    fclose(current_db_file);
}

void insert_content_to_file(const char *content) {
    fprintf(current_db_file, "%lu,%s\n", current_id, content);
    ++current_id;
}

void find_content_by_id(unsigned long target_id, char* output_content, size_t content_size) {
    unsigned long id_row = 0;
    char content_buffer[256];
    
    fseek(current_db_file, 0, SEEK_SET); 
    while (fgets(content_buffer, sizeof(content_buffer), current_db_file)) {
        if (sscanf(content_buffer, "%lu", &id_row) == 1) {
            if (id_row == target_id) {
                strncpy(output_content, content_buffer, content_size);
                return;
            }
        }
    }
}

/* TODO:
    1. Optimizing the row format saved on file, we can eliminate a lot of characters
    2. Indexing searching
    3. Use fseek to jump on the right row
    4. Can we remove \n from file?
*/

int main() 
{
    printf("Mighty Morphin NoSQL release 0\n");

    clock_t start, end;
    double elapsed_time;

    start = clock(); 

    open_db_connection_read("myamazingdb.txt");

    char found_content[512];
    find_content_by_id(999999, found_content, 512);
    printf("Content found: %s\n", found_content);

    close_db_connection();

    end = clock();

    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; //Convert in milliseconds
    printf("Completed in: %.3f ms\n", elapsed_time);
    
    /*char buffer[256];

    FILE *file = fopen(DB_FILE, "a");

    for (unsigned long i = 0; i < 1000000; i++) {
        snprintf(buffer, sizeof(buffer), "{name:\"user_%lu\",age:18}", i);
        insert_content_to_file(file, i, buffer);
    }

    fclose(file);*/

    //Seeding
    /*open_db_connection("myamazingdb.txt");    
    insert_content_to_file("{name:\"user_216\",age:18}");
    insert_content_to_file("{name:\"user_216\",age:18}");
    close_db_connection();*/

    //one million records
    /*open_db_connection_write("myamazingdb.txt");
    char buffer[256];
    for (unsigned long i = 0; i < 1000000; i++) {
        snprintf(buffer, sizeof(buffer), "{name:\"user_%lu\",age:%lu}", i);
        insert_content_to_file(buffer);
    }
    close_db_connection();*/

    return 0;
}