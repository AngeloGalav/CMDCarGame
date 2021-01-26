#include "Menu.hpp"

Menu::Menu()
{
    height = SCREEN_HEIGHT;
    width = SCREEN_WIDTH;
    position.X = 0;
    position.Y = height - 34;
    exit = false;
    devModeEnabler = false;
    lightWeightEnabler = false;
    SetWindow();
}

/**
    Crea la finestra del menu.
*/
void Menu::SetWindow()
{
    _COORD coord;
    coord.X = width;
    coord.Y = height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = height - 1;
    Rect.Right = width - 1;

    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);    //prendi il puntatore al framebuffer
    SetConsoleScreenBufferSize(hconsole, coord);  //setta le dimensioni del buffer
    SetConsoleWindowInfo(hconsole, TRUE, &Rect);  //setta le dimensioni della finestra
                                                    //(il secondo parametro indica come le coordinate dovrebbero essere calcolate,
                                                //in questo caso, è dall'alto verso il basso (0 è la coordinata più in alto a dx))
}

/**
    Mostra l'how to play.
*/
void Menu::howToPlay()
{
    SetConsoleTextAttribute(hconsole, BLACK_B_GREEN_F);
    back_to_main = false;
    system("cls");

    position.Y = 10;

    char directory[25];
    strcpy(directory, "menu/howto.txt");
    printTextFile(directory, true, width, 0);

    while(!back_to_main)
    {
        Sleep(100);
        if(GetAsyncKeyState(VK_RETURN) != 0)
        {
            back_to_main = true;
            system("cls");
        }
    }
    back_to_main = false;
    MainMenu();
}

/**
    Menu di Game Over.
*/
void Menu::GameOverMenu()
{
    SetConsoleTextAttribute(hconsole, BLACK_B_BLACK_F);
    SHORT CursorMenuStartPosition = 0;
    system("cls");

    position.Y = 10;

    bool stop = false;
    int index = 0;

    char directory[25];
    strcpy(directory, "menu/gameover.txt");

    //change text color
    SetConsoleTextAttribute(hconsole, BLACK_B_PURPLE_F);
    printTextFile(directory, true, width, position.Y);

    //menu entries
    SetConsoleTextAttribute(hconsole, BLACK_B_WHITE_F);
    position.X = 42;

    position.Y += 4;
    CursorMenuStartPosition = position.Y;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Restart";

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Exit";

    position.Y += 3;
    printStats();

    position.Y += 2;
    displayPoints();

    while(!stop)
    {
        position.X = 70;

        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, BLACK_B_BLACK_F);

        cout << "             " ;
        //codice del rendering della freccetta lol
        position.Y = CursorMenuStartPosition + index;

        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, BLACK_B_CYAN_F);
        cout << "<--  ";

        Sleep(100);

        //vai su o giu nel menu
        if(GetAsyncKeyState(VK_UP) != 0)
        {
            index--;
            if(index < 0)
            {
                index = 1;
            }
        }
        else if(GetAsyncKeyState(VK_DOWN) != 0)
        {
            index++;
            if(index > 1)
            {
                index = 0;
            }
        }
        else if(GetAsyncKeyState(VK_RETURN) !=0)
        {
            stop = true;

            if (index == 1) //exit
            {
                exit = true;
            }
        }
    }
}

