all:
	$(CC) -I/usr/include/python2.5 -I/usr/include -L/usr/lib -lpython2.5 -o fincore.so -shared -fPIC -Wall fincore.c

clean:
	rm -f fincore.so
