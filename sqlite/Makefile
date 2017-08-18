OBJS=simple select insert create replace
all: $(OBJS)

simple: simple.c
	$(CC) -o $@ $< -lsqlite3

select: select.c
	$(CC) -o $@ $< -lsqlite3

replace: replace.c
	$(CC) -o $@ $< -lsqlite3

insert: insert.c
	$(CC) -o $@ $< -lsqlite3

create: create.c
	$(CC) -o $@ $< -lsqlite3

.PHONY: clean

clean:
	rm -f $(OBJS)
