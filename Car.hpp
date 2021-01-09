#ifndef CAR_HPP
#define CAR_HPP
#include "ConsoleSprite.hpp"

/**
    TODO: GET-SET DI PLAYER CAR SPEED E ACCELERATOR ENABLER
    elimina flickering
*/

class Car
{
    protected:
        ConsoleSprite carSprite;
        void updateCollider();

    public:
        //contructors
        Car();
        Car(char* directory, int x, int y);

        //function for car movement by keyboard
        void Movement(HANDLE hConsole);
        int speed;
        bool acceleratorEnabler;

        //functions for rendering
        void RenderObject(HANDLE hConsole);

        //generates a square Collider, and can be used to delete annoying ghosting effects
        Collider collider;

        //debugStuff
        void printSpriteInfo(HANDLE hConsole);
        COORD printSinglePixelInfo(HANDLE hConsole, COORD windowCursor);
        void printAddressInfoDebug();
        void setSpeed(int n);
        int getSpeed();
        void setAccelerator(bool b);
        bool getAccelerator();


};

#endif // CAR_HPP
