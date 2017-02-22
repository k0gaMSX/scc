
all: tests

tests:
	SCCEXECPATH=../../bin PATH=../../bin:$$PATH ./chktest.sh `cat scc-tests.lst`
clean:
	rm -f *.o *.ir *.qbe *core
