all: libsconf.a

libsconf.a: sconf.c
	$(CC) $(CFLAGS) -c $<
	ar cru libsconf.a sconf.o

clean:
	rm -f libsconf.a *.o
	make -C tests clean
