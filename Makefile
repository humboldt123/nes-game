SRC 	:= game.c
SRCOUT 	:= game.s
SRCOBJ 	:= game.o
OUT 	:= game.nes

.PHONY: all build clean

all: build
build: src/$(SRC)
	mkdir -p build
	cc65 -Ors -o build/$(SRCOUT) src/$(SRC) --add-source -I lib/
	ca65 lib/neslib/crt0.s -o build/crt0.o
	ca65 build/$(SRCOUT) -g -o build/$(SRCOBJ)
	ld65 -C lib/nrom.cfg -o build/$(OUT) build/crt0.o build/$(SRCOBJ) nes.lib -Ln lib/debug.txt
clean:
	rm -rf build
