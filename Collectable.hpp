#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP
#include "ConsoleSprite.hpp"
#include "Car.hpp"

/**
    TODO: EFFECT GET-SET
*/

class Collectable : public ConsoleSprite
{
    protected:
        //stop boolean for stopping the object after collision
        bool stop;
        int effect;
    public:

        //constructors
        Collectable();
        Collectable(char* directory, int x, int y, int ef);

        int getEffect();

        //used to delete annoying ghosting effect
        void moveForward(HANDLE hConsole);

        void Collision(HANDLE hConsole);
};

#endif // COLLECTABLE_HPP
