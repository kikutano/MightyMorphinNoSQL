<div style="text-align: center;">
    <img src="logo.png" alt="Descrizione" width="160">
</div>

# MightyMorphinNoSQL
A simple NoSQL database built for learning purposes. This database uses the **Append-Only Log** paradigm to store data and **indexes** to retrieve documents.  
My goal is to build an entire NoSQL database from scratch — with no dependencies — that’s easy to use and as fast as possible.
Anothe goal is to maintain code as simple and minimalist as I can.

I'm not a C expert — I'm learning by doing. So be patient :P

## Milestone 1 (v0.1.3)
✅ Implemented  
👨‍💻 In progress

The goal of the first release is to allow you to:

- ✅ Create databases and tables  
- ✅ Insert documents  
- ✅ Search documents by ID as fast as possible (**currently uses divide et impera to search indexes**)  
- 👨‍💻 Search and filter by columns  
- 👨‍💻 Implement indexing using B-Tree  

## Next Goals
- CRUD operations  
- TCP layer  
- Concurrency support  
- Automated tests
- Json validator

#### Queries:
- `select * from [table_name]`
- `select * from [table_name] where id = [value]`

#### Commands, rows:
- `insert into [table] [id] '[content with spaces]'`

#### Commands, database:
- `create database [name]`
- `create table [name]`
- `open connection database [name]`

## Usage Example:
```
create database my_amazing_db
open connection my_amazing_db
craate table my_amazing_table_1
create table my_amazing_table_2
insert into my_amazing_table_1 1 'value1 with spaces'
insert into my_amazing_table_1 2 'value2 with spaces'
insert into my_amazing_table_2 1 'value3 with spaces'
insert into my_amazing_table_2 2 'value4 with spaces'
```