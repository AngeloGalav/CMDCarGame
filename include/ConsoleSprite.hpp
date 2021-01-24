#ifndef CONSOLESPRITE_HPP
#define CONSOLESPRITE_HPP
#include <fstream>
#include <string>
#include "Pixel.hpp"

struct Collider
{
    SHORT rightLine; //salviamo solo la y perchè la x è uguale per entrambi
    SHORT leftLine;

    SHORT bottomLine;//salvo solamente la y perchè la x è uguale per entrambi
    SHORT topLine;

};


class ConsoleSprite
{
    //screePosition etc. sono READ ONLY! NON MODIFICARE fuori dallo scope della classe
    private:
        int pixelCount;
        void calculate_RightLeftLine();
        Pixel pixels[MAX_PIXEL_SIZE];   //32 è il massimo buffer size
        void loadFromFile(char* directory);

    protected:
        COORD screenPosition;
        void generateCollider();
        Collider rect_collider;
        HANDLE object_hConsole;
        void translate(int x, int y);

    public:
        //costruttori
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
