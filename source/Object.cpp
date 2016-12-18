#include "Object.h"
////////////////////////////////////////////////////////////////////////////////////////
//Constructors
////////////////////////////////////////////////////////////////////////////////////////
		Object::Object(){
			init(0, 0, 10, 10, 'a');
		}
		Object::Object(int initX, int initY, int initWidth, int initHeight){
			init(initX, initY, initWidth, initHeight, 'a');
		}
		Object::Object(int initX, int initY, int initWidth, int initHeight, char initString){
			init(initX, initY, initWidth, initHeight, initString);
		}
////////////////////////////////////////////////////////////////////////////////////////
//Misc Functions
////////////////////////////////////////////////////////////////////////////////////////
		void Object::draw(uint color){
			sf2d_draw_rectangle(x, y, width, height, color);
		}
		void Object::print(uint color, uint fontSize, sftd_font *font){
			printOffset(0, 0, color, fontSize, font);
		}
		void Object::printOffset(int xOffset, int yOffset, uint color, uint fontSize, sftd_font *font){
			sftd_draw_text(font, (x+xOffset), (y+yOffset), color, fontSize, "d");
		}
		void Object::init(int initX, int initY, int initWidth, int initHeight, char initString){
			x = initX;
			y = initY;
			width = initWidth;
			height = initHeight;
			string = initString;
		};
////////////////////////////////////////////////////////////////////////////////////////
//Get Functions
////////////////////////////////////////////////////////////////////////////////////////
		int Object::getX(){
			return x;
		}
		int Object::getY(){
			return y;
		}
		int Object::getWidth(){
			return width;
		}
		int Object::getHeight(){
			return height;
		}
		//char getText(){
		//	return string; do poprawy
		//}
////////////////////////////////////////////////////////////////////////////////////////
//Set Functions
////////////////////////////////////////////////////////////////////////////////////////
		void Object::setX(int newX){
			x = newX;
		}
		void Object::setY(int newY){
			y = newY;
		}
		void Object::setWidth(int newWidth){
			width = newWidth;
		}
		void Object::setHeight(int newHeight){
			height = newHeight;
		}
		//void setText(){
		//
		//}



