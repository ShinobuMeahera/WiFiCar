#include <sf2d.h>
#include <sftd.h>
#define uint u32
class Object {

	public:
		Object();
		Object(int initX, int initY, int initWidth, int initHeight, char initString);
		Object(int initX, int initY, int initWidth, int initHeight);
		void draw(uint color);
		void print(uint color, uint fontSize, sftd_font *font);
		void printOffset(int xOffset, int yOffset, uint color, uint fontSize, sftd_font *font);
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		void setX(int newX);
		void setY(int newY);
		void setWidth(int newWidth);
		void setHeight(int newHeight);

	private:
		int x;   // Length of object
		int y;  // Breadth of object
		int width; //Width of object
		int height;   // Height of object
		char string; // Text of object
		void init(int initX, int initY, int initWidth, int initHeight, char initString);


};
