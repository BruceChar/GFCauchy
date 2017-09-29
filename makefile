obj=./test/test_gf.c gf.c
all:
	cc ${obj} -Wall -g -o ./gftest --std=c99 -Iinclude


clean:
	rm -rf *.o crstest
