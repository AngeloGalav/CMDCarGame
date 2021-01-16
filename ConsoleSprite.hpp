#ifndef CONSOLESPRITE_HPP
#define CONSOLESPRITE_HPP
#include <fstream>
#include <string>
#include "Pixel.hpp"

//to understand how this collider works, imagine 4 line that go from end of the screen to the other. The collider looks like a tic-tac-toe grid
struct Collider
{
    SHORT rightLine; //i'll save only the y coordinates of these, since the x value is the same for both
    SHORT leftLine;

    SHORT bottomLine;// i'll save only the x coordinates of these, since the y value is the same for both
    SHORT topLine;

};

class ConsoleSprite
{
    //screen position values etc are READ ONLY! DO NOT modify them outside of the class scope
    private:
        int pixelCount;
        void calculate_RightLeftLine();
        void generateCollider();
        Pixel pixels[32]; //maximum sprite buffer size

    public:
        //constructors
        ConsoleSprite();
        ConsoleSprite(char* directory, int x, int y);

        //collider info
        Collider SquareCollider;
        COORD screenPosition;

        //position relative to the screen (WORST FUCKING IDEA EVER)

        void loadFromFile(char* directory);
        void renderSprite(HANDLE hConsole);
        void deleteSprite(HANDLE hConsole);

        void translate(int x, int y);
        void moveTo(int x, int y);


        //debugStuff
        void renderColliders(HANDLE hConsole);
        COORD printSinglePixelInfo(HANDLE hConsole, COORD windowCursor);
        void printAddressDebug();

};

#endif // CONSOLESPRITE_HPP
