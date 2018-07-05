default: win

clean:
	rm -f *~ *.o win

kawakami: Kawakami.c
	gcc -c -o kawakami.o Kawakami.c `pkg-config --cflags --libs gtk+-2.0`

kondo: Kondo.c
	gcc -c -o kondo.o Kondo.c `pkg-config --cflags --libs gtk+-2.0`

tohata: Tohata.c
	gcc -c -o tohata.o Tohata.c `pkg-config --cflags --libs gtk+-2.0`

win: win.c kawakami.o kondo.o tohata.o
	gcc -g -Wall -o win win.c kawakami.o kondo.o tohata.o `pkg-config --cflags --libs gtk+-2.0`