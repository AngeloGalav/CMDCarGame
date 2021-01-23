#include "Pixel.hpp"

Pixel::Pixel(char pix)
{
    pixChar = pix;
    position.X = 0;
    position.Y = 0;
}

Pixel::Pixel()
{
    position.X = 20;
    position.Y = 20;
    pixChar = ' ';
}

//renderizza il pixel (lo mostra su schermo)
void Pixel::RenderPixel(HANDLE hConsole)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    SetConsoleTextAttribute(hConsole, color);
    cout << pixChar;
}

//function for deleting the pixel (essentially it sets the pixel to black)
void Pixel::BlankPixel(HANDLE hConsole, COORD previousCursor)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), previousCursor);
    //set color to black (since the street is black)
    SetConsoleTextAttribute(hConsole, 0x00);
    cout << ' ';
}

//debugInfo
void Pixel::printPixelInfo()
{
    cout << position.X << ", " << position.Y << " ";

}


