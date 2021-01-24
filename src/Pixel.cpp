#include "Pixel.hpp"

Pixel::Pixel()
{
    position.X = 20;
    position.Y = 20;
    pixChar = ' ';
    color = BLACK_B_BLACK_F;
}

Pixel::Pixel(char pix)
{
    position.X = 0;
    position.Y = 0;
    pixChar = pix;
    color = BLACK_B_BLACK_F;
}



/**
    Renderizza il pixel monstrandolo sullo schermo.

    @params:hConsole, puntatore al framebuffer.
*/
void Pixel::RenderPixel(HANDLE hConsole)
{
    SetConsoleCursorPosition(hConsole, position);
    SetConsoleTextAttribute(hConsole, color);
    cout << pixChar;
}

/**
    Elimina il pixel, rendendolo nero.

    @params:hConsole, puntatore al framebuffer.
            previousCursor, il cursore vecchio del renderer.
*/
void Pixel::BlankPixel(HANDLE hConsole, COORD previousCursor)
{
    SetConsoleCursorPosition(hConsole, previousCursor);
    //set color to black (since the street is black)
    SetConsoleTextAttribute(hConsole, 0x00);
    cout << ' ';
}

/**
    Stampa le coordinate del pixel. Usato per debugging.
*/
void Pixel::printPixelInfo()
{
    cout << position.X << ", " << position.Y << " ";

}


