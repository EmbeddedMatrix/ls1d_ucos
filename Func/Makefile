
%.o:%.c
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<

src = $(wildcard *.c)
objs = $(patsubst %.c,%.o,$(src))

Func.o: $(objs) $(src)
	$(AR) -cr  $@ $?

clean:
	rm -f *.o *.a *.s