#include "loadtests.h"
#include "../src/mightymorphinnosql.h"
#include <stdio.h>
#include <time.h>

const long long NUM_OPERATIONS = 10000;

void perform_bulk_insert() {
  char query_buffer[256];
  clock_t start_time, end_time;
  double time_spent;
  double throughput_qps = 0;

  mm_log("Phase 1: Performing %lld INSERT queries", NUM_OPERATIONS);
  start_time = clock();

  for (long long i = 0; i < NUM_OPERATIONS; ++i) {
    snprintf(query_buffer, sizeof(query_buffer),
             "insert into users %lld '{\"name\":\"user%lld\", \"age\":%d}'", i,
             i, 18);

    command_run(query_buffer);
  }

  end_time = clock();
  time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  throughput_qps = NUM_OPERATIONS / time_spent;
  mm_log("Finished %lld INSERT queries in %.2f seconds. QPS: %.2f",
         NUM_OPERATIONS, time_spent, throughput_qps);
}

void perform_bulk_queries() {
  char query_buffer[256];
  clock_t start_time, end_time;
  double time_spent;
  double throughput_qps = 0;

  mm_log("Phase 2: Performing %lld SELECT queries on latest id", NUM_OPERATIONS);
  start_time = clock();

  for (long long i = 0; i < NUM_OPERATIONS; ++i) {
    snprintf(query_buffer, sizeof(query_buffer),
             "select * from users where id = %lld", i);

    command_run(query_buffer);
  }

  end_time = clock();
  time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  throughput_qps = NUM_OPERATIONS / time_spent;
  mm_log("Finished %lld SELECT queries in %.2f seconds. QPS: %.2f",
         NUM_OPERATIONS, time_spent, throughput_qps);
}

void perform_full_load_test() {
  mm_log("Starting Full Load Test");

  command_run("create database testdb");
  command_run("open connection database testdb");
  command_run("create table users");

  perform_bulk_insert();
  perform_bulk_queries();

  command_run("close connection database testdb");
  command_run("delete database testdb");

  mm_log("Full Load Test Completed");
  mm_log("QPS: Query Per Second");
}