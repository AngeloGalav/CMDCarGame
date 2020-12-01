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

        //debug functions
        void printPixelInfo();
};

#endif // PIXEL_HPP
