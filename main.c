#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

FILE* current_db_file;
char* db_file_name;
unsigned long current_id = 0;

void open_db_connection_write(char* name)
{
	db_file_name = name;
	current_db_file = fopen(name, "a");
}

void open_db_connection_read(char* name)
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

void insert(const char* content) 
{
	fprintf(current_db_file, "%lu,%s\n", current_id, content);
	++current_id;
}

void find_by_id(unsigned long target_id, char* output_content, size_t content_size) 
{
	unsigned long id_row = 0;
	char content_buffer[256];

	while (fgets(content_buffer, sizeof(content_buffer), current_db_file)) 
    {
		if (sscanf(content_buffer, "%lu", &id_row) == 1)
        {
			if (id_row == target_id) 
            {
				strncpy(output_content, content_buffer, content_size);
				return;
			}
		}
	}
}

/* Utils methods */
void insert_one_million_rows()
{
	open_db_connection_write("myamazingdb.txt");
	char buffer[256];
	for (unsigned long i = 0; i < 1000000; i++) {
		snprintf(buffer, sizeof(buffer), "{name:\"user_%lu\",age:%lu}", i);
		insert(buffer);
	}
	close_db_connection();
}

clock_t start, end;
double elapsed_time;
void start_stopwatch()
{
	start = clock();
}

void stop_stopwatch()
{
	end = clock();
	elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; //Convert in milliseconds
	printf("Completed in: %.3f ms\n", elapsed_time);
}

/* TODO:
	1. Optimizing the row format saved on file, we can eliminate a lot of characters
	2. Indexing searching
	3. Use fseek to jump on the right row
	4. Can we remove \n from file?
    4. Save index_counter on file!
*/

int main(int argc, char* argv[])
{
	printf("Mighty Morphin NoSQL release 0\n");
	printf("Usage: find <id>\n");
	printf("Usage: insert <content>\n");
	printf("Usage: insert_one_millions (insert one million of rows)\n");

	if (strcmp(argv[1], "find") == 0) {
		unsigned long id = strtoul(argv[2], NULL, 10);

		open_db_connection_read("myamazingdb.txt");
		char found_content[512];
		start_stopwatch();
		find_by_id(id, found_content, 512);
		stop_stopwatch();

		printf("Content found: %s\n", found_content);
		close_db_connection();
		return 0;
	}
	else if (strcmp(argv[1], "insert") == 0) {
        printf("Inserting %s\n", argv[2]);
        open_db_connection_write("myamazingdb.txt");

		insert(argv[2]);        
        close_db_connection();
	}
	else if (strcmp(argv[1], "insert_one_millions")) {
		insert_one_million_rows();
	}

	return 0;
}