#include "LevelManager.hpp"
#include "Menu.hpp"

COORD current_cursor;

//TODO: Check if updateCollider() does something or is useless by using devMode

void Play(HANDLE hConsole, LevelManager level_manager, Menu mainMenu);
void hidecursor();
void DebugWindow(HANDLE hConsole, Car playerObject, LevelManager level);

int main()
{
    //random seed
    srand (time(NULL));

    //get console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //levelManager class
    LevelManager levelManager(hConsole);

    //hide the cursorss
    hidecursor();

    //Menu class handles also SetWindow options (the dimensions of the window, the buffersize etc...)
    Menu mainMenu(SCREEN_WIDTH, SCREEN_HEIGHT);
    mainMenu.MainMenu();

    if (!mainMenu.exit)
    {
        levelManager.devMode = mainMenu.devModeEnabler;
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
        if (level_manager.devMode) //if devMode is on...
        {
            DebugWindow(hConsole, level_manager.playerCar, level_manager); //then show debug info
        }

        if (level_manager.isPlayerDead() || GetAsyncKeyState(VK_ESCAPE) !=0)
        {
            mainMenu.saveStats(level_manager.getStats());
            mainMenu.savePoints(level_manager.getTotalPoints());

            mainMenu.GameOverMenu();

            if (mainMenu.exit) //mainMenu exit bool segnala quando uscire dal loop
            {
                stop = true;
            } else
            {
                level_manager.Start();
            }
        }
    }

}

//function for hiding the cursor
void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info; //information of the cursor
   info.dwSize = 100;
   info.bVisible = FALSE; //only active for debug
   SetConsoleCursorInfo(consoleHandle, &info);
}


void DebugWindow(HANDLE hConsole, Car playerObject, LevelManager level)
{
    COORD window_position;
    window_position.X = UI_POS_X;
    window_position.Y = UI_POS_Y + UI_WIDTH;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    SetConsoleTextAttribute(hConsole, 14);

    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

    cout << "player_position: ";
    playerObject.printSpritePosition();


    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

    Collider* Collider_ptr;
    Collider_ptr = playerObject.getCollider_ptr();

    cout << "player_col:(" << Collider_ptr->topLine << ", " << Collider_ptr->leftLine  << ")" << ", (" << Collider_ptr->bottomLine << ", "
    << Collider_ptr->rightLine << ")";


    window_position.Y++;
    //mostra info di ciascun pixel della macchina
    window_position = playerObject.printSinglePixelInfo(hConsole, window_position);

}

