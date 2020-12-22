#include "Car.hpp"

Car::Car()
{
    carSprite = ConsoleSprite();
    updateCollider();
    speed = 100;
}

Car::Car(char* directory, int x, int y)
{
    carSprite = ConsoleSprite(directory,x,y);
    updateCollider();
    speed = 100;
}

void Car::Movement(HANDLE hConsole){

        carSprite.deleteSprite(hConsole);

        if ( GetAsyncKeyState(VK_UP) && collider.topLine > 1) carSprite.translate(0,-1);
        else if ( GetAsyncKeyState(VK_LEFT) && collider.leftLine > 11 ) carSprite.translate(-1,0);
        else if ( GetAsyncKeyState(VK_RIGHT) && collider.rightLine < 69 ) carSprite.translate(1,0);
        else if ( GetAsyncKeyState(VK_DOWN) && collider.bottomLine < 42 ) carSprite.translate(0,1);
        else if ( GetAsyncKeyState(81) && speed < 125 && acceleratorEnabler) speed++;
        else if ( GetAsyncKeyState(69) && speed > 1 && acceleratorEnabler) speed--;

        updateCollider();
        carSprite.renderSprite(hConsole);
}

//updates colliders after modifications
void Car::updateCollider()
{
    collider = carSprite.SquareCollider;
}

//mostra l'oggetto sullo schermo
void Car::RenderObject(HANDLE hConsole){
    carSprite.renderSprite(hConsole);
}

//debugFunctions
void Car::printSpriteInfo(HANDLE hConsole){
    cout << carSprite.screenPosition.X  << ", " << carSprite.screenPosition.Y << " ";
};

//prints the coordinates of the pixels of the car
COORD Car::printSinglePixelInfo(HANDLE hConsole, COORD windowCursor){
    return carSprite.printSinglePixelInfo(hConsole, windowCursor);
}

//prints the address in memory of the pixels
void Car::printAddressInfoDebug(){
    carSprite.printAddressDebug();
}
