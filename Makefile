CC = gcc
CFLAGS = -g -O0 -Wall

target = pg

objs = main.o \
	memory.o \
	page_allocator.o \
	paging.o

$(target): $(objs)
	$(CC) $(CFLAGS) -lpthread $(objs) -o $(target)

.c.o:
	$(CC) $(CFLAGS) -I. -c $<

test:
	./$(target)

clean:
	rm -f *~ core* *.o $(target)

.PHONY: clean $(target)
