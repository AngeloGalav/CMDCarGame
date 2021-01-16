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
        ConsoleSprite car_sprite;
        void updateCollider();
        bool accelerator_enabler;

        //confini di dove la macchina si può muovere.
        int left_wall;
        int right_wall;
        int up_wall;
        int down_wall;


    public:
        //contructors
        Car();
        Car(char* directory, int x, int y);

        //function for car movement by keyboard
        void Movement(HANDLE hConsole);

        //functions for rendering
        void RenderObject(HANDLE hConsole);

        //generates a square Collider, and can be used to delete annoying ghosting effects
        Collider collider;

        //sets boundaries for car
        void setBoundaries(int leftWall, int rightWall, int upWall, int downWall);

        //debugStuff
        void renderColliders(HANDLE hConsole);
        void printSpriteInfo(HANDLE hConsole);
        COORD printSinglePixelInfo(HANDLE hConsole, COORD windowCursor);
        void printAddressInfoDebug();

};

#endif // CAR_HPP
