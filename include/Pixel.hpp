#ifndef PIXEL_HPP
#define PIXEL_HPP
#include "Definitions.hpp"
#include <windows.h>
#include <iostream>


using namespace std;

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
