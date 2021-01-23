#ifndef MENU_HPP
#define MENU_HPP
#include <string>
#include <fstream>
#include "List.hpp"
#include "Definitions.hpp"

using namespace std;

class Menu
{
    protected:
        _COORD position;
        int width;
        int height ;
        void printTextFile(char* directory, bool centered, int x, int y);
        COORD printStats(COORD position);
        void displayPoints(COORD position);

        ptr_list stats_from_level;
        int points_from_level;


        HANDLE hconsole;

        //menu design variables:

        int entries_spacing;    //TODO: WHAT TO DO WITH THIS?

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

        void saveStats(ptr_list to_retrieve);
        void savePoints(int points);
};

#endif // MENU_HPP
