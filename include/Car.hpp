#ifndef CAR_HPP
#define CAR_HPP
#include "ConsoleSprite.hpp"

class Car : public ConsoleSprite
{
    protected:
        //confini di dove la macchina si può muovere.
        int left_wall;
        int right_wall;
        int up_wall;
        int down_wall;

    public:
        //contructors
        Car();
        Car(char* directory, int x, int y, HANDLE thConsole);

        //funzioni per il movimento della macchina via tastiera.
        void optimized_Movement();

        //setta i limiti di movimento della macchina
        void setBoundaries(int leftWall, int rightWall, int upWall, int downWall);

};

#endif // CAR_HPP
