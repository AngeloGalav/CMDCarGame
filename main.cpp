#include "LevelManager.hpp"
#include "Menu.hpp"

COORD current_cursor;

//TODO: Check if updateCollider() does something or is useless by using devMode

void Play(HANDLE hConsole, LevelManager levelManger, Menu mainMenu);
void hidecursor();
void DebugWindow(HANDLE hConsole, Car playerObject, LevelManager level);

int main()
{
    int width = 113, height = 44;
    //random seed
    srand (time(NULL));

    //get console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //levelManager class
    LevelManager levelmanager(hConsole);

    //hide the cursorss
    hidecursor();

    //Menu class handles also SetWindow options (the dimensions of the window, the buffersize etc...)
    Menu mainMenu(width, height);
    mainMenu.MainMenu();

    if (!mainMenu.exit)
    {
        levelmanager.devMode = mainMenu.devModeEnabler;
        Play(hConsole, levelmanager, mainMenu);
    }

    return 0;
}

void Play(HANDLE hConsole, LevelManager levelManager, Menu mainMenu)
{
    bool stop = false;
    levelManager.Start();

    //gameLoop
    while (!stop)
    {

        levelManager.Update();
        if (levelManager.devMode) //if devMode is on...
        {
            DebugWindow(hConsole, levelManager.playerCar, levelManager); //then show debug info
        }

        if (levelManager.isPlayerDead() || GetAsyncKeyState(VK_ESCAPE) !=0)
        {
            mainMenu.GameOverMenu();
            if (mainMenu.exit) //mainMenu exit bool segnala quando uscire dal loop
            {
                stop = true;
            } else
            {
                levelManager.Start();
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
    window_position.X = 80;
    window_position.Y = 18;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    SetConsoleTextAttribute(hConsole, 14);


    /*
    //mostra info di 4 elementi raccoglibili su schermo
    //tra le info mostrate: posizione e colliders

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << level.collectables[0].collider.bottomLine << ", " << level.collectables[0].collider.topLine << ", " <<level.collectables[0].collider.rightLine << ", " <<level.collectables[0].collider.leftLine
    << ", " << level.availableCollectablesIndexes[0] << ", " << level.collectables[0].objectSprite.screenPosition.X << ", " << level.collectables[0].objectSprite.screenPosition.Y << "    ";
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << level.collectables[1].collider.bottomLine << ", " << level.collectables[1].collider.topLine << ", " <<level.collectables[1].collider.rightLine << ", " <<level.collectables[1].collider.leftLine
    << ", " <<level.availableCollectablesIndexes[1] << ", " << level.collectables[1].objectSprite.screenPosition.X << ", " << level.collectables[1].objectSprite.screenPosition.Y<< "    ";
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << level.collectables[2].collider.bottomLine << ", " << level.collectables[2].collider.topLine << ", " <<level.collectables[2].collider.rightLine << ", " <<level.collectables[2].collider.leftLine
    << ", " <<level.availableCollectablesIndexes[2] << ", " << level.collectables[2].objectSprite.screenPosition.X << ", " << level.collectables[2].objectSprite.screenPosition.Y<< "    ";
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << level.collectables[3].collider.bottomLine << ", " << level.collectables[3].collider.topLine << ", " <<level.collectables[3].collider.rightLine << ", " <<level.collectables[3].collider.leftLine
    << ", " <<level.availableCollectablesIndexes[3] << ", " << level.collectables[3].objectSprite.screenPosition.X << ", " << level.collectables[3].objectSprite.screenPosition.Y<< "    ";
    */

    cout << "player_col:(" << playerObject.collider.topLine << ", " << playerObject.collider.leftLine  << ")" << ", (" << playerObject.collider.bottomLine << ", "
     << playerObject.collider.rightLine << ")";

      window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

     cout << "player_col_ptr:(" << playerObject.collider_pointer->topLine << ", " << playerObject.collider_pointer->leftLine  << ")" << ", (" << playerObject.collider_pointer->bottomLine << ", "
     << playerObject.collider_pointer->rightLine << ")" ;

    window_position.Y += 2;
    //mostra info di ciascun pixel della macchina
    window_position = playerObject.printSinglePixelInfo(hConsole, window_position);

    window_position.X = 80;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

}

