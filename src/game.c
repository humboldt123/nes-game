#include <neslib/neslib.h>
#include <stdint.h>

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name (pop)

// form feed is 0x0C which is a heart in this palette
unsigned char text[] = "vish was here \f";
const uint8_t palette[] = { 0x0F, 0x06, 0x15, 0x36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void main (void) {
	// screen off
	ppu_off();
	pal_bg(palette);

	// screen is 32 x 30 tiles
	vram_adr(NTADR_A(10,14)); 
	
	// Write text to the starting point
	vram_write(text, sizeof(text));

	// Activate PPU
	ppu_on_all();

	while (1) {
		/* Game Loop */
	}
}
