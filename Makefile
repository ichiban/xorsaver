CFLAGS=-lswscale -lavutil -lavformat -lavcodec -lz -lavutil -lm `sdl-config --cflags --libs`

DEFAULT: xorsaver

clean:
	rm -f xorsaver
