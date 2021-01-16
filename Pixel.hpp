#ifndef PIXEL_HPP
#define PIXEL_HPP
#include <conio.h>
#include <windows.h>
#include <iostream>
#define BACKGROUND_WHITE 119

using namespace std;

/**
    TODO: GET-SET CHAR PIXCHAR
*/



class Pixel
{
    protected:

    public:
        //constructors
        Pixel(char pix);
        Pixel();

        //the character that represents the pixel
        char pixChar;

        WORD color;
        COORD position;  //the position of the pixel is relative to the screen.

        //rendering functions
        void RenderPixel(HANDLE hConsole);
        void BlankPixel(HANDLE hConsole, COORD previousCursor);


        //setter and getter
        void setX(SHORT x);
        void setY(SHORT y);

        SHORT getPosition();
        void setPosition(COORD pos);

        void setColor();
        void setPixChar(WORD _color);

        //debug functions
        void printPixelInfo();
};

#endif // PIXEL_HPP
