default: cbw reverse back win

clean:
	rm -f *~ *.o win

cbw: ChangeByWhether.c
	gcc -c -o cbw.o ChangeByWhether.c `pkg-config --cflags --libs gtk+-2.0`

reverse: Reverse.c
	gcc -c -o reverse.o Reverse.c `pkg-config --cflags --libs gtk+-2.0`

back: Background.c
	gcc -c -o back.o Background.c `pkg-config --cflags --libs gtk+-2.0`

win: win.c cbw.o reverse.o back.o
	gcc -g -Wall -o win win.c cbw.o reverse.o back.o `pkg-config --cflags --libs gtk+-2.0`