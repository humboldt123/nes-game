if exist build (RD /S /Q build)
mkdir build

cc65 -Ors -o build/game.s src/game.c --add-source -I lib/
ca65 lib/neslib/crt0.s -o build/crt0.o
ca65 build/game.s -g -o build/game.o
ld65 -C lib/nrom.cfg -o build/game.nes build/crt0.o build/game.o nes.lib -Ln build/debug.txt