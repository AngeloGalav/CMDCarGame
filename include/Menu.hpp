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
        void printStats();
        void printStatsRec(int nline);
        void displayPoints();

        Queue<level_info>* stats_from_level;
        int points_from_level;
        bool exit;

        HANDLE hconsole;

    public:
        //constructor
        Menu();
        bool devModeEnabler;
        bool lightWeightEnabler;
        bool back_to_main;

        bool exitMenu();
        void SetWindow();
        void howToPlay();
        void MainMenu();
        void GameOverMenu();

        void saveStats(Queue<level_info>* to_retrieve);
        void saveScore(int points);
};

#endif // MENU_HPP
