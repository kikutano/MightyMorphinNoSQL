#include "unittests.h"

void test_database_instructions_suite() {
  command_run("create database testdb");
  command_run("open connection database testdb");
  command_run("create table users");
  /*command_run("insert into users 1 '{\"name:\": \"user1\", \"age:\": 18}'");
  command_run("insert into users 2 '{\"name:\": \"user2\", \"age:\": 25}'");
  command_run("insert into users 3 '{\"name:\": \"user3\", \"age:\": 14}'");
  command_run("insert into users 4 '{\"name:\": \"user4\", \"age:\": 38}'");
  command_run("select * from users");
  command_run("select * from users where id = 3");*/
  command_run("close connection database testdb");
  command_run("delete database testdb");
}

int main() {
  mm_log("Running unit tests:");
  test_database_instructions_suite();
  mm_log("All tests finished.");
  return 0;
}