/**
    Mostra il menu principale.
*/
void Menu::MainMenu()
{
    //posizione di inzio del menu.
    SHORT CursorMenuStartPosition = 0;

    //colore del titolo
    SetConsoleTextAttribute(hconsole, BLACK_B_RED_F); //12
    position.X = 0;
    position.Y = 10;

    bool stop = false;
    int index = 0; //index of menu cursor

    char directory[25];
    strcpy(directory, "menu/title.txt");
    printTextFile(directory, true, width, position.Y);

    SetConsoleTextAttribute(hconsole, BLACK_B_YELLOW_F);

    position.Y += 2;
    strcpy(directory, "menu/titlebar.txt");
    printTextFile(directory, true, width, position.Y);

    //menu entries
    SetConsoleTextAttribute(hconsole, BLACK_B_WHITE_F);
    position.X = 42;

    position.Y += 2;
    CursorMenuStartPosition = position.Y;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# New Game";

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# How To Play";

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);

    if (lightWeightEnabler){
        cout << "# Enable RTX                  ";
    } else
    {
        cout << "# Disable RTX                 ";
    }

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);

    if (!devModeEnabler){
        cout << "# Enable DEVMODE                ";
    } else
    {
        cout << "# Disable DEVMODE               ";
    }

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Exit";

    position.Y += 7;

    strcpy(directory, "menu/menuhowto.txt");
    printTextFile(directory, true, width, position.Y);

    while(!stop)
    {
        position.X = 68;

        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, BLACK_B_BLACK_F);
        cout << "              " ;

        //codice del rendering della freccetta
        position.Y = CursorMenuStartPosition + index;
        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, BLACK_B_CYAN_F);
        cout << "<--  ";

        Sleep(100);

        //vai su o giu nel menu
        if(GetAsyncKeyState(VK_UP) != 0)
        {
            index--;
            if(index < 0)
            {
                index = 4;
            }
        }
        else if(GetAsyncKeyState(VK_DOWN) != 0)
        {
            index++;
            if(index == 5)
            {
                index = 0;
            }
        }
        else if(GetAsyncKeyState(VK_RETURN) !=0)
        {
            if (index == 0)
            {
                //setto stop = true per uscire dal loop.
                stop = true;
            }
            else if (index == 1)
            {
                stop = true;
                howToPlay();

            }
            else if (index == 2)
            {
                stop = true;
                lightWeightEnabler = !lightWeightEnabler;
                MainMenu();
            }
            else if (index == 3)
            {
                stop = true;
                devModeEnabler = !devModeEnabler;
                MainMenu();
            }
            else if (index == 4)
            {
                //l'exit bool è public, in modo da essere letto da tutti senza getter.
                exit = true;
                stop = true;
            }
        }
    }
}

/**
    Ritorna il valore booleano exit.
*/
bool Menu::exitMenu()
{
    return exit;
}

/**
    Stampa il un file di testo, centrandolo se richiesto.

    @params: directory, posizione del file nel computer.
             centered, true se si vuole il testo centrato, false altrimenti
             x e y, posizione dell'elemento. (coordinate usate SOLO se non si centra il testo)
*/
void Menu::printTextFile(char* directory, bool centered, int x, int y)
{
    char ch;
    int mxChar = 0;

    int lineCharCounter = 0;

    if (centered)
    {
        //primo filestream per il conteggio dei caratteri, usato solo se il documento deve essere centrato
        fstream fin(directory, fstream::in);

        while (fin >> noskipws >> ch)
        {
            if (ch != '\n')
            {
                lineCharCounter++;
            } else
            {
                if (lineCharCounter > mxChar)
                {
                    mxChar = lineCharCounter;
                }
                lineCharCounter = 0;
            }

        }
        position.X = x/2 - mxChar/2;


        //fstream si chiude automaticamente, ma è buona pratica chiuderlo manualmente quando si finisce di usarlo.
        fin.close();
    } else
    {
        //se il testo non deve essere centrato, allora lo mette nella posizione di default
        position.X = x;
    }
    SetConsoleCursorPosition(hconsole, position);

    //secondo filestream per stampare effettivamente il documento nel gioco
    fstream fin1(directory, fstream::in);
    while (fin1 >> noskipws >> ch)
    {
        cout << ch;
        if (ch == '\n')
        {
            position.Y++;
            SetConsoleCursorPosition(hconsole,position);
        }

    }

    fin1.close();
}

/**
    Prende la lista delle statistische da mostrare dal levelmanager.
*/
void Menu::saveStats(InfoQ* to_retrieve)
{
    stats_from_level = to_retrieve;
}

/**
    Prende il numero totale di punti dal levelmanager.
*/
void Menu::saveScore(int points)
{
    points_from_level = points;
}

/**
    Mostra il numero totale di punti su schermo.
*/
void Menu::displayPoints()
{
    position.X = 19;
    SetConsoleCursorPosition(hconsole, position);
    SetConsoleTextAttribute(hconsole, BLACK_B_RED_F);
    cout << "Total points: " << points_from_level;
}


/**
    Stampa le statistiche degli ultimi 10 livelli,
    in modo ricorsivo.
*/
void Menu::printStats()
{
    position.X = 19;
    SetConsoleCursorPosition(hconsole,position);
    SetConsoleTextAttribute(hconsole, BLACK_B_WHITE_F);

    cout << "Previous levels stats: ";
    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    SetConsoleTextAttribute(hconsole, BLACK_B_GREEN_F);

    if ( !(stats_from_level->isEmpty()) )
    {
        printStatsRec(0);
    } else cout << "No statistics to display :-(";
}

void Menu::printStatsRec(int nline)
{
    if ( !(stats_from_level->isEmpty()) && nline < 10)
    {
        cout << "- ";

        level_info toPrint = stats_from_level->pop();
        stats_from_level->printInfo(toPrint);

        position.Y++;
        SetConsoleCursorPosition(hconsole,position);
        nline++;
        printStatsRec(nline);
    }
}
