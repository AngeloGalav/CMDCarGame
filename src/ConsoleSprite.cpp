#include "ConsoleSprite.hpp"

ConsoleSprite::ConsoleSprite()
{
    screenPosition.X = 20;
    screenPosition.Y = 5;

    pixelCount = 4;
    int i = 0;
    while (pixelCount > i){                         //questo for genera uno sprite placeholder.
        pixels[i].position.X = screenPosition.X+i;
        pixels[i].position.Y = screenPosition.Y+i;
        pixels[i].color = BACKGROUND_CYAN;
        i++;
    }
    generateCollider();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleSprite::ConsoleSprite(char* directory, int x, int y, HANDLE thConsole)
{
    screenPosition.X = x;
    screenPosition.Y = y;
    loadFromFile(directory);
    generateCollider();
    setConsoleHandle(thConsole);
}

/**
    Setta il puntatore al frame buffer dello sprite.
    @params: thConsole, puntatore al framebuffer.
*/
void ConsoleSprite::setConsoleHandle(HANDLE thConsole)
{
    hConsole = thConsole;
}

/**
*   Renderizza lo sprite su schermo.
*/
void ConsoleSprite::renderSprite()
{
    int i=0;
    while (pixelCount > i){
        pixels[i].RenderPixel(hConsole);
        i++;
    }
}

/**
 * Elimina lo sprite su schermo
 */
void ConsoleSprite::deleteSprite()
{
    int i=0;
    while (pixelCount > i)
    {
        pixels[i].BlankPixel(hConsole, pixels[i].position);
        i++;
    }
}
/**
    Carica e decodifica gli sprite da un file .txt, creando uno sprite.

    @params: directory, la posizione del file nel computer.
*/
void ConsoleSprite::loadFromFile(char* directory)
{
    pixelCount = 0;
    char ch;
    //filestream per il decoding dello sprite
    fstream fin1(directory, fstream::in);
    int i=0;
    COORD coords;
    coords.Y = 0;
    //coords.x parte da -1 per non fare un do while
    coords.X = -1;
    //decoding (lo sprite deve essere minore di 32)
    //l'intera funzione decodifica il file finchè i caratteri non sono finiti.
    while (fin1 >> noskipws >> ch) //decodifica carattere per carattere
                                   //no skipws = non si skippano gli spazi bianchi
    {
        coords.X++;
        if (ch != ' ' && ch != '\n') //salta il carattere vuoto
        {
            pixelCount++;
            if (ch == 'r')
            {   //red_backgroud
                pixels[i].color = RED_B_GREEN_F;
                pixels[i].pixChar = ' ';

            } else if (ch == 'g')
            {   //grey_backgroud
                pixels[i].color = BACKGROUND_GREY;
                pixels[i].pixChar = ' ';

            } else if (ch == 'b')
            {   //lightblue_character, red_background
                pixels[i].color = RED_B_CYAN_F;
                pixels[i].pixChar = CHAR_HIGH_SQUARE; //quadrato alto

            } else if (ch == 'y')
            {   //yellow_char, red_background
                pixels[i].color = RED_B_YELLOW_F;
                pixels[i].pixChar = CHAR_HIGH_SQUARE; //alto

            } else if (ch == 'p')
            {   //grey_char, black_background
                pixels[i].color = BLACK_B_GREY_F;
                pixels[i].pixChar = CHAR_LOW_SQUARE; //basso

            } else if (ch == 'e')
            {   //red_char, yellow_background
                pixels[i].color = RED_B_YELLOW_F;
                pixels[i].pixChar = CHAR_LOW_SQUARE;

            } else if (ch == 'x')
            {   //ocra char, brown ocra
                pixels[i].color = GOLD_B_BROWN_F;
                pixels[i].pixChar = CHAR_HIGH_SQUARE;

            } else if (ch == 'm')
            {   //ocra background
                pixels[i].color = BACKGROUND_GOLD;
                pixels[i].pixChar = ' ';

            } else if (ch == 'o')
            {   //ocra char, black background
                pixels[i].color = BLACK_B_GOLD_F;
                pixels[i].pixChar =  CHAR_LOW_SQUARE;

            } else if (ch == 's')
            {   //black char, ocra background
                pixels[i].color = BLACK_B_GOLD_F;
                pixels[i].pixChar = CHAR_HIGH_SQUARE;

            } else if (ch == 'd')
            {   //blue_background
                pixels[i].color = BLUE_B_CYAN_F;
                pixels[i].pixChar = ' ';

            } else if (ch == 'l')
            {   //lightblue_character, blue_background
                pixels[i].color = BLUE_B_CYAN_F;
                pixels[i].pixChar = (char) CHAR_HIGH_SQUARE;
            }

            pixels[i].position.Y = coords.Y + screenPosition.Y;
            pixels[i].position.X = coords.X + screenPosition.X;
            i++;
        }
        else if (ch == '\n') //la linea è terminata
        {
            coords.X = -1;
            coords.Y++;
        }
    }

    fin1.close();
}


/**
    Mette tutti i pixel a un colore.
    Usato nel lightweight renderer.
*/
void ConsoleSprite::SetColorOfAllPixels(WORD Color)
{
    for(int i = 0; i < pixelCount; i++)
    {
        pixels[i].color = Color;
    }
}

/**
    Mette tutti i pixel a un carattere.
    Usato nel lightweight renderer.
*/
void ConsoleSprite::SetCharOfAllPixel(char pixChar)
{
      for(int i = 0; i < pixelCount; i++)
    {
        pixels[i].pixChar = pixChar;
    }
}

/**
    Trasla di (x,y) l'elemento.

    @params: x e y, le coordinate di traslazione.
*/
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

/**
    Al contrario di translate, moveTo permette di posizionare un oggetto in una posizione precisa.

    @params: x e y, le coordinate al quale l'oggetto verrà spostato.
*/
void ConsoleSprite::moveTo(int x, int y)
{
    int xVariation = x - screenPosition.X;
    int yVariation = y - screenPosition.Y;

    //update della posizione
    screenPosition.X = x;
    screenPosition.Y = y;

    //collider update
    rect_collider.bottomLine += yVariation;
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


/**
    Genera/calcola i bordi dello sprite per permettere la collision detection.
*/
void ConsoleSprite::generateCollider()
{
    rect_collider.bottomLine = pixels[pixelCount-1].position.Y; //usare questo metodo per il calcolo del collider in basso è possibile grazie a come funziona
    rect_collider.topLine = pixels[0].position.Y;               //la decodifica degli sprite.

    calculate_RightLeftLine();
}

/**
    Calcola qual e' il pixel piu' esposto a destra e a sinistra dello sprite,
    in modo da poter generare i collider ponendoli su questi due pixel.
*/
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
    Renderizza i collider di ciascuno sprite su schermo.
    Usata durante la devMode per mostrare i collider.
*/
void ConsoleSprite::renderColliders()
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

/**
    Elimina il render del collider durante la devMode. Usata per impedire il ghosting
    causato dai collider durante la modalità developer.
*/
void ConsoleSprite::deleteCollider_render()
{
    COORD coord;

    SetConsoleTextAttribute(hConsole, BLACK_B_GREEN_F);
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


/** Getter del collider dell'elemento */
Collider* ConsoleSprite::getCollider_ptr()
{
    return &rect_collider;
}

/** Getter della posizione */
COORD ConsoleSprite::getPosition()
{
    return screenPosition;
}

/**
    Confronta i collider di questo oggetto e un altro per verificare
    se è avvenuta una collisione.

    @params: ptr_collider_elem, il puntatore al collider del secondo oggetto.
*/
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


///FUNZIONI DI DEBUGGING///

/**
    Stampa le informazioni di ogni singolo pixel sullo schermo.

    @params: windowCursor, il cursore usato dalla funzione.
            Il passaggio è per riferimento siccome voglio salvare
            le modifiche al cursore usato nella funzione precedente.
*/
void ConsoleSprite::printSinglePixelInfo(COORD* windowCursor)
{
    int i = 0;
    while (pixelCount > i){
        SetConsoleCursorPosition(hConsole, *windowCursor);
        cout << "pixelNumber " << i+1 << ':';
        pixels[i].printPixelInfo();
        i++;
        windowCursor->Y++;
    }
}
/**
    Stampa la posizione dello sprite.
*/
void ConsoleSprite::printSpritePosition()
{
    cout << screenPosition.X  << ", " << screenPosition.Y << " ";
}

