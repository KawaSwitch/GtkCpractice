default: win testpct

win: win.c
	gcc -Wall -o win win.c `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm -f *~ *.o win testpct

pct: pct.c
	gcc -Wall -c -o pct.o pct.c

testpct: main.c pct.o
	gcc -Wall -o testpct main.c pct.o