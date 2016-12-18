#include <stdio.h>
#include <string.h>
#include <math.h>
#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include "FreeSans_ttf.h"
#include "Button.h"

#define GREY_DARK RGBA8(70, 70, 70, 0xFF)
#define GREY_LIGHT RGBA8(109, 109, 109, 0xFF)
//////

touchPosition touch;
sftd_font *font;

void initialization();
void menu();
void finalization();

int main()
{
	initialization();
	font = sftd_load_font_mem(FreeSans_ttf, FreeSans_ttf_size);

	sf2d_set_clear_color(RGBA8(0x30, 0x00, 0x00, 0xFF));
	sf2d_set_vblank_wait(1);

	menu();

	sftd_free_font(font);
	finalization();
	return 0;
}

void initialization(){
	sf2d_init();
	sftd_init();
	hidInit();

}

void finalization(){
	sftd_fini();
	sf2d_fini();
	hidExit();
}

void menu(){
	u32 boxColor = RGBA8(0, 150, 0, 255);
	u32 textColor = RGBA8(255, 0, 0, 255);
	char buf[20];
	Object topBox(50, 50, 150, 55, 'A');
	Button newConnectionButton(85, 70, 150, 30);
	Button optionButton(85, 120, 150, 30);
	Button aboutButton(85, 170, 150, 30);

	while (aptMainLoop()) {
		//sprintf(buf, "%d", aboutButton.direction(1));
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			topBox.draw(boxColor);
			topBox.printOffset(20, 0, textColor, 20, font);
			sftd_draw_text(font, 10, 10,  RGBA8(0,   255, 0,   255), 20, buf);
		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			newConnectionButton.draw(boxColor);
			optionButton.draw(boxColor);
			aboutButton.draw(boxColor);
		sf2d_end_frame();

		sf2d_swapbuffers();

		hidScanInput();
		hidTouchRead(&touch);
		if(newConnectionButton.clicked())newConnectionButton.setX(10);
		newConnectionButton.change();
		aboutButton.change();
		if (hidKeysDown() & KEY_START) break;
	}

}


