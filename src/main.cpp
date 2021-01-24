#include "LevelManager.hpp"
#include "Menu.hpp"

COORD current_cursor;

//TODO: Check if updateCollider() does something or is useless by using devMode

void Play(HANDLE hConsole, LevelManager level_manager, Menu mainMenu);
void hidecursor();
void DebugWindow(HANDLE hConsole, Car* playerObject, LevelManager level);

int main()
{
    srand (time(NULL));

    //console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //creazione classe levelmanager
    LevelManager levelManager(hConsole);

    //occulta il cursore
    hidecursor();


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
        if (mainMenu.devModeEnabler) //if devMode is on...
        {
            DebugWindow(hConsole, level_manager.getPlayerCarPtr(), level_manager); //then show debug info
        }

        if (level_manager.isPlayerDead() || GetAsyncKeyState(VK_ESCAPE) !=0)
        {
            mainMenu.saveStats(level_manager.getStats());
            mainMenu.saveScore(level_manager.getTotalPoints());

            mainMenu.GameOverMenu();

            if (mainMenu.exitMenu()) //mainMenu exit bool segnala quando uscire dal loop
            {
                stop = true;
            } else
            {
                level_manager.Start();
            }
        }
    }
}


void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info; //informazioni sul cursore
   info.dwSize = 100;
   info.bVisible = FALSE; //attiva solo in debug
   SetConsoleCursorInfo(consoleHandle, &info);
}


void DebugWindow(HANDLE hConsole, Car* playerObject, LevelManager level)
{
    IndexQ* debugger = level.getQueuePtr();

    COORD window_position;
    window_position.X = UI_POS_X;
    window_position.Y = UI_POS_Y + UI_HEIGHT - 3;
    SetConsoleCursorPosition(hConsole, window_position);
    SetConsoleTextAttribute(hConsole, 14);

    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);

    cout << "player_position: ";
    playerObject->printSpritePosition();


    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);

    Collider* Collider_ptr;
    Collider_ptr = playerObject->getCollider_ptr();

    cout << "player_col:(" << Collider_ptr->topLine << ", " << Collider_ptr->leftLine  << ")" << ", (" << Collider_ptr->bottomLine << ", "
    << Collider_ptr->rightLine << ")";

    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);
    debugger->debugPrint();

    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);

    //mostra info di ciascun pixel della macchina

    playerObject->printSinglePixelInfo(&window_position);
}

