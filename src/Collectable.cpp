#include "Collectable.hpp"

Collectable::Collectable() : ConsoleSprite()
{
    collectable_type = UnknownCollectable;
    stop = false;
    effect = 0;
}

Collectable::Collectable(char* directory, int ef, CollectableType elem_type, HANDLE thConsole) : ConsoleSprite(directory, 0, 0, thConsole)
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
*/
void Collectable::moveForward()
{
    if (rect_collider.bottomLine <= LOWER_SCREEN_BOUNDARY && !stop)
    {
        deleteSprite();
        translate(0, 1);
        renderSprite();
    }
}

/**
    Ritorna il tipo dell'oggetto. Usato per il conteggio degli elementi
    in LevelManager.
*/
CollectableType Collectable::getTypeOfCollectable()
{
    return collectable_type;
}


/**
    Elimina i collider dandogli una posizione "nulla", fuori dai confini di gioco,
    (permettendo così il loro riutilizzo da parte di un nuovo elemento.)
    Lo sprite dell'oggetto viene poi cancellato dall'area di gioco.

*/
void Collectable::Collision()
{
    //ferma l'oggetto.
    if (!stop)
    {
        deleteSprite();
    }
    stop = true;

    //elemina il collider dandogli un offset molto grande.
    rect_collider.bottomLine = DELETED_COL;
    rect_collider.leftLine = DELETED_COL;
    rect_collider.topLine = DELETED_COL;
    rect_collider.rightLine = DELETED_COL;
};

