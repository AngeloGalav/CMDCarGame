#include "LevelManager.hpp"

//toDO: hConsole trimming.

LevelManager::LevelManager(HANDLE thConsole)
{
    hConsole = thConsole;

    char filePath[4][128];

    strcpy(filePath[0], "sprites/carSprite.txt");
    strcpy(filePath[1], "sprites/oilSprite.txt");
    strcpy(filePath[2], "sprites/holeSprite.txt");
    strcpy(filePath[3], "sprites/carSprite2.txt");

    //inizializzazione oggetti
    playerCar = Car(filePath[0], 43, 30, hConsole);
    gas = Collectable(filePath[1], 300, Gas, hConsole);
    puddle = Collectable(filePath[2], -100, Puddle, hConsole);
    enemyCar = Collectable(filePath[3], -1000, EnemyCar, hConsole);
}

double clockToMilliseconds(clock_t ticks){
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks/(double)CLOCKS_PER_SEC)*1000.0;
}
/**
    Metodo che viene chiamato all'inizio del gioco.
*/
void LevelManager::Start()
{
    //reinizializzazione dei valori di gioco, così che nulla cambi al restart del gioco
    availableObjects = MAX_ON_SCREEN_OBJECTS;
    time = 0;
    isDead = false;

    game_speed = SPEED_LIMIT;
    points = 0;
    levelCounter = 1;
    levelCounterFloor = 0;
    list_size = 0;
    timeToWaitForSpawn = MIN_TIME_TO_WAIT_SPAWN;

    frameAnimationEnvBool = true;

    if (lightWeightMode)  //se sono in lightMode, metto gli sprite più light, in stile ASCII
    {
        playerCar.SetColorOfAllPixels(BLACK_B_CYAN_F);
        playerCar.SetCharOfAllPixel('@');

        enemyCar.SetColorOfAllPixels(BLACK_B_RED_F);
        enemyCar.SetCharOfAllPixel('o');

        gas.SetColorOfAllPixels(BLACK_B_GREEN_F);
        gas.SetCharOfAllPixel('#');

        puddle.SetColorOfAllPixels(BLACK_B_YELLOW_F);
        puddle.SetCharOfAllPixel('_');
    }

    playerCarCol = playerCar.getCollider_ptr();

    puddle_counter = 0;
    gas_tanks_counter = 0;
    level_list = Queue<level_info>();
    totalPoints = 0;

    pointsUpperBound = 1000;

    playerCar.setBoundaries(LEFT_SCREEN_BOUNDARY + 1, RIGHT_SCREEN_BOUNDARY - 1,
                            UPPER_SCREEN_BOUNDARY + 1, LOWER_SCREEN_BOUNDARY - 1);

    indexQueue = Queue<int>();

    for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
    {
        indexQueue.enqueue(i);
        collectables[i].available = true;
        collectables[i].object = Collectable();
    }

    drawBackground();

    UIGameInfoInit();
    playerCar.renderSprite();

    fpsCount = 0;
    deltaTime = 0;
    fps = 0;
}

/**
    Metodo che si ripete ad ogni frame.
*/
void LevelManager::Update()
{
    clock_t beginFrame = clock(); //misuro il tempo di un frame per il calcolo degli fps
    Sleep(game_speed);

    UIGameInfo();

    //codice per l'eliminazione dei collider, per togliere il ghosting (questo va solo se siamo in devMode)
    if (devMode)
    {
        playerCar.deleteCollider_render();

        for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
        {
            //in ogni slot occupato elimino i collider. (per impedire il ghosting)
            if (!collectables[i].available) collectables[i].object.deleteCollider_render();
        }
    }

    if (devMode) manualAccelerator();

    playerGameMechanics();

    time++;

    //se non sono in lightMode, attiva l'animazione carina che da il senso di velocità
    if (!lightWeightMode) enviromentAnimationRenderer();

    if (time >= timeToWaitForSpawn && availableObjects > 0)
    {
        Spawn();
        time = 0;
    }

    playerCar.optimized_Movement();

    for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
    {
        if (!collectables[i].available)
        {
            collectables[i].object.Movement();

            //visualizza i collider di ciascun elemento.
            if (devMode) collectables[i].object.renderColliders();
        }
    }

    //visualizzazione dei collider della macchinina.
    if (devMode) playerCar.renderColliders();


    checkColliders();

    //tolgo il ghosting.
    playerCar.renderSprite(); //TODO: DELETE GHOSTING

    clock_t endFrame = clock();

    deltaTime += endFrame - beginFrame;
    fpsCount++;

    if (clockToMilliseconds(deltaTime) > 1000.0){   //calcolo fps
        fps = (((double) fpsCount)/deltaTime) * 10000.0;
        deltaTime = 0;
        fpsCount = 0;
    }

    if (devMode) DebugWindow();
}

