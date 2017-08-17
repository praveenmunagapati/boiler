OBJS=simple standard 
all: $(OBJS)

simple: simple.c
	$(CC) -o $@ $< -lsqlite3

standard: standard.c
	$(CC) -o $@ $< -lsqlite3

.PHONY: clean

clean:
	rm -f $(OBJS)
