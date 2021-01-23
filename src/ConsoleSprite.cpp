#include "ConsoleSprite.hpp"


//placeholder sprite if directory is undefined
ConsoleSprite::ConsoleSprite()
{
    screenPosition.X = 20;
    screenPosition.Y = 5;

    pixelCount = 4;
    int i = 0;
    while (pixelCount > i){
        pixels[i].position.X = screenPosition.X+i;
        pixels[i].position.Y = screenPosition.Y+i;
        pixels[i].color = BACKGROUND_CYAN;
        i++;
    }
    generateCollider();
    object_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleSprite::ConsoleSprite(char* directory, int x, int y)
{
    screenPosition.X = x;
    screenPosition.Y = y;
    loadFromFile(directory);
    generateCollider();
}

/**
    Renderizza lo sprite su schermo.
*/
void ConsoleSprite::renderSprite(HANDLE hConsole)
{
    int i=0;
    while (pixelCount > i){
        pixels[i].RenderPixel(hConsole);
        i++;
    }
}

//elimina lo sprite su schermo
void ConsoleSprite::deleteSprite(HANDLE hConsole)
{
    int i=0;
    while (pixelCount > i)
    {
        pixels[i].BlankPixel(hConsole, pixels[i].position);
        i++;
    }
}

void ConsoleSprite::loadFromFile(char* directory)
{
    pixelCount = 0;
    char ch;
    //filestream for sprite decoding
    fstream fin1(directory, fstream::in);
    int i=0;
    COORD coords;
    coords.Y = 0;
    //coords.x parte da -1 per non fare un do while
    coords.X = -1;
    //decoding (SPRITE HAS TO BE LESS THAN 32 PIX)
    //the whole function decodes the file until the characters are finished.
    while (fin1 >> noskipws >> ch) //decoding char by char
                                   //no skipws = non si skippano gli spazi bianchi
    {                              //TODO: fai progetto empty di prova per vedere cosa sa fin1 noskipws ch etc
        coords.X++;
        if (ch != ' ' && ch != '\n') //if character is empty, then skip
        {
            pixelCount++;
            if (ch == 'r')
            {   //red_backgroud
                pixels[i].color = 193;
                pixels[i].pixChar = ' ';

            } else if (ch == 'g')
            {   //grey_backgroud
                pixels[i].color = 136;
                pixels[i].pixChar = ' ';

            } else if (ch == 'b')
            {   //lightblue_character, red_background
                pixels[i].color = 203;
                pixels[i].pixChar = (char) 223; //quadrato alto

            } else if (ch == 'y')
            {   //yellow_char, red_background
                pixels[i].color = 206;
                pixels[i].pixChar = (char) 223; //alto

            } else if (ch == 'p')
            {   //grey_char, black_background
                pixels[i].color = 8;
                pixels[i].pixChar = (char) 220; //basso

            } else if (ch == 'e')
            {   //red_char, yellow_background
                pixels[i].color = 206;
                pixels[i].pixChar = (char) 220;

            } else if (ch == 'x')
            {   //ocra char, brown ocra
                pixels[i].color = 70;
                pixels[i].pixChar = (char) 223;

            } else if (ch == 'm')
            {   //ocra background
                pixels[i].color = 102;
                pixels[i].pixChar = ' ';

            } else if (ch == 'o')
            {   //ocra char, black background
                pixels[i].color = 006;
                pixels[i].pixChar = (char) 220;

            } else if (ch == 's')
            {   //black char, ocra background
                pixels[i].color = 006;
                pixels[i].pixChar = (char) 223;

            } else if (ch == 'd')
            {   //blue_background
                pixels[i].color = 155;
                pixels[i].pixChar = ' ';

            } else if (ch == 'l')
            {   //lightblue_character, blue_background
                pixels[i].color = 155;
                pixels[i].pixChar = (char) 223;
            }

            pixels[i].position.Y = coords.Y + screenPosition.Y;
            pixels[i].position.X = coords.X + screenPosition.X;
            i++;
        }
        else if (ch == '\n') //line has ended.
        {
            coords.X = -1;
            coords.Y++;
        }
    }

    fin1.close();
}

//move to a certain position
void ConsoleSprite::translate(int x, int y)
{
    //update the position
    screenPosition.X += x;
    screenPosition.Y += y;
    //fixing colliders
    rect_collider.bottomLine += y;
    rect_collider.topLine += y;
    rect_collider.leftLine += x;
    rect_collider.rightLine += x;

    int i=0;
    while (pixelCount > i)
    {
        pixels[i].position.X += x;
        pixels[i].position.Y += y;
        i++;
    }
}

//al contrario di translate, moveTo permette di mettere un oggetto in una posizione precisa
void ConsoleSprite::moveTo(int x, int y)
{
    int xVariation = x - screenPosition.X;
    int yVariation = y - screenPosition.Y;

    //position update
    screenPosition.X = x;
    screenPosition.Y = y;

    //collider fix/update
    rect_collider.bottomLine += yVariation; //(+1 perche comincia da 0)
    rect_collider.topLine += yVariation;
    rect_collider.leftLine += xVariation;
    rect_collider.rightLine += xVariation;

    int i=0;
    while (pixelCount > i)
    {
        pixels[i].position.X += xVariation;
        pixels[i].position.Y += yVariation;
        i++;
    }
}


//generates/calculates the boundaries of the sprite to optimize collision
void ConsoleSprite::generateCollider()
{
    rect_collider.bottomLine = pixels[pixelCount-1].position.Y; //possible thanks to how sprite decoding works (last pixel is always bottom)
    rect_collider.topLine = pixels[0].position.Y;

    calculate_RightLeftLine();
}

//separeted the calculations of the RightLine so it looks cleaner
void ConsoleSprite::calculate_RightLeftLine()
{
    int mx = 0;
    int mn = 0;
    for (int i=0; i < pixelCount; i++)
    {
        //finds the farthest pixel on the right and on the left, so that even unusual shapes are supported.
        if (pixels[mx].position.X < pixels[i].position.X)
        {
            mx = i;
        }

        if (pixels[mn].position.X > pixels[i].position.X)
        {
            mn = i;
        }
    }

    rect_collider.rightLine = pixels[mx].position.X;
    rect_collider.leftLine = pixels[mn].position.X;
}

/**
    Funzione usata per renderizzare i collider di ciascuno sprite su schermo.
*/
void ConsoleSprite::renderColliders(HANDLE hConsole)
{
    COORD coord;

    SetConsoleTextAttribute(hConsole, GREEN_B_RED_F);
    coord.X = rect_collider.leftLine;
    coord.Y = rect_collider.topLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << "o";
    coord.X = rect_collider.rightLine;
    coord.Y = rect_collider.bottomLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << "o";
    coord.X = rect_collider.leftLine;
    coord.Y = rect_collider.bottomLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << "o";
    coord.X = rect_collider.rightLine;
    coord.Y = rect_collider.topLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << "o";
}


void ConsoleSprite::deleteCollider_render(HANDLE hConsole)
{
    COORD coord;

    SetConsoleTextAttribute(hConsole, BACKGROUND_BLACK);
    coord.X = rect_collider.leftLine;
    coord.Y = rect_collider.topLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << " ";
    coord.X = rect_collider.rightLine;
    coord.Y = rect_collider.bottomLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << " ";
    coord.X = rect_collider.leftLine;
    coord.Y = rect_collider.bottomLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << " ";
    coord.X = rect_collider.rightLine;
    coord.Y = rect_collider.topLine;
    SetConsoleCursorPosition(hConsole, coord);
    cout << " ";
}

Collider* ConsoleSprite::getCollider_ptr()
{
    return &rect_collider;
}

COORD ConsoleSprite::getPosition()
{
    return screenPosition;
}

void ConsoleSprite::printSpritePosition()
{
    cout << screenPosition.X  << ", " << screenPosition.Y << " ";
}

bool ConsoleSprite::checkCollision(Collider* ptr_collider_elem)
{
       return ((rect_collider.topLine <= ptr_collider_elem->bottomLine && rect_collider.topLine >= ptr_collider_elem->topLine)
        || (rect_collider.bottomLine <= ptr_collider_elem->bottomLine && rect_collider.bottomLine >= ptr_collider_elem->topLine))
        &&  ((rect_collider.leftLine >= ptr_collider_elem->leftLine && rect_collider.leftLine <= ptr_collider_elem->rightLine)
        || (rect_collider.rightLine <= ptr_collider_elem->rightLine && rect_collider.rightLine >= ptr_collider_elem->leftLine)
        //test in piu aggiunto nel caso l'oggetto sia piu piccolo del secondo.
        || (ptr_collider_elem->rightLine >=  rect_collider.leftLine && ptr_collider_elem->rightLine <=  rect_collider.rightLine)
        || (ptr_collider_elem->leftLine >=  rect_collider.leftLine && ptr_collider_elem->leftLine <=  rect_collider.rightLine));
}


/**
    DEBUG STUFF
*/

//debugInfo, functions used for debugging (unused in final version/nonDev mode)
COORD ConsoleSprite::printSinglePixelInfo(HANDLE hConsole, COORD windowCursor)
{
    int i = 0;
    while (pixelCount > i){
        SetConsoleCursorPosition(hConsole, windowCursor);
        cout << "pixelNumber " << i+1 << ':';
        pixels[i].printPixelInfo();
        i++;
        windowCursor.Y++;
    }

    return windowCursor;

}

void ConsoleSprite::printAddressDebug()
{
    cout << &pixels[pixelCount-1].position.Y << ", " << rect_collider.bottomLine;
}
