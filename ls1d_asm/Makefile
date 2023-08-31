
%.o:%.S
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<

src = $(wildcard *.S)
objs = $(patsubst %.S,%.o,$(src))

asm.o: $(objs) $(src)
	$(AR) -cr  $@ $?

clean:
	rm -f *.o *.a *.s 

