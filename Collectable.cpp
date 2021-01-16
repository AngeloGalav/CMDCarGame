#include "Collectable.hpp"

Collectable::Collectable()
{
    stop = false;
    object_sprite = ConsoleSprite();
    updateCollider();
    effect = 0;
}

Collectable::Collectable(char* directory, int x, int y, int ef)
{
    stop = false;
    object_sprite = ConsoleSprite(directory,x,y);
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
    object_sprite.renderSprite(hConsole);
}

void Collectable::removeObject(HANDLE hConsole)
{
    object_sprite.deleteSprite(hConsole);
}

//funzione che fa andare giu i cosetti da prendere
void Collectable::moveForward(HANDLE hConsole)
{

    if (object_sprite.SquareCollider.bottomLine <= 42 && !stop)
    {
        object_sprite.deleteSprite(hConsole);
        object_sprite.translate(0,1);
        RenderObject(hConsole);
    }
    updateCollider();

    //if the sprite gets to the bottom.. delete it.
    /* else if (objectSprite.SquareCollider.bottomLine >= 42 && !stop)
    {
        Collision(hConsole);
    } */
}

void Collectable::renderColliders(HANDLE hConsole)
{
    object_sprite.renderColliders(hConsole);
}

void Collectable::moveTo(int x, int y)
{
    object_sprite.moveTo(x, y);
}

void Collectable::updateCollider()
{
    collider = object_sprite.SquareCollider;
}

/**
    Elimina i collider dandogli una posizione "nulla", fuori dai confini di gioco,
    (permettendo così il loro riutilizzo da parte di un nuovo elemento.)
    Lo sprite dell'oggetto viene poi cancellato dall'area di gioco.


    @params: hConsole, il puntatore al framebuffer.
*/
void Collectable::Collision(HANDLE hConsole)
{
    //stop the object
    if (!stop)
    {
        object_sprite.deleteSprite(hConsole);
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
    return object_sprite.printSinglePixelInfo(hConsole, windowCursor);
}

//versione provvisoria per la collisione.. ora la collision detection e' gestita dal LevelManager, questa funzione viene usata per il debugging
//TODO: CANCELLA QUESTA FUNZIONE.
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
    object_sprite.printAddressDebug();
}

