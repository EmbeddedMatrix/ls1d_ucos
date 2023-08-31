
%.o:%.c
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<

src = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(src))

lib1d.o: $(objs) $(src)
	$(AR) -cr  $@ $?

clean:
	rm -f *.o *.a *.s

