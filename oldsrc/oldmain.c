#include <string.h>

#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include "FreeSans_ttf.h"



/*
int main()
{
	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};

	gfxInitDefault();
	//gfxSet3D(true); // uncomment if using stereoscopic 3D
	consoleInit(GFX_TOP, NULL);
	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0;
	printf("\x1b[0;0HPress Start to exit.");
	printf("\x1b[1;0HCirclePad position:");
	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		// Your code goes hereb

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		u32 kUp = hidKeysUp();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Example rendering code that displays a white pixel
		// Please note that the 3DS screens are sideways (thus 240x400 and 240x320)
		//u8* fbl = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		//u8* fbr = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		//memset(fbl, 0, 240*400*3);
		//memset(fbr, 0, 240*400*3);
		//fbl[3*(390*240)] = 0xFF;
		//fbl[3*(399*240)+1] = 0xFF;
		//fbl[3*(10+399*240)+2] = 0xFF;
		//fbr[3*(10*240)] = 0xFF;
		//fbr[3*(0*240)+1] = 0xFF;
		//fbr[3*(10+0*240)+2] = 0xFF;

		if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
				{
					//Clear console
					consoleClear();

					//These two lines must be rewritten because we cleared the whole console
					printf("\x1b[0;0HPress Start to exit.");
					printf("\x1b[1;0HCirclePad position:");

					printf("\x1b[3;0H"); //Move the cursor to the fourth row because on the third one we'll write the circle pad position

					//Check if some of the keys are down, held or up
					int i;
					for (i = 0; i < 32; i++)
					{
						if (kDown & BIT(i)) printf("%s down\n", keysNames[i]);
						if (kHeld & BIT(i)) printf("%s held\n", keysNames[i]);
						if (kUp & BIT(i)) printf("%s up\n", keysNames[i]);
					}
				}

				//Set keys old values for the next frame
				kDownOld = kDown;
				kHeldOld = kHeld;
				kUpOld = kUp;

				circlePosition pos;

				//Read the CirclePad position
				hidCircleRead(&pos);

				//Print the CirclePad position
				printf("\x1b[2;0H%04d; %04d", pos.dx, pos.dy);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	gfxExit();
	return 0;
}
*/
void button_check(sftd_font *font){
	u32 color[32];
	float a;
	char keysNames[32][32] = {
			"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
			"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
			"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
			"", "", "KEY_ZL", "KEY_ZR",
			"", "", "", "",
			"KEY_TOUCH", "", "", "",
			"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
			"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
		};
	u32  kHeldOld = 0;
	while (aptMainLoop()) {
		hidScanInput();



		if (hidKeysDown() & KEY_SELECT) break;
		u32 kHeld = hidKeysHeld();
		if ( kHeld != kHeldOld)
	                {
					//consoleClear();
	                //Check if some of the keys are down, held or up
	                int i;
	                for (i = 0; i < 32; i++)
	                	{
	                	color[i] = RGBA8(0xAF, 0x00, 0x00, 0xFF);

	                	if (kHeld & BIT(i)) {

	                		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

	                					sftd_draw_text(font, 10, 10,  RGBA8(0,   255, 0,   255), 20, keysNames[i]);

	                				sf2d_end_frame();
	                		color[i]= RGBA8(0x00, 0xAF, 0x00, 0xFF);

	                	}

	                	}
	                }

					kHeldOld = kHeld;

	                sf2d_start_frame(GFX_TOP, GFX_LEFT);
	                    //Draws a 100x100 yellow rectangle (255, 255, 00, 255) at (150, 70)
	                    sf2d_draw_rectangle(20-a, 8, 75, 25, color[9]);
	                    sf2d_draw_rectangle(305-a, 8, 75, 25, color[8]);
	                    sf2d_draw_fill_circle(320-a,125,13,color[11]);
	                    sf2d_draw_fill_circle(370-a,125,13,color[0]);
	                    sf2d_draw_fill_circle(345-a,100,13,color[10]);
	                    sf2d_draw_fill_circle(345-a,150,13,color[1]);
	                    sf2d_draw_rectangle(40-a, 160, 40, 15, color[5]);
	                    sf2d_draw_rectangle(85-a, 160, 40, 15, color[4]);
	                    sf2d_draw_rectangle(75-a, 130, 15, 40, color[6]);
	                    sf2d_draw_rectangle(75-a, 170, 15, 40, color[7]);
	                sf2d_end_frame();
	                a = (5*osGet3DSliderState());
	                sf2d_start_frame(GFX_TOP, GFX_RIGHT);
	                	                    //Draws a 100x100 yellow rectangle (255, 255, 00, 255) at (150, 70)
	                	                    sf2d_draw_rectangle(20+a, 8, 75, 25, color[9]);
	                	                    sf2d_draw_rectangle(305+a, 8, 75, 25, color[8]);
	                	                    sf2d_draw_fill_circle(320+a,125,13,color[11]);
	                	                    sf2d_draw_fill_circle(370+a,125,13,color[0]);
	                	                    sf2d_draw_fill_circle(345+a,100,13,color[10]);
	                	                    sf2d_draw_fill_circle(345+a,150,13,color[1]);
	                	                    sf2d_draw_rectangle(40+a, 160, 40, 15, color[5]);
	                	                    sf2d_draw_rectangle(85+a, 160, 40, 15, color[4]);
	                	                    sf2d_draw_rectangle(75+a, 130, 15, 40, color[6]);
	                	                    sf2d_draw_rectangle(75+a, 170, 15, 40, color[7]);
	               sf2d_end_frame();

	                //sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	                 //   //Draws a 70x100 blue rectangle (0, 0, 00, 255) at (120, 30)
	                //    sf2d_draw_rectangle(120, 30, 70, 100, RGBA8(0x00, 0x00, 0xFF, 0xFF));
	               // sf2d_end_frame();

	                sf2d_swapbuffers();
	}




}
int main()
{
	sf2d_init();
	sf2d_set_3D(1);


    //consoleInit(GFX_BOTTOM, NULL);
    sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0x00));
	sftd_init();
	sftd_font *font = sftd_load_font_mem(FreeSans_ttf, FreeSans_ttf_size);
	int textWidth = 0;
	int textHeight = 0;
	const char someText[] = "Wal sie v";

    while (aptMainLoop()) {

    	//printf("\x1b[0;0HPress Start to exit.");
    	//printf("\x1b[1;0HSelect Option:");
    	//printf("\x1b[3;0HA: Button Test.");
    	//printf("\x1b[5;0HWifi: %X" ,osGetWifiStrength());
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);

			//sftd_draw_textf(font, 10, 10, RGBA8(0, 255, 0, 255), 20, "FPS %f", sf2d_get_fps());
			///sftd_calc_bounding_box(&textWidth, &textHeight, font, 20, 300, someText);
			//sf2d_draw_rectangle(10, 40, textWidth, textHeight, RGBA8(0, 100, 0, 255));
			//sftd_draw_text_wrap(font, 10, 40,  RGBA8(255, 255, 255, 255), 20, 300, someText);
		sf2d_draw_rectangle(40, 160, 40, 15,RGBA8(255,255,0,255));
		sf2d_end_frame();



		//sf2d_swapbuffers();
        hidScanInput();
        if (hidKeysDown() & KEY_A) {
        	consoleClear();
        	button_check(font);
        }
        if (hidKeysDown() & KEY_START) break;



    }
    sftd_free_font(font);
    sftd_fini();
    sf2d_fini();
    return 0;
}

