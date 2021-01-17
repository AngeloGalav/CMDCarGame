#include "Collectable.hpp"

Collectable::Collectable() : ConsoleSprite()
{
    stop = false;
    effect = 0;
}

Collectable::Collectable(char* directory, int x, int y, int ef) : ConsoleSprite(directory,x,y)
{
    stop = false;
    effect = ef;
}

int Collectable::getEffect()
{
    return effect;
}

//funzione che fa andare giu i cosetti da prendere
void Collectable::moveForward(HANDLE hConsole)
{
    if (rect_collider.bottomLine <= 42 && !stop)
    {
        deleteSprite(hConsole);
        translate(0,1);
        renderSprite(hConsole);
    }
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
        deleteSprite(hConsole);
    }
    stop = true;

    //deleteCollider by offsetting it temporarily
    rect_collider.bottomLine = 999;
    rect_collider.leftLine = 999;
    rect_collider.topLine = 999;
    rect_collider.rightLine = 999;
};