/**
    Funzione che crea e aggiorna le meccaniche di gioco, in particolare si occupa di aumentare il livello,
    la difficolt�, etc.. (oppure di diminuirli).

*/
void LevelManager::playerGameMechanics()
{
    int prev_game_speed = game_speed;

    //aggiunto letteralmente solo perche ci dava fastidio vedere 129 e non 130 per velocita
    //(inoltre in questo modo la velocità diminuisce in modo omogeneo...)
    if (game_speed == 21 && !devMode) game_speed = 20;

    if (points >= pointsUpperBound) //next level
    {
        //update della StatList...
        addStats();

        points -= pointsUpperBound;
        levelCounter++;

        if (timeToWaitForSpawn > MAX_TIME_TO_WAIT_SPAWN) timeToWaitForSpawn--; //il tempo di spawning diminuisce.. (aumento difficoltà)

        if (game_speed - 20 > 0 && !devMode) game_speed -= 20;  //se in devMode, non ti diminuisce la velocità quando scendi di livello.
        else if (game_speed - 20 < 0 && !devMode) game_speed = 1;

        //punto in cui la difficolt� finisce di aumentare, e quindi aumento il levelCounter floor.
        if (timeToWaitForSpawn == MAX_TIME_TO_WAIT_SPAWN && game_speed == prev_game_speed) levelCounterFloor++;
    }
    else if (points < 0) //previous level
    {
        //update della StatList...
        addStats();

        points =  pointsUpperBound + points;

        if (levelCounter >= 2) levelCounter--;
        else if (!devMode) isDead = true; //se il tuo livello è minore di 1, allora sei morto. (MA NON SE USI LA DEVMODE)

        //se la condizione � vera, abbiamo raggiunto la difficolt� massima
        if (levelCounterFloor >= 1) levelCounterFloor--;
        else
        {    //posso abbassare la difficolt� solo se levelCounterFloor è 0.
            if (timeToWaitForSpawn < MIN_TIME_TO_WAIT_SPAWN) timeToWaitForSpawn++;

            if (game_speed < SPEED_LIMIT ) game_speed += 20;
            else game_speed = SPEED_LIMIT;
        }
    }
}

/**
    Funzione che abilita l'accelerazione manuale, usata in devMode.
*/
void LevelManager::manualAccelerator()
{
    if (GetAsyncKeyState(KEY_E) && game_speed < SPEED_LIMIT && devMode) game_speed++;
    else if (GetAsyncKeyState(KEY_Q) && game_speed > 1 && devMode) game_speed--;
}

/**
    Funzione che controlla i collider di ciascun oggetto nel gioco, per vedere se sbatte contro la macchina.
    E' lento, ma � anche il metodo pi� preciso per registrare le collisioni.

    Se ho una collisione, passo il controllo a CollisionHandler
*/
void LevelManager::checkColliders()
{
    for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
    {
        if (!collectables[i].available) //per ogni elemento sullo schermo...
        {
            Collider* col_ptr = collectables[i].object.getCollider_ptr();

            if (playerCar.checkCollision(col_ptr)) //...guarda se c'è una collisione.
            {
                if (collectables[i].object.getTypeOfCollectable() == Gas) gas_tanks_counter++;
                else if (collectables[i].object.getTypeOfCollectable() == Puddle) puddle_counter++;

                CollisionHandler(i); //se c'è una collisione, passo il controllo al collisionHandler.
                points += collectables[i].object.getEffect();

                if (collectables[i].object.getEffect() > 0) totalPoints += collectables[i].object.getEffect();
            }

            if (col_ptr->bottomLine > LOWER_SCREEN_BOUNDARY - 2 && col_ptr->bottomLine < DELETED_COL - 1)
                CollisionHandler(i); //Se l'elemento scompare dello schermo, è come se fosse avvenuta un collisione.
        }
    }
}

/**
    Funzione che spawna un oggetto in posizione random dalla parte pi� alta del framebuffer.

    Per funzionare, guarda se c'� un posto libero fra i collectables di gioco e poi gli viene cos� assegnato un
    ID (l'indice del availableCollectablesIndixes).

    Dopodich�, viene renderizzato.
*/

