.POSIX:

include ../../config.mk

all: crt-$(ARCH)-$(SYS).o

crt-$(ARCH)-$(SYS).o: $(ARCH)-$(SYS)/crt.o
	ln -f $(ARCH)-$(SYS)/crt.o $@

clean:
	rm -f */crt.o crt-*.o
