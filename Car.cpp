#include "Car.hpp"

/** Nei due costruttori qua sotto si crea lo sprite della macchina e
*   si aggiornano i collider.
*/
Car::Car()
{
    car_sprite = ConsoleSprite();
    updateCollider();  //TODO: Controllare se ha senso fare l'update dei collider
}

Car::Car(char* directory, int x, int y)
{
    car_sprite = ConsoleSprite(directory,x,y);
    updateCollider();
}

/** Metodo che controlla il movimento della macchina.
*   Permette il movimento da tastiera, attraverso diversi keybindings.
*
*   @param hConsole, ovvero il puntatore all'handle dell'stdout,
*   che contiene le informazioni sul framebuffer.
*/

void Car::Movement(HANDLE hConsole){

        car_sprite.deleteSprite(hConsole);

        if (GetAsyncKeyState(VK_UP) && collider.topLine > up_wall) car_sprite.translate(0,-1);
        else if (GetAsyncKeyState(VK_LEFT) && collider.leftLine > left_wall ) car_sprite.translate(-1,0);
        else if (GetAsyncKeyState(VK_RIGHT) && collider.rightLine < right_wall ) car_sprite.translate(1,0);
        else if (GetAsyncKeyState(VK_DOWN) && collider.bottomLine < down_wall) car_sprite.translate(0,1);

        updateCollider();
        car_sprite.renderSprite(hConsole);
}

//updates colliders after modifications
void Car::updateCollider()
{
    collider = car_sprite.SquareCollider;
}

//mostra l'oggetto sullo schermo
void Car::RenderObject(HANDLE hConsole){
    car_sprite.renderSprite(hConsole);
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

void Car::renderColliders(HANDLE hConsole)
{
    car_sprite.renderColliders(hConsole);
}

//debugFunctions
void Car::printSpriteInfo(HANDLE hConsole){
    cout << car_sprite.screenPosition.X  << ", " << car_sprite.screenPosition.Y << " ";
}

//prints the coordinates of the pixels of the car
COORD Car::printSinglePixelInfo(HANDLE hConsole, COORD windowCursor){
    return car_sprite.printSinglePixelInfo(hConsole, windowCursor);
}

//prints the address in memory of the pixels
void Car::printAddressInfoDebug(){
    car_sprite.printAddressDebug();
}
