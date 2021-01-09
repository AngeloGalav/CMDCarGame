#include "Collectable.hpp"

Collectable::Collectable()
{
    stop = false;
    objectSprite = ConsoleSprite();
    updateCollider();
    effect = 0;
}

Collectable::Collectable(char* directory, int x, int y, int ef)
{
    stop = false;
    objectSprite = ConsoleSprite(directory,x,y);
    updateCollider();
    effect = ef;
}

void Collectable::setEffect(int n)
{
    effect = n;
}

int Collectable::getEffect()
{
    return effect;
}

void Collectable::RenderObject(HANDLE hConsole)
{
    objectSprite.renderSprite(hConsole);
}

void Collectable::removeObject(HANDLE hConsole)
{
    objectSprite.deleteSprite(hConsole);
}

//funzione che fa andare giu i cosetti da prendere
void Collectable::moveForward(HANDLE hConsole)
{
    updateCollider();
    if (objectSprite.SquareCollider.bottomLine <= 42 && !stop)
    {
        objectSprite.deleteSprite(hConsole);
        objectSprite.translate(0,1);
        RenderObject(hConsole);
    }

    //if the sprite gets to the bottom.. delete it.
    /* else if (objectSprite.SquareCollider.bottomLine >= 42 && !stop)
    {
        Collision(hConsole);
    } */
}

void Collectable::moveTo(int x, int y)
{
    objectSprite.moveTo(x, y);
}

void Collectable::updateCollider()
{
    collider = objectSprite.SquareCollider;
}

//collision handler.
void Collectable::Collision(HANDLE hConsole)
{
    //stop the object
    if (!stop)
    {
        objectSprite.deleteSprite(hConsole);
    }
    stop = true;

    //deleteCollider by offsetting it temporarily
    collider.bottomLine = 999;
    collider.leftLine = 999;
    collider.topLine = 999;
    collider.rightLine = 999;


};

//debugInfo
COORD Collectable::printSinglePixelInfo(HANDLE hConsole, COORD windowCursor)
{
    return objectSprite.printSinglePixelInfo(hConsole, windowCursor);
}

//versione provvisoria per la collisione.. ora la collision detection e' gestita dal LevelManager
void Collectable::checkCollidersDebug(HANDLE hConsole, Car playerCar)
{
    if ((playerCar.collider.topLine <=  collider.bottomLine && playerCar.collider.topLine >=  collider.topLine)
        || (playerCar.collider.bottomLine >=  collider.bottomLine && playerCar.collider.bottomLine <=  collider.topLine)
        || (collider.bottomLine >=  playerCar.collider.topLine && collider.bottomLine <=  playerCar.collider.bottomLine))

    {
        if ((playerCar.collider.leftLine >=  collider.leftLine && playerCar.collider.leftLine <=  collider.rightLine)
            || (playerCar.collider.rightLine <=  collider.rightLine && playerCar.collider.rightLine >=  collider.leftLine)
            || (collider.rightLine >=  playerCar.collider.leftLine && collider.rightLine <=  playerCar.collider.rightLine))
        {
            //COLLISION!!!!
            Collision(hConsole);
        }
    }
}

void Collectable::printAddressInfoDebug()
{
    objectSprite.printAddressDebug();
}

