OBJS=syslogger sockout socksrv
all: $(OBJS)

syslogger: syslogger.c
	$(CC) -o $@ $<

sockout: sockout.c
	$(CC) -o $@ $<

socksrv: socksrv.c
	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS)
