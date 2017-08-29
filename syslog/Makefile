OBJS=syslogger
all: $(OBJS)

syslogger: syslogger.c
	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS)
