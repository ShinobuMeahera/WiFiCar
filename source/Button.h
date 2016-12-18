#include "Object.h"
#define uint u32
class Button: public Object {

	public:
		Button(int initX, int initY, int initWidth, int initHeight, char initString);
		Button(int initX, int initY, int initWidth, int initHeight);
		Button();
		bool clicked();
		void change();
		void touchControll();
		void saveLast();
		void draw(uint color);


	private:
		touchPosition touch;
		touchPosition touchOld;
		bool active;
		bool editMode;
		void init(int initX, int initY, int initWidth, int initHeight, char initString);
};




