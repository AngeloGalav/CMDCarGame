#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP
#include "ConsoleSprite.hpp"
#include "Car.hpp"

enum CollectableType
{
    UnknownCollectable = -1,    //parte da -1 (così gli altri enum saranno 0,1,2)
    EnemyCar = 0,
    Puddle,
    Gas
};

class Collectable : public ConsoleSprite
{
    protected:
        bool stop;  //booleano che stoppa l'oggetto dopo una collisione.
        int effect; //punti sommati al giocatore quando viene colpito
        CollectableType collectable_type;
        int direction;  //direzione di movimento dell'oggetto

    public:
        //constructors
        Collectable();
        Collectable(char* directory, int ef, CollectableType elem_type, HANDLE thConsole);

        int getEffect();
        CollectableType getTypeOfCollectable();

        void Movement();
        void randomDir();
        void setDir(int direction);
        int getDir();

        void Collision();
};

#endif // COLLECTABLE_HPP
