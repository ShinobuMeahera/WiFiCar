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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "FreeSans_ttf.h"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000
#define GREY_DARK RGBA8(70, 70, 70, 0xFF)
#define GREY_LIGHT RGBA8(109, 109, 109, 0xFF)
//////
static u32 *SOC_buffer = NULL;
s32 sock = -1, csock = -1;

__attribute__((format(printf,1,2)))


const static char http_200[] = "HTTP/1.1 200 OK\r\n";

const static char indexdata[] = "<html> \
                               <head><title>A test page</title></head> \
                               <body> \
                               This small test page has had %d hits. \
                               </body> \
                               </html>";

const static char http_html_hdr[] = "Content-type: text/html\r\n\r\n";
const static char http_get_index[] = "GET / HTTP/1.1\r\n";
//////
typedef struct {
	float x;
	float y;
	float w;
	float h;

} rect;

touchPosition touch;
sftd_font *font;

void initialization();
int isSelected(rect);
int menuSelect(rect, rect, rect);
void newConScreen();
void optionsScreen();
void menuScreen();
void aboutScreen();
void editButton(rect*);
void server();
void failExit(const char *fmt, ...);
void socShutdown();
void finalization();

void func3(){
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
	char wifiStat[4][40] = {
						"signal strength is terrible",
						"signal strength is bad",
						"signal strength is decent",
						"signal strength is good"
		};
	u32  kHeldOld = 0;
	u32 color[32];
	sftd_font *font = sftd_load_font_mem(FreeSans_ttf, FreeSans_ttf_size);
		while (aptMainLoop()) {
			hidScanInput();

			if (hidKeysDown() & KEY_SELECT) break;

			u32 kHeld = hidKeysHeld();
			if ( kHeld != kHeldOld)
		                {

		                int i;
		                sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		                for (i = 0; i < 32; i++)
		                	{
		                	color[i] = RGBA8(0xAF, 0x00, 0x00, 0xFF);

		                	if (kHeld & BIT(i)) {

		                		sftd_draw_text(font, 10, 10+20*i,  RGBA8(0,   255, 0,   255), 20, keysNames[i]);
		                		color[i]= RGBA8(0x00, 0xAF, 0x00, 0xFF);

		                	}
		                	}
		                sf2d_end_frame();
		                sf2d_swapbuffers();
		                }

						kHeldOld = kHeld;

		                sf2d_start_frame(GFX_TOP, GFX_LEFT);
		                    sf2d_draw_rectangle(20, 8, 75, 25, color[9]);
		                    sf2d_draw_rectangle(305, 8, 75, 25, color[8]);
		                    sf2d_draw_fill_circle(320,125,13,color[11]);
		                    sf2d_draw_fill_circle(370,125,13,color[0]);
		                    sf2d_draw_fill_circle(345,100,13,color[10]);
		                    sf2d_draw_fill_circle(345,150,13,color[1]);
		                    sf2d_draw_rectangle(40, 160, 40, 15, color[5]);
		                    sf2d_draw_rectangle(85, 160, 40, 15, color[4]);
		                    sf2d_draw_rectangle(75, 130, 15, 40, color[6]);
		                    sf2d_draw_rectangle(75, 170, 15, 40, color[7]);
		                    sftd_draw_text(font, 100, 100,  RGBA8(255-(85*osGetWifiStrength()), 85*osGetWifiStrength(),   0,   255), 15, wifiStat[osGetWifiStrength()]);
		                sf2d_end_frame();


		               sf2d_swapbuffers();
		}
		sftd_free_font(font);
}

int main()
{
	initialization();
	font = sftd_load_font_mem(FreeSans_ttf, FreeSans_ttf_size);

	sf2d_set_clear_color(RGBA8(0x30, 0x00, 0x00, 0xFF));
	sf2d_set_vblank_wait(1);

	menuScreen();


	sftd_free_font(font);
	finalization();
	return 0;
}

void initialization(){
	sf2d_init();
	sftd_init();
	hidInit();

};

void finalization(){
	sftd_fini();
	sf2d_fini();
	hidExit();
};

