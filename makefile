CC = gcc
CFLAGS = -std=c99 -Isrc -Ilibs/lua/include -Ilibs/sdl2/include -Iextensions/pv8sdk.lua -Iextensions/pv8sdk.sdl2 -O2

src = $(wildcard src/*.c) \
	$(wildcard extensions/pv8sdk.lua/*c) \
	$(wildcard extensions/pv8sdk.sdl2/*c)
		
obj = $(src:.c=.o)

demossrc = $(wildcard demos/*.c)
demosobj = $(demossrc:.c=.o)

sdlrunner: $(demosobj) pv8sdk
	$(CC) $(CFLAGS) -o $@ $^ libs/lua/lib/rpi/liblua.a `sdl2-config --cflags --libs` -lm -ldl

pv8sdk: $(obj)
	ar rcs $@ $^  

.PHONY: clean
clean:
	rm -f pv8sdk $(obj)
