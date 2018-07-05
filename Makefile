default: win

win: win.c
	gcc -Wall -o win win.c `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm -f *~ *.o win