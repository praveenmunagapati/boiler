See https://www.sqlite.org

Sqlite is probably installed on your Linux host already. You'll need to add the
development headers to compile to the C API. On a yum based host,

    yum install sqlite3
    yum install sqlite-devel

## Sqlite3

The sqlite3 binary can be used on the command line. This creates example.db:

    % sqlite] sqlite3 example.db
    sqlite> create table people (name text, age integer);

    sqlite> .tables
    people

    sqlite> .schema people
    CREATE TABLE people (name text, age integer);

    sqlite> insert into people values ("ben", 8);
    sqlite> insert into people values ("sarah", 5);

    sqlite> select * from people;
    ben|8
    sarah|5
    sqlite> .exit

## C API

See simple.c in this directory. It is an example from the SQLite documentation.
Run "make" to compile simple from simple.c.

    ./simple example.db "select * from people;"
    name = ben
    age = 8

    name = sarah
    age = 5

The simple.c example uses the convenience wrapper `sqlite3_exec`.

### C examples

The programs `create.c`, `insert.c`, and `select.c` show basic C API usage.

  % make
  % rm -f example.db
  % ./create example.db
  % ./insert example.db
  % ./select example.db
  name: ben age 8
  name: isaac age 13

## SQLite Documentation

 * Core C API: https://www.sqlite.org/cintro.html
 * SQL language: https://www.sqlite.org/lang.html 
 * Datatypes: https://www.sqlite.org/datatype3.html



