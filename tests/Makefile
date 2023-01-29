CFLAGS = -O2
LDFLAGS = -pthread
OBJS = test-list

all:	$(OBJS)

%: %.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I../include -c $^

%: %.c

clean:
	rm -f core *.o *~ $(OBJS)
