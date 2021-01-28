#include "Car.hpp"

/** Nei due costruttori qua sotto si crea lo sprite della macchina e
*   si aggiornano i collider.
*/
Car::Car() : ConsoleSprite()
{
    right_wall = 0;
    left_wall = 0;
    up_wall = 0;
    down_wall = 0;
}

Car::Car(char* directory, int x, int y, HANDLE thConsole) : ConsoleSprite(directory,x,y, thConsole)
{
    right_wall = 0;
    left_wall = 0;
    up_wall = 0;
    down_wall = 0;
}


/** Metodo che controlla il movimento della macchina.
*   Permette il movimento da tastiera, attraverso diversi keybindings.
*/
void Car::optimized_Movement()
{
    int x = 0, y = 0;

    bool hasMoved = true;
    if (GetAsyncKeyState(VK_UP) && rect_collider.topLine > up_wall) {x = 0; y = -1;}
    else if (GetAsyncKeyState(VK_LEFT) && rect_collider.leftLine > left_wall ) {x = -1; y = 0;}
    else if (GetAsyncKeyState(VK_RIGHT) && rect_collider.rightLine < right_wall ) {x = 1; y = 0;}
    else if (GetAsyncKeyState(VK_DOWN) && rect_collider.bottomLine < down_wall) {x = 0; y = 1;}
    else hasMoved = false;

    if (hasMoved)
    {
        deleteSprite();
        translate(x, y);
        renderSprite();
    }
}

/**
*   Setta i limiti di movimento della macchina nel livello.
*
*   @params: coordinate dei 4 "muri" che segnano i limiti.
*/
void Car::setBoundaries(int leftWall, int rightWall, int upWall, int downWall)
{
    right_wall = rightWall;
    left_wall = leftWall;
    up_wall = upWall;
    down_wall = downWall;
}
