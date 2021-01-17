#include "Car.hpp"

/** Nei due costruttori qua sotto si crea lo sprite della macchina e
*   si aggiornano i collider.
*/
Car::Car() : ConsoleSprite(){}

Car::Car(char* directory, int x, int y) : ConsoleSprite(directory,x,y)
{}

/** Metodo che controlla il movimento della macchina.
*   Permette il movimento da tastiera, attraverso diversi keybindings.
*
*   @param hConsole, ovvero il puntatore all'handle dell'stdout,
*   che contiene le informazioni sul framebuffer.
*/

void Car::Movement(HANDLE hConsole){

        deleteSprite(hConsole);

        if (GetAsyncKeyState(VK_UP) && rect_collider.topLine > up_wall) translate(0,-1);
        else if (GetAsyncKeyState(VK_LEFT) && rect_collider.leftLine > left_wall ) translate(-1,0);
        else if (GetAsyncKeyState(VK_RIGHT) && rect_collider.rightLine < right_wall ) translate(1,0);
        else if (GetAsyncKeyState(VK_DOWN) && rect_collider.bottomLine < down_wall) translate(0,1);

        renderSprite(hConsole);

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
