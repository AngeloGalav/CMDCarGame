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
        Car(char* directory, int x, int y);

        //function for car movement by keyboard
        void Movement(HANDLE hConsole);

        //sets boundaries for car
        void setBoundaries(int leftWall, int rightWall, int upWall, int downWall);

};

#endif // CAR_HPP
