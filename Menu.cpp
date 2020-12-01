#include "Menu.hpp"
#include <string>

Menu::Menu(int width,int height)
{
    this->height = height;
    this->width = width;
    position.X = 0;
    position.Y = height - 34;
    exit = false;
    devModeEnabler = false;
    SetWindow();

}

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

    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);    //get console handle
    SetConsoleScreenBufferSize(hconsole, coord);  //set buffer size (to fix the window size)
    SetConsoleWindowInfo(hconsole, TRUE, &Rect);  //set window size, the second parameter indicates how coordinates should be calculated.
                                                //(in this case, is from top to bottom (0 from top))

}

void Menu::howToPlay()
{
    SetConsoleTextAttribute(hconsole, 10); //13 is purple, 10 is green
    back_to_main = false;
    system("cls");

    position.Y = height - 34;

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

void Menu::GameOverMenu()
{
    SHORT CursorMenuStartPosition = 0;
    //fix red console text bug
    SetConsoleTextAttribute(hconsole, 10);
    system("cls");

    position.Y = height - 34;

    bool stop = false;
    int index = 0;

    char directory[25];
    strcpy(directory, "menu/gameover.txt");

    //change text color
    SetConsoleTextAttribute(hconsole, 13);
    printTextFile(directory, true, width, position.Y);

    //menu entries
    SetConsoleTextAttribute(hconsole, 7);
    position.X = width/2 - (73-50)/2 - 3;

    position.Y += 4;
    CursorMenuStartPosition = position.Y;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Restart";

    /*
    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Return to Main Menu";
    */
    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# Exit";

    while(!stop)
    {

        position.X = 70;

        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, 0x00);

        cout << "              " ;
        //codice del rendering della freccetta lol
        position.Y = CursorMenuStartPosition + index;

        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, 11);
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

            /*if (index == 1) //main menu
            {
                system("cls");
                MainMenu();
            }*/
            if (index == 1) //exit
            {
                exit = true;
            }
        }
    }
}

void Menu::MainMenu()
{
    //save menu entries Y in order to know where to draw the cursor
    SHORT CursorMenuStartPosition = 0;

    //title color
    SetConsoleTextAttribute(hconsole, 12); //12
    position.X = width - 73;
    position.Y = height - 34;

    bool stop = false;
    int index = 0; //index of menu cursor

    char directory[25];
    strcpy(directory, "menu/title.txt");
    printTextFile(directory, true, width, position.Y);

    SetConsoleTextAttribute(hconsole, 14); //12

    position.Y += 2;
    strcpy(directory, "menu/titlebar.txt");
    printTextFile(directory, true, width, position.Y);

    //menu entries
    SetConsoleTextAttribute(hconsole, 7);
    position.X = width/2 - (73-50)/2 - 3;

    position.Y += 2;
    CursorMenuStartPosition = position.Y;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# New Game";

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    cout << "# How To Play";

    position.Y++;
    SetConsoleCursorPosition(hconsole,position);
    //devMode entries have spaces to easily fix ghosting
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
        SetConsoleTextAttribute(hconsole, 0x00);
        cout << "              " ;

        //codice del rendering della freccetta
        position.Y = CursorMenuStartPosition + index;
        SetConsoleCursorPosition(hconsole,position);
        SetConsoleTextAttribute(hconsole, 11);
        cout << "<--  ";

        Sleep(100);

        //vai su o giu nel menu
        if(GetAsyncKeyState(VK_UP) != 0)
        {
            index--;
            if(index < 0)
            {
                index = 3;
            }
        }
        else if(GetAsyncKeyState(VK_DOWN) != 0)
        {
            index++;
            if(index == 4)
            {
                index = 0;
            }
        }
        else if(GetAsyncKeyState(VK_RETURN) !=0)
        {
            if(index == 0)
            {
                //using stop bool manipulation in order to exit from the loop and to avoid using break
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
                devModeEnabler = !devModeEnabler;
                MainMenu();
            }
            else if (index == 3)
            {
                //the exit bool is public, and can be read by the main in order to exit the application
                exit = true;
                stop = true;
            }
        }
    }
}

void Menu::printTextFile(char* directory, bool centered, int x, int y)
{
    char ch;
    int mxChar = 0;

    int lineCharCounter = 0;

    if (centered){
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


        //fstream closes automatically, but it is best practice to close when you've finished to use it
        fin.close();
    } else {
        //se il testo non deve essere centrato, allora mettilo nella posizione di default
        position.X = x;
    }
    SetConsoleCursorPosition(hconsole,position);

    //secondo filestream per stampare il documento nel gioco
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
