#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP
#include "ConsoleSprite.hpp"
#include "Car.hpp"

/**
    TODO: EFFECT GET-SET
*/

class Collectable
{
    protected:
        //stop boolean for stopping the object after collision
        ConsoleSprite object_sprite;
        bool stop;

    public:

        //constructors
        Collectable();
        Collectable(char* directory, int x, int y, int ef);

        //Sprite & Collider info (Colliders are just read only versions of squareColliders)

        int effect;
        Collider collider; //perchè public?
        void updateCollider();
        void Collision(HANDLE hConsole);
        void setEffect(int n);
        int getEffect();
        //used to delete annoying ghosting effect
        void RenderObject(HANDLE hConsole);
        void moveForward(HANDLE hConsole);

        void removeObject(HANDLE hConsole);
        void checkCollidersDebug(HANDLE hConsole, Car playerCar);

        void moveTo(int x, int y);

        //debug functions
        void renderColliders(HANDLE hConsole);
        void printAddressInfoDebug();
        COORD printSinglePixelInfo(HANDLE hConsole, COORD windowCursor);
        void checkCollidersDebug(HANDLE hConsole);
};

#endif // COLLECTABLE_HPP
