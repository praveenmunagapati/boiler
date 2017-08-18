OBJS=simple standard insert create
all: $(OBJS)

simple: simple.c
	$(CC) -o $@ $< -lsqlite3

standard: standard.c
	$(CC) -o $@ $< -lsqlite3

insert: insert.c
	$(CC) -o $@ $< -lsqlite3

create: create.c
	$(CC) -o $@ $< -lsqlite3

.PHONY: clean

clean:
	rm -f $(OBJS)
