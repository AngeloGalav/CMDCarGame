#ifndef CONSOLESPRITE_HPP
#define CONSOLESPRITE_HPP
#include <fstream>
#include <string>
#include "Pixel.hpp"

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
        Pixel pixels[32];   //32 is the maximum sprite buffer size
        void loadFromFile(char* directory);

    protected:
        COORD screenPosition;
        void generateCollider();
        Collider rect_collider;
        HANDLE object_hConsole;
        void translate(int x, int y);

    public:
        //constructors
        ConsoleSprite();
        ConsoleSprite(char* directory, int x, int y);
        void setConsoleHandle(HANDLE hConsole);

        void renderSprite(HANDLE hConsole);
        void deleteSprite(HANDLE hConsole);

        Collider* getCollider_ptr();
        bool checkCollision(Collider* to_check);

        COORD getPosition();

        void moveTo(int x, int y);

        //debugStuff
        void renderColliders(HANDLE hConsole);
        void deleteCollider_render(HANDLE hConsole);
        COORD printSinglePixelInfo(HANDLE hConsole, COORD windowCursor);
        void printAddressDebug();
        void printSpritePosition();

};

#endif // CONSOLESPRITE_HPP
