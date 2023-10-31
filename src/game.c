#include <neslib/neslib.h>
#include <stdint.h>

#pragma bss-name(push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name(pop)

// form feed is 0x0C which is a heart in this palette
unsigned char text[] = {
	// screen is 32 x 30 tiles
	MSB(NTADR_A(9,14))|NT_UPD_HORZ, // where to write, repeat horizontally
	LSB(NTADR_A(9,14)),
	15, // length
	'v','i','s','h',' ','w','a','s',' ','h','e','r','e',' ','\f',
	NT_UPD_EOF // must end in eof
};
const uint8_t palette[] = { 0x0F, 0x06, 0x15, 0x36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// loop variable
unsigned char i;

void main (void) {
	// screen off
	ppu_off();
	pal_bg(palette);

	ppu_on_all(); // turn on the screen (activate ppu)
	
	set_vram_update(text);

	ppu_wait_nmi(); // we will be writing on the next frame 

	set_vram_update(NULL); // stop writing it over and over

	while (1) {
		// write text to the starting point
		
		
	}
}
