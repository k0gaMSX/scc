.POSIX:

all: tests

tests:
	CFLAGS='' SCCPREFIX=../../rootdir/ PATH=../../rootdir/bin:$$PATH ./chktest.sh scc-tests.lst

clean:
	rm -f *.as *.o *.ir *.qbe *core test.log

distclean: clean
dep:
