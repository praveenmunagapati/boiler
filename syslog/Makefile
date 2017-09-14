OBJS=syslogger io-to-ux ux-to-syslog
all: $(OBJS)

syslogger: syslogger.c
	$(CC) -o $@ $<

io-to-ux: io-to-ux.c
	$(CC) -o $@ $<

ux-to-syslog: ux-to-syslog.c
	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS)
