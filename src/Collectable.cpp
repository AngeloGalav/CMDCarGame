#include "Collectable.hpp"

Collectable::Collectable() : ConsoleSprite()
{
    collectable_type = UnknownCollectable;
    stop = false;
    effect = 0;
    direction = 0;
}

Collectable::Collectable(char* directory, int ef, CollectableType elem_type, HANDLE thConsole) : ConsoleSprite(directory, 0, 0, thConsole)
{
    collectable_type = elem_type;
    stop = false;
    effect = ef;
    direction = 0;
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
    Muove in avanti l'oggetto in zigzag, cambiando direzione
    ogni volta che sbatte a lato, oppure muove in avanti l'oggetto, finché non incontra
    il fondo dello schermo.

    Il modo in cui si muove dipende dal valore di direction.

    E' molto diverso dal metodo Movement della classe Car.
*/
void Collectable::Movement()
{
    if (rect_collider.bottomLine <= LOWER_SCREEN_BOUNDARY && !stop)
    {
        if (rect_collider.leftLine < LEFT_SCREEN_BOUNDARY + 2
         || rect_collider.rightLine > RIGHT_SCREEN_BOUNDARY - 2) {direction *= -1;}

        deleteSprite();
        translate(direction, 1);
        renderSprite();

        //impedisce a un elemento essere bloccato
        if (direction != 0 && rect_collider.leftLine <= LEFT_SCREEN_BOUNDARY)
        {
            deleteSprite();
            moveTo(LEFT_SCREEN_BOUNDARY + 3, screenPosition.Y);
            renderSprite();
        }
        if (direction != 0 && rect_collider.rightLine >= RIGHT_SCREEN_BOUNDARY)
        {
            deleteSprite();
            moveTo(RIGHT_SCREEN_BOUNDARY - 2, screenPosition.Y);
            renderSprite();
        }
    }
}

/**
    Setta la direzione dell'oggetto in modo randomico.
*/
void Collectable::randomDir()
{
    int randInt = rand() % 3;

    if (randInt == 0) direction = -1;
    else if (randInt == 1) direction = 1;
    else direction = 0;
}

/**
    Setta la direzione dell'oggetto.
*/
void Collectable::setDir(int direction)
{
    this->direction = direction;
}

/**
    Ritornna la direzione dell'oggetto.
*/
int Collectable::getDir()
{
    return direction;
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

