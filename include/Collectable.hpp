#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP
#include "ConsoleSprite.hpp"
#include "Car.hpp"

enum CollectableType {
    UnknownCollectable = -1,
    EnemyCar = 0,
    Puddle,
    Gas
};

class Collectable : public ConsoleSprite
{
    protected:
        //stop boolean for stopping the object after collision
        bool stop;
        int effect;
        CollectableType collectable_type;

    public:

        //constructors
        Collectable();
        Collectable(char* directory, int ef, CollectableType elem_type);

        int getEffect();

        CollectableType getTypeOfCollectable();

        void moveForward(HANDLE hConsole);

        void Collision(HANDLE hConsole);
};

#endif // COLLECTABLE_HPP
