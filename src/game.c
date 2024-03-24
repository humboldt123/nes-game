#include <neslib/neslib.h>
#include <stdint.h>

#pragma bss-name(push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name(pop)


#define WIDTH 256
#define HEIGHT 240

static unsigned char MOVEMENT_SPEED;

// vars
static unsigned char pad;
static unsigned char spr;
static unsigned char frame;
static unsigned char emotion;
static unsigned char flipped;

static unsigned char x;
static unsigned char y;
static unsigned char moving;

static unsigned char u,v;


unsigned char i; // loop

// form feed is 0x0C which is a heart in this palette
unsigned char text[] = {
	// screen is 32 x 30 tiles
	MSB(NTADR_A(9,14))|NT_UPD_HORZ, // where to write, repeat horizontally
	LSB(NTADR_A(9,14)),
	15, // length
	'v','i','s','h',' ','w','a','s',' ','h','e','r','e',' ','\f',
	NT_UPD_EOF // must end in eof
};

// this is just the default ghost, not like a source of truth
unsigned char ghost[] = {
	//head
	0, 0, 0x80, 0, 
	8, 0, 0x81, 0,
	
	//body
	0, 8, 0x90, 0,
	8, 8, 0x91, 0, 
	128 // terminate
};

unsigned char block[] = {
	//head
	0, 0, 0x41, 0, 
	8, 0, 0x42, 0,
	
	//body
	0, 8, 0x51, 0,
	8, 8, 0x52, 0, 
	128 // terminate
};

const unsigned char palette[] = { 0x0F, 0x06, 0x15, 0x36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// this is so scuffed what am i doing here
const unsigned char level[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void main (void) {
	ppu_on_all(); // turn on the screen (activate ppu)
	pal_bg(palette);
	
	MOVEMENT_SPEED = 1;
	
	frame = 0;

	x = 52;
	y = 180;

	set_vram_update(text);

	ppu_wait_nmi(); // we will be writing on the next frame 


	while (1) {
		ppu_wait_frame();
		pal_spr(palette);

		// controls
		moving = 0;
		pad = pad_poll(i);
		if (pad & PAD_LEFT) {
			x -= MOVEMENT_SPEED;
			moving = 1;
			flipped = 1;
		}
		if (pad & PAD_RIGHT) {
			x += MOVEMENT_SPEED;
			moving = 1;
			flipped = 0;
		}
		if (pad & PAD_UP) {
			y -= MOVEMENT_SPEED;
			moving = 1;
		}
		if (pad & PAD_DOWN) {
			y += MOVEMENT_SPEED;
			moving = 1;
		}

		if (pad & PAD_A) {
			++emotion;
		}

		// update sprite
		ghost[flipped ? 6 : 2] = 0x80 + (emotion % 3 * 2);
		ghost[flipped ? 2 : 6] = 0x81 + (emotion % 3 * 2);
		ghost[flipped ? 14 : 10] = 0x90;
		ghost[flipped ? 10 : 14] = 0x91;
		if (moving == 1) {
			ghost[flipped ? 14 : 10] += (frame / 12 % 4 * 2);
			ghost[flipped ? 10 : 14] += (frame / 12 % 4 * 2);
		} else {
			frame = 0;
		}

		// 64 is flip attr.
		if (flipped == 1) {
			ghost[3] = 64;
			ghost[7] = 64;
			ghost[11] = 64;
			ghost[15] = 64;
			
		} else {
			ghost[3] = 0;
			ghost[7] = 0;
			ghost[11] = 0;
			ghost[15] = 0;
		}




		// render
		// items render in the opposite order you would expect them to. 

		spr = 0; // why am *i* charged with keeping track of the sprID :(
		
	
		// render ghost
		spr = oam_meta_spr(x, y, spr, ghost);


		//render bg
		for (u = 0; u < sizeof(level); ++u) {
			v = level[u]; // address tile
			if (v == 1) {
				spr = oam_meta_spr((u % 16)*16, (u / 16)*16, spr, block); // WAIT NO THE NES SPRITE LIMIT IS 8 IM GONNA CRY
			}
		}


		++frame;
	}
}
