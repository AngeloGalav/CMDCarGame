#include "Collectable.hpp"

Collectable::Collectable() : ConsoleSprite()
{
    collectable_type = UnknownCollectable;
    stop = false;
    effect = 0;
}

Collectable::Collectable(char* directory, int ef, CollectableType elem_type) : ConsoleSprite(directory, 0, 0)
{
    collectable_type = elem_type;
    stop = false;
    effect = ef;
}

/**
    Getter dell'effetto.

    @returns: effect, il valore dell'effetto.
*/
int Collectable::getEffect()
{
    return effect;
}

/**
    Muove in avanti l'oggetto, finché non incontra
    il fondo dello schermo.

    @params: hConsole, il puntatore al framebuffer.
*/
void Collectable::moveForward(HANDLE hConsole)
{
    if (rect_collider.bottomLine <= LOWER_SCREEN_BOUNDARY && !stop)
    {
        deleteSprite(hConsole);
        translate(0, 1);
        renderSprite(hConsole);
    }
}

CollectableType Collectable::getTypeOfCollectable()
{
    return collectable_type;
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
    rect_collider.bottomLine = DELETED_COL;
    rect_collider.leftLine = DELETED_COL;
    rect_collider.topLine = DELETED_COL;
    rect_collider.rightLine = DELETED_COL;
};