void newConScreen(){
	rect ipBox,acceptButton;
	bool ok = false;

	static SwkbdState swkbd;
	static char mybuf[20];
	static SwkbdStatusData swkbdStatus;
	static SwkbdLearningData swkbdLearning;
	SwkbdButton button = SWKBD_BUTTON_NONE;

	ipBox.x = 60;
	ipBox.y = 100;
	ipBox.w = 180;
	ipBox.h = 50;

	acceptButton.x = 180;
	acceptButton.y = 180;
	acceptButton.w = 60;
	acceptButton.h = 50;

	u32 ipColor = RGBA8(255, 0, 0, 255);

	while (aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&touch);

		if(isSelected(ipBox)){
			swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 16);
			swkbdSetInitialText(&swkbd, mybuf);
			swkbdSetPasswordMode(&swkbd, SWKBD_PASSWORD_NONE);
			swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
			swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
			swkbdSetNumpadKeys(&swkbd, L';', L'.');
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
			ok=true;
		};

		if(ok && isSelected(acceptButton))
			server();

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sftd_draw_text(font, 80, 140,  RGBA8(0, 0, 255, 255), 40, mybuf);
		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_rectangle(ipBox.x, ipBox.y, ipBox.w, ipBox.h, ipColor);
			sftd_draw_text(font, ipBox.x, ipBox.y,  RGBA8(0, 0, 255, 255), 20, "Enter Ip Address");
			sf2d_draw_rectangle(acceptButton.x, acceptButton.y, acceptButton.w, acceptButton.h, ipColor);
			sftd_draw_text(font, acceptButton.x, acceptButton.y,  RGBA8(0, 0, 255, 255), 20, "Accept");
		sf2d_end_frame();
		sf2d_swapbuffers();

		if (hidKeysDown() & KEY_B) break;

	};

};
void menuScreen(){
	rect newCon, options, about;
	u32 boxColor = RGBA8(0, 150, 0, 255);
	u32 textColor = RGBA8(255, 0, 0, 255);
	newCon.x = 85;
	newCon.y = 70;
	newCon.w = 150;
	newCon.h = 30;

	options.x = 85;
	options.y = 120;
	options.w = 150;
	options.h = 30;

	about.x = 85;
	about.y = 170;
	about.w = 150;
	about.h = 30;

	while (aptMainLoop()) {
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_rectangle(50, 50, 250,55, RGBA8(0,   255, 0,   255));
			sftd_draw_text(font, 80, 50,  RGBA8(255,   0, 0,   255), 40, "Nintendo Car");
		sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_rectangle(newCon.x, newCon.y, newCon.w, newCon.h, boxColor);
			sf2d_draw_rectangle(options.x, options.y, options.w, options.h, boxColor);
			sf2d_draw_rectangle(about.x, about.y, about.w, about.h, boxColor);
			sftd_draw_text(font, newCon.x+20, newCon.y, textColor, 20, "New Connection");
			sftd_draw_text(font, options.x+20, options.y, textColor, 20, "Options");
			sftd_draw_text(font, about.x+20, about.y, textColor, 20, "About");
		sf2d_end_frame();
		sf2d_swapbuffers();

		hidScanInput();
		hidTouchRead(&touch);

		switch(menuSelect(newCon, options, about))
		{
			case 1: newConScreen();
				break;
			case 2: optionScreen();
				break;
			case 3: aboutScreen();
				break;
			case 0: break;
		};

		if (hidKeysDown() & KEY_START) break;

	};

};

void optionScreen(){

	rect BGR, BGL, splitBar, buttonLeft;

	BGR.x = 96;
	BGR.y = 0;
	BGR.w = 224;
	BGR.h = 240;

	BGL.x = 0;
	BGL.y = 0;
	BGL.w = 94;
	BGL.h = 240;

	splitBar.x = 93;
	splitBar.y = 0;
	splitBar.w = 5;
	splitBar.h = 240;

	buttonLeft.x = 20;
	buttonLeft.y = 20;
	buttonLeft.w = 90;
	buttonLeft.h = 20;



	while(aptMainLoop()){

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sftd_draw_text(font, 20, 40,  RGBA8(0, 0, 255, 255), 40, "Options");
		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_rectangle(BGR.x, BGR.y, BGR.w, BGR.h, GREY_DARK);
			sf2d_draw_rectangle(BGL.x, BGL.y, BGL.w, BGL.h, GREY_DARK);
			sf2d_draw_rectangle(splitBar.x, splitBar.y, splitBar.w, splitBar.h, RGBA8(0, 54, 99, 0XFF));
			sf2d_draw_rectangle(buttonLeft.x, buttonLeft.y, buttonLeft.w, buttonLeft.h, RGBA8(50, 54, 40, 0XFF));

			sftd_draw_text(font, 100, 200,  RGBA8(255, 255, 255, 255), 20, "Press B if you want exit this screen.");
		sf2d_end_frame();

		sf2d_swapbuffers();

		hidScanInput();
		hidTouchRead(&touch);

		if(isSelected(buttonLeft))
			editButton(&buttonLeft);

		if (hidKeysDown() & KEY_B) break;
	}
};

