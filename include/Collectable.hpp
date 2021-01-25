#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP
#include "ConsoleSprite.hpp"
#include "Car.hpp"

enum CollectableType
{
    UnknownCollectable = -1,
    EnemyCar = 0,
    Puddle,
    Gas
};

class Collectable : public ConsoleSprite
{
    protected:
        bool stop;  //booleano che stoppa l'oggetto dopo una collisione.
        int effect;
        CollectableType collectable_type;
        int direction;

    public:
        //constructors
        Collectable();
        Collectable(char* directory, int ef, CollectableType elem_type, HANDLE thConsole);

        int getEffect();
        CollectableType getTypeOfCollectable();

        void moveForward();
        void Movement();
        void randomDir();
        void setDir(int direction);
        int getDir();

        void Collision();
};

#endif // COLLECTABLE_HPP
