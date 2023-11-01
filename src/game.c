#include <neslib/neslib.h>
#include <stdint.h>

#pragma bss-name(push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name(pop)

// vars
static unsigned char pad;
static unsigned char spr;

static unsigned char x;
static unsigned char y;

// form feed is 0x0C which is a heart in this palette
unsigned char text[] = {
	// screen is 32 x 30 tiles
	MSB(NTADR_A(9,14))|NT_UPD_HORZ, // where to write, repeat horizontally
	LSB(NTADR_A(9,14)),
	15, // length
	'v','i','s','h',' ','w','a','s',' ','h','e','r','e',' ','\f',
	NT_UPD_EOF // must end in eof
};

const unsigned char guy[] = {
	0, 0, 0x80, 0,
	8, 0, 0x81, 0,
	0, 8, 0x90, 0,
	8, 8, 0x91, 0, 
	128 // terminate
};

const uint8_t palette[] = { 0x0F, 0x06, 0x15, 0x36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// loop variable
unsigned char i;

void main (void) {
	// screen off
	ppu_off();
	pal_bg(palette);

	ppu_on_all(); // turn on the screen (activate ppu)

	x = 52;
	y = 180;

	set_vram_update(text);

	ppu_wait_nmi(); // we will be writing on the next frame 

	set_vram_update(NULL); // stop writing it over and over

	while (1) {
		ppu_wait_frame();
		pal_spr(palette);

		// render sprite
		spr = 0;
		spr = oam_meta_spr(x, y, spr, guy);
		spr = oam_meta_spr(x+30, y+30, spr, guy);
	
		// controls
		pad = pad_poll(i);
		if (pad & PAD_LEFT && x > 0) {
			x -= 2;
		}
		if (pad & PAD_RIGHT && x < 232) {
			x += 2;
		}
		if (pad & PAD_UP && y > 0) {
			y -= 2;
		}
		if (pad & PAD_DOWN && y < 212) {
			y += 2;
		}
		
	}
}
