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
    protected:
        int pixelCount;
        void calculate_RightLeftLine();
        Pixel pixels[MAX_PIXEL_SIZE];   //32 è il massimo buffer size
        void loadFromFile(char* directory);
        COORD screenPosition;
        void generateCollider();
        Collider rect_collider;
        void translate(int x, int y);
        HANDLE hConsole;

    public:
        //costruttori
        ConsoleSprite();
        ConsoleSprite(char* directory, int x, int y, HANDLE thConsole);
        void setConsoleHandle(HANDLE hConsole);

        void renderSprite();
        void deleteSprite();

        COORD getPosition();
        Collider* getCollider_ptr();
        bool checkCollision(Collider* to_check);

        void SetCharOfAllPixel(char pixChar);
        void SetColorOfAllPixels(WORD Color);

        void moveTo(int x, int y);

        //debugStuff
        void renderColliders();
        void deleteCollider_render();
        void printSinglePixelInfo(COORD* windowCursor);
        void printSpritePosition();

};

#endif // CONSOLESPRITE_HPP