void LevelManager::Spawn()
{
    int k = -1;
    if (!indexQueue.isEmpty()) k = indexQueue.dequeue(); //prendi il primo indice della queue
    int randomValue = rand() % 3;

    //se dopo tutti questi passaggi, k � != -1 (quindi si � trovato un posto libero) finalizza lo spawn
    if (k != -1)
    {
        collectables[k].available = false;
        if (randomValue == 0) collectables[k].object = gas;
        else if (randomValue == 1) collectables[k].object = puddle;
        else if (randomValue == 2) {collectables[k].object = enemyCar; collectables[k].object.randomDir();}

        randomValue = rand() % 54 + (LEFT_SCREEN_BOUNDARY + 1);
        collectables[k].object.moveTo(randomValue, 0);

        Collider* coll = collectables[k].object.getCollider_ptr();
        if (coll->leftLine <= ROAD_CENTER && ROAD_CENTER <= coll->rightLine) collectables[k].object.moveTo(ROAD_CENTER + 1, 0);

        //fixa un bug che "cancellava" una parte del lato sinistro della strada durante la lightWeightMode.
        if (collectables[k].object.getTypeOfCollectable() == EnemyCar && randomValue <= LEFT_SCREEN_BOUNDARY + 1
            && collectables[k].object.getDir() != 0 )
            {   collectables[k].object.moveTo(LEFT_SCREEN_BOUNDARY + 2, 0);}

        availableObjects--;

        //its ALIVEEE!
        //Posso renderizzare l'oggetto.
        collectables[k].object.renderSprite();
    }
}

/**
    Elimina l'elemento i nel piano di gioco che ha subito una collisione.
*/
//funzione chiamata ogni volta che avviene una collisione
void LevelManager::CollisionHandler(int i)
{
    //elimina l'immagine del collider (i quattro punti ai vertici) se in devMode quando avviene una collisione...
    if (devMode) collectables[i].object.deleteCollider_render();

    collectables[i].object.Collision();

    indexQueue.enqueue(i);

    //libera lo spazio in i
    collectables[i].available = true;

    //update dei dati di gioco
    availableObjects++;
}


///FUNCTIONS FOR LEVEL AESTETIC///

/**
    Posiziona il cursore del renderer nelle coordinate x e y.
*/
void LevelManager::gotoPos(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    //posiziono il cursore per disegnare sulla console
    SetConsoleCursorPosition(hConsole, coord);
}

/**
    Crea la animazione ai bordi della strada e nella riga
    di mezzeria, dando un senso di velocit� al gioco.
*/
void LevelManager::enviromentAnimationRenderer()
{
    if (frameAnimationEnvBool){
        for (int j = 0; j < LOWER_SCREEN_BOUNDARY; j++)
        {
            gotoPos(LEFT_SCREEN_BOUNDARY, j);

            if (j % 2 == 0) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            else if (j % 2 != 0) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);

            cout << ' ';

            gotoPos(ROAD_CENTER, j);
            SetConsoleTextAttribute(hConsole, BLACK_B_WHITE_F);

            //parte del codice che permette
            //alle macchine di essere disegnate sopra alla linea di mezzeria.
            bool checked = true;

            for (int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
            {
                if (collectables[i].object.getDir()!= 0)
                {
                    if (!(checkRoadCenter(collectables[i].object.getCollider_ptr(), j)
                        || collectables[i].available)) //se una macchinina si trova sopra la riga, NON PUOI DISEGNARCI SOPRA.
                    {
                        checked = false;
                        i = MAX_ON_SCREEN_OBJECTS;
                    }
                }
            }

            if (checked && checkRoadCenter(playerCarCol, j))//faccio anche il check con la macchina del giocatore, per vedere se lui è
                                                            //sulla riga.
            {
                if (j % 2 == 0) cout << '|';
                else if (j % 2 != 0) cout << ' ';
            }

            gotoPos(RIGHT_SCREEN_BOUNDARY, j);

            if (j % 2 == 0) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            else if (j % 2 != 0) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);

            cout << ' ';
        }
    }
    else
    {
        for (int j = 0; j < LOWER_SCREEN_BOUNDARY; j++)
        {
            gotoPos(LEFT_SCREEN_BOUNDARY, j);

            if (j % 2 == 0) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            else if (j % 2 != 0)SetConsoleTextAttribute(hConsole, BACKGROUND_RED);

            cout << ' ';

            gotoPos(ROAD_CENTER, j);
            SetConsoleTextAttribute(hConsole, BLACK_B_WHITE_F);

            //parte del codice che permette
            //alle macchine di essere disegnate sopra alla linea di mezzeria.
            bool checked = true;

            for (int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
            {
                if (collectables[i].object.getDir()!= 0)
                {
                    if (!(checkRoadCenter(collectables[i].object.getCollider_ptr(), j)
                        || collectables[i].available))  //se una macchinina si trova sopra la riga, NON PUOI DISEGNARCI SOPRA.
                    {
                        checked = false;
                        i = MAX_ON_SCREEN_OBJECTS;
                    }
                }
            }

            if (checked && checkRoadCenter(playerCarCol, j)) //faccio anche il check con la macchina del giocatore, per vedere se lui è
                                                            //sulla riga.
            {
                if (j % 2 != 0) cout << '|';
                else if (j % 2 == 0) cout << ' ';
            }

            gotoPos(RIGHT_SCREEN_BOUNDARY, j);

            if (j % 2 == 0) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            else if (j % 2 != 0) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);

            cout << ' ';
        }
    }
    frameAnimationEnvBool = !frameAnimationEnvBool;
}

