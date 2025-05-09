<div style="text-align: center;">
    <img src="logo.png" alt="Descrizione" width="160">
</div>

# MightyMorphinNoSQL

A simple NoSql database.

## Disclaimer:
This is a **toy projects!** The goal is to create a **NoSQL Database** from scratch, super light, without dependencies just for fun and learning. This database uses **Append-Only Log** paradigm for inserting and Indexing to retrieve data. It's a good idea? No. So why? Why not?!

**Be patient, I'm writing a lot of documentation as I write code, but, in this phase I'm rewriting a lot of code. So documentation will come when I start to consolidate some principle :P**

For now this project allow you to write row on a single database and uses indexing to retrieve data by Id. 

## Supported commands:
* `[] variable value`

#### Queries:
- `select * from [table_name]`
- `select * from [table_name] where id = [value]`

#### Commands:

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

find on my_amazing_table_2 2
print database my_amazing_db.db
print table my_amazing_db_my_amazing_table_2_table.db
```

And yes, I'known there is a lot of work to do. :)