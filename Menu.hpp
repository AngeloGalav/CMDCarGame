#ifndef MENU_HPP
#define MENU_HPP
#include "ConsoleSprite.hpp"
using namespace std;

class Menu
{
    protected:
        _COORD position;
        int width;
        int height ;
        void printTextFile(char* directory, bool centered, int x, int y);
        HANDLE hconsole;

    public:
        //constructor
        Menu(int width,int height);
        bool devModeEnabler;
        bool back_to_main;

        bool exit;
        void SetWindow();
        void howToPlay();
        void MainMenu();
        void GameOverMenu();

};

#endif // MENU_HPP