/**
    Guarda se può disegnare la riga di mezzeria nella riga j in base
    in base ai collider dell'oggetto che si vuole controllare.
*/
bool LevelManager::checkRoadCenter(Collider* elem, int j)
{
    if (elem->leftLine < ROAD_CENTER && elem->rightLine > ROAD_CENTER)
    {
        if (((elem->topLine > j) && (elem->bottomLine > j)) || ((elem->topLine < j) && (elem->bottomLine < j)))
        { return true;}

        return false;

    } else return true;
}

/**
  Stampa lo sfondo verde del gioco.
*/
void LevelManager::drawBackground()
{
    for (int i = 0; i < SCREEN_WIDTH; i++){
        for (int j = 0; j < SCREEN_HEIGHT - 1; j++){

            gotoPos(i, j);

            if (j % 2 == 0 && (i == LEFT_SCREEN_BOUNDARY || i == RIGHT_SCREEN_BOUNDARY)) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            else if (j % 2 != 0 && (i == LEFT_SCREEN_BOUNDARY || i == RIGHT_SCREEN_BOUNDARY)) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            else if (i > LEFT_SCREEN_BOUNDARY && i < RIGHT_SCREEN_BOUNDARY) SetConsoleTextAttribute(hConsole, BLACK_B_WHITE_F);
            else SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);

            cout << " ";
        }
    }
}

//UI elements

/**
    Aggiorna e posiziona la UI del gioco, contente le varie informazioni.
*/

void LevelManager::UIGameInfo()
{
    COORD window_position;
    window_position.X = UI_POS_X;
    window_position.Y = UI_POS_Y;

    SetConsoleCursorPosition(hConsole, window_position);
    SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);

    cout << "      Game Info";
    window_position.Y += 2;
    SetConsoleCursorPosition(hConsole, window_position);

    cout << " SPEED: " << setw(6) << (SPEED_LIMIT + 50) - game_speed << " km/h";
    window_position.Y += 2;
    SetConsoleCursorPosition(hConsole, window_position);

    if (points < 0) SetConsoleTextAttribute(hConsole, BLACK_B_RED_F);
    else SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);

    cout << " Level: " << setw(11) << levelCounter;
    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);

    cout << " Score: " << setw(11) << points;
    window_position.Y++;
    SetConsoleCursorPosition(hConsole, window_position);

    SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);
    cout << " FPS: " << setw(13) << ((int)fps )/ 10.0;
}

/**
    Inizializza la UI, creando il quadrato nero delle informazioni
    sul gioco, e stampando "Good Luck!".
*/
void LevelManager::UIGameInfoInit()
{
    int j = 10;
    for (int i = UI_POS_X; i < UI_POS_X + UI_WIDTH; i++)
    {
        for (j = UI_POS_Y; j < UI_POS_Y + UI_HEIGHT; j++)
        {
            gotoPos(i, j);
            SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);
            cout << " ";
        }
    }
    SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);
    gotoPos(UI_POS_X + 5, j - 2);
    cout << "Good Luck!";
}


/**
    Ritorna il puntatore alle statistiche finali sul livello
*/
Queue<level_info>* LevelManager::getStats()
{
    return &level_list;
}

/**
    Ritorna il numero totale di punti.
*/
int LevelManager::getTotalPoints()
{
    return totalPoints;
}

/**
    Aggiunge un nuovo elemento alla lista delle statistische.
*/
void LevelManager::addStats()
{
    level_info toAdd;
    setLevelInfo(&toAdd, levelCounter, points, gas_tanks_counter, puddle_counter);
    level_list.enqueue(toAdd);
    gas_tanks_counter = 0;
    puddle_counter = 0;
    list_size++;

    if (level_list.getSize() >= MAX_LIST_SIZE)
    {
        level_list.dequeue();
    }
}

/**Getter dello stato del giocatore. Usato per capire quando far comparire il menu di gameover.*/
bool LevelManager::isPlayerDead()
{
    return isDead;
}

///FUNZIONI DI DEBUGGING///


/**
    Mostra le informazioni di debugging su schermo durante la devMode.
*/
void LevelManager::DebugWindow()
{
    Car* playerObject = &playerCar;
    Queue<int>* debugger = &indexQueue;

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
