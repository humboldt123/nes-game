#include <neslib/neslib.h>
#include <stdint.h>

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name (pop)

const uint8_t text[] = "Hello Boilerplate";
const uint8_t palette[] = { 0x02, 0x11, 0x14, 0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void screenSetup(void) {
	/* Turn of the screen */
	ppu_off();

	/* Setup the palette */
	pal_bg(palette);
}

// From https://github.com/themangomago/nes-boilerplate
void main (void) {
	/* Setup Screen */
	screenSetup();

	vram_adr(NTADR_A(7,14)); /* Starting point */
	vram_write(text, sizeof(text)); /* Write text to the starting point */

	ppu_on_all(); /* Activate PPU */

	while (1) {
		/* Game Loop */
	}
}
