PROGS=test1 test2 test3 test4 test5
OBJS=$(patsubst %,%.o,$(PROGS))

CFLAGS = -I../
CFLAGS += -g -O0
CFLAGS += -Wall -Wextra

LDFLAGS=

TEST_TARGET=run_tests
TESTS=./do_tests

all: $(OBJS) $(PROGS) $(TEST_TARGET) 

# static pattern rule: multiple targets 

$(OBJS) ../print-decimal.o: %.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $< 

$(PROGS): %: %.o ../print-decimal.o
	$(CC) -o $@ $(CFLAGS) $< ../print-decimal.o $(LDFLAGS)


run_tests: $(PROGS)
	perl $(TESTS)

.PHONY: clean

clean:	
	rm -f $(PROGS) $(OBJS) *.o test*.out 
