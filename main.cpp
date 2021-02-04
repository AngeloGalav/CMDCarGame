#include "include\LevelManager.hpp"
#include "include\Menu.hpp"
#include <windows.h>

void Play(HANDLE hConsole, LevelManager level_manager, Menu mainMenu);
void windowOptions();
void DebugWindow(HANDLE hConsole, Car* playerObject, LevelManager level);

int main()
{
    srand (time(NULL));

    //console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //creazione classe levelmanager
    LevelManager levelManager(hConsole);
    //occulta il cursore
    windowOptions();

    Menu mainMenu = Menu();
    mainMenu.MainMenu();

    if (!mainMenu.exitMenu())
    {
        levelManager.devMode = mainMenu.devModeEnabler;
        levelManager.lightWeightMode = mainMenu.lightWeightEnabler;
        Play(hConsole, levelManager, mainMenu);
    }

    return 0;
}

void Play(HANDLE hConsole, LevelManager level_manager, Menu mainMenu)
{
    bool stop = false;
    level_manager.Start();

    //gameLoop
    while (!stop)
    {
        level_manager.Update();

        if (level_manager.isPlayerDead() || GetAsyncKeyState(VK_ESCAPE) != 0)
        {
            mainMenu.saveStats(level_manager.getStats());
            mainMenu.saveScore(level_manager.getTotalPoints());

            mainMenu.GameOverMenu();

            if (mainMenu.exitMenu()) //mainMenu exit bool segnala quando uscire dal loop
            {
                stop = true;
            } else
            {
                system("cls");

                level_manager.listCleanup(); //pulizia delle liste

                mainMenu.MainMenu();

                if (!mainMenu.exitMenu())
                {
                    level_manager.devMode = mainMenu.devModeEnabler;
                    level_manager.lightWeightMode = mainMenu.lightWeightEnabler;
                    level_manager.Start();
                }
                else stop = true;
            }
        }
    }
}


void windowOptions()
{
    //codice che permette alla finestra di non venire scalata.
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    //codice che nasconde il cursore
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info; //informazioni sul cursore
    info.dwSize = 100;
    info.bVisible = FALSE; //attiva solo in debug
    SetConsoleCursorInfo(consoleHandle, &info);
}