void editButton(rect *obj){
	if(obj != NULL){
		obj->x = touch.px;
		obj->y = touch.py;
	};
};

void aboutScreen(){
	while(aptMainLoop()){
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sftd_draw_text(font, 20, 40,  RGBA8(0, 0, 255, 255), 40, "Created By:");
			sftd_draw_text(font, 80, 100,  RGBA8(0, 0, 255, 255), 40, "Michal Anyszka");
			sftd_draw_text(font, 80, 140,  RGBA8(0, 0, 255, 255), 40, "Przemyslaw Debiec");
		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sftd_draw_text(font, 100, 200,  RGBA8(255, 255, 255, 255), 20, "Press B if you want exit this screen.");
		sf2d_end_frame();

		sf2d_swapbuffers();

		hidScanInput();
		if (hidKeysDown() & KEY_B) break;
		};
};
void server(){
	int ret;

		u32	clientlen;
		struct sockaddr_in client;
		struct sockaddr_in server;
		char temp[1026];
		static int hits=0;

		gfxInitDefault();

		// register gfxExit to be run when app quits
		// this can help simplify error handling
		atexit(gfxExit);

		consoleInit(GFX_TOP, NULL);

		printf ("\nlibctru sockets demo\n");

		// allocate buffer for SOC service
		SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

		if(SOC_buffer == NULL) {
			failExit("memalign: failed to allocate\n");
		}

		// Now intialise soc:u service
		if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
	    	failExit("socInit: 0x%08X\n", (unsigned int)ret);
		}

		// register socShutdown to run at exit
		// atexit functions execute in reverse order so this runs before gfxExit
		atexit(socShutdown);

		// libctru provides BSD sockets so most code from here is standard
		clientlen = sizeof(client);

		sock = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

		if (sock < 0) {
			failExit("socket: %d %s\n", errno, strerror(errno));
		}

		memset (&server, 0, sizeof (server));
		memset (&client, 0, sizeof (client));

		server.sin_family = AF_INET;
		server.sin_port = htons (80);
		server.sin_addr.s_addr = gethostid();

		printf("Point your browser to http://%s/\n",inet_ntoa(server.sin_addr));

		if ( (ret = bind (sock, (struct sockaddr *) &server, sizeof (server))) ) {
			close(sock);
			failExit("bind: %d %s\n", errno, strerror(errno));
		}

		// Set socket non blocking so we can still read input to exit
		fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

		if ( (ret = listen( sock, 5)) ) {
			failExit("listen: %d %s\n", errno, strerror(errno));
		}

		while (aptMainLoop()) {
			gspWaitForVBlank();
			hidScanInput();

			csock = accept (sock, (struct sockaddr *) &client, &clientlen);

			if (csock<0) {
				if(errno != EAGAIN) {
					failExit("accept: %d %s\n", errno, strerror(errno));
				}
			} else {
				// set client socket to blocking to simplify sending data back
				fcntl(csock, F_SETFL, fcntl(csock, F_GETFL, 0) & ~O_NONBLOCK);
				printf("Connecting port %d from %s\n", client.sin_port, inet_ntoa(client.sin_addr));
				memset (temp, 0, 1026);

				ret = recv (csock, temp, 1024, 0);

				if ( !strncmp( temp, http_get_index, strlen(http_get_index) ) ) {
					hits++;

					send(csock, http_200, strlen(http_200),0);
					send(csock, http_html_hdr, strlen(http_html_hdr),0);
					sprintf(temp, indexdata, hits);
					send(csock, temp, strlen(temp),0);
				}

				close (csock);
				csock = -1;

			}

			u32 kDown = hidKeysDown();
			if (kDown & KEY_START) break;
		}

		close(sock);

		return 0;
}

int menuSelect(rect button1, rect button2, rect button3){
	if(isSelected(button1))
		return 1;
	if(isSelected(button2))
		return 2;
	if(isSelected(button3))
		return 3;

	return 0;
};

int isSelected(rect button){
	if (hidKeysDown() & BIT(20))
		if((touch.px<(button.x + button.w)) && (touch.px>button.x))
			if(touch.py<(button.y + button.h) && (touch.py>button.y)){
				return 1;
			}
	return 0;
};

void socShutdown() {
//---------------------------------------------------------------------------------
	printf("waiting for socExit...\n");
	socExit();

};

void failExit(const char *fmt, ...) {
//---------------------------------------------------------------------------------

	if(sock>0) close(sock);
	if(csock>0) close(csock);

	va_list ap;

	printf(CONSOLE_RED);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(CONSOLE_RESET);
	printf("\nPress B to exit\n");

	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_B) exit(0);
	}
};
