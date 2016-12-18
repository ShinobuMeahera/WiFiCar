#include <3ds.h>
#include "Button.h"
#define BX 10
#define BY 10
////////////////////////////////////////////////////////////////////////////////////////
//Constructors
////////////////////////////////////////////////////////////////////////////////////////
Button::Button(int initX, int initY, int initWidth, int initHeight, char initString){
	init(initX, initY, initWidth, initHeight, initString);
}
Button::Button(int initX, int initY, int initWidth, int initHeight){
	init(initX, initY, initWidth, initHeight, 'a');
}
Button::Button(){
	init(0, 0, 10, 10, 'a');
}
////////////////////////////////////////////////////////////////////////////////////////
//Misc Functions
////////////////////////////////////////////////////////////////////////////////////////
bool Button::clicked(){

hidTouchRead(&touch);

	if (hidKeysDown() & BIT(20))
			if((touch.px<(getX() + getWidth())) && (touch.px>getX()))
				if(touch.py<(getY() + getHeight()) && (touch.py>getY())){
					return true;
				}
		return false;
}
void Button::change(){
	editMode = true;
	saveLast();
	hidTouchRead(&touch);

	touchControll();
	if(active){
		setX(touch.px - (BX/2) );
		setY(touch.py - (BX/2) );
	}

	//editMode = false; jak wyjsc z editmode

}
void Button::init(int initX, int initY, int initWidth, int initHeight, char initString){
	setX(initX);
	setY(initY);
	setWidth(initWidth);
	setHeight(initHeight);
}
void Button::saveLast(){
	touchOld = touch;
}
void Button::draw(uint color){
	sf2d_draw_rectangle(getX(), getY(), getWidth(), getHeight(), color);
	if(editMode){
		sf2d_draw_rectangle(getX(), getY(), BX, BY, RGBA8(0x7B, 0x00, 0x00, 0xFF));
		sf2d_draw_rectangle((getX() + getWidth()) - BX, (getY() + getHeight()) - BY , BX, BY, RGBA8(0x00, 0x00, 0x7B, 0xFF));
	}
}
void Button::touchControll(){

	if(hidKeysDown() & BIT(20)){
		if((touch.px<(getX() + BX)) && (touch.px>getX())){
			if(touch.py<(getY() + BY) && (touch.py>getY())){
				active = true;
			}
		}
	}

	if(hidKeysUp() & BIT(20)){
		active = false;
	}
}
