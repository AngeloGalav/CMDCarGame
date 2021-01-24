#include "LevelManager.hpp"

//toDO: hConsole trimming.
//todo: levelProgression bug.
//TODO: levelMax setting.

LevelManager::LevelManager(HANDLE thConsole)
{
    hConsole = thConsole;

    char filePath[5][128];

    strcpy(filePath[0], "sprites/carSprite.txt");
    strcpy(filePath[1], "sprites/oilSprite.txt");
    strcpy(filePath[2], "sprites/holeSprite.txt");
    strcpy(filePath[3], "sprites/carSprite2.txt");

    //inizializzazione oggetti
    playerCar = Car(filePath[0], 43, 30);
    gas = Collectable(filePath[1], 300, Gas);
    puddle = Collectable(filePath[2], -100, Puddle);
    enemyCar = Collectable(filePath[3], -1000, EnemyCar);
}

/**
    Metodo che viene chiamato all'inizio del gioco.
*/
void LevelManager::Start()
{
    //reinizializzazione dei valori di gioco, così che nulla cambi al restart del gioco
    availableObjects = MAX_ON_SCREEN_OBJECTS;
    onScreenObjects = 0;

    time = 0;
    isDead = false;

    game_speed = SPEED_LIMIT;
    points = 0;
    levelCounter = 1;
    levelCounterFloor = 0;
    list_size = 0;
    timeToWaitForSpawn = MIN_TIME_TO_WAIT_SPAWN;

    frameAnimationEnvBool = true;

    puddle_counter = 0;
    gas_tanks_counter = 0;
    level_list = new infolist();
    totalPoints = 0;

    pointsUpperBound = 1000;

    playerCar.setBoundaries(LEFT_SCREEN_BOUNDARY + 1, RIGHT_SCREEN_BOUNDARY - 1,
                            UPPER_SCREEN_BOUNDARY + 1, LOWER_SCREEN_BOUNDARY - 1);

    for(int i=0; i < MAX_ON_SCREEN_OBJECTS; i++)
    {
        availableCollectablesIndices[i] = true;
        collectables[i] = Collectable();
    }

    drawBackground();

    UIGameInfoInit();
    playerCar.renderSprite(hConsole);
}

/**
    Metodo che si ripete ad ogni frame.
*/
void LevelManager::Update()
{
    Sleep(game_speed);

    UIGameInfo();

    playerCar.renderSprite(hConsole);

    //code for collider visualization in DevMode (in this case, we cancel the previus "collider image")
    if (devMode)
    {
        //car collider image clearance
        playerCar.deleteCollider_render(hConsole);

        for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
        {
            //ogni slot che non � disponibile � occupato.
            if (!availableCollectablesIndices[i])
            {
                collectables[i].deleteCollider_render(hConsole);
            }
        }
    }

    if (devMode)
    {
        manualAccelerator();
    }

    playerGameMechanics();

    time++;

    //renders the background animaiton
    enviromentAnimationRenderer();

    if (time >= timeToWaitForSpawn && availableObjects > 0)
    {
        Spawn();
        time = 0;
    }

    for(int i = 0; i < MAX_ON_SCREEN_OBJECTS; i++)
    {
        if (!availableCollectablesIndices[i])
        {
            collectables[i].moveForward(hConsole);

            //collider visualization in devMode for sprites
            if (devMode)
            {
                collectables[i].renderColliders(hConsole);
            }
        }
    }

    playerCar.Movement(hConsole);

    //collider visualization in devMode for sprites for car
    if (devMode)
    {
        playerCar.renderColliders(hConsole);
    }

    checkColliders();

    //eliminates annoying ghosting effect
    playerCar.renderSprite(hConsole); //TODO: DELETE GHOSTING
}

/**
    Funzione che crea e aggiorna le meccaniche di gioco, in particolare si occupa di aumentare il livello,
    la difficolt�, etc.. (oppure di diminuirli).

*/
void LevelManager::playerGameMechanics()
{
    int prev_game_speed = game_speed;

    //aggiunto letteralmente solo perche ci dava fastidio vedere 129 e non 130 per velocita
    if (game_speed == 21 && !devMode)
    {
        game_speed = 20;
    }

    if (points >= pointsUpperBound) //next level
    {
        //update the StatList...
        addStats();

        points -= pointsUpperBound;
        levelCounter++;

        if (timeToWaitForSpawn > MAX_TIME_TO_WAIT_SPAWN)
        {
            timeToWaitForSpawn--;
        }

        if (game_speed - 20 > 0 && !devMode) //speedbounds
        {
            game_speed -= 20;
        }
        else
        {
            game_speed = 1;
        }

        if (timeToWaitForSpawn == MAX_TIME_TO_WAIT_SPAWN && game_speed == prev_game_speed)
        { //punto in cui la difficolt� finisce di aumentare
            levelCounterFloor++;
        }
    }
    else if (points < 0) //previous level
    {
        //update the StatList...
        addStats();

        points =  pointsUpperBound + points;

        if (levelCounter >= 2)
        {
            levelCounter--;
        }
        else if (!devMode) //se il livello torna a 0, allora muori se points < 0
        {
            isDead = true;
        }

        if (levelCounterFloor >= 1)
        { //se la condizione � vera, abbiamo raggiunto la difficolt� massima

            levelCounterFloor--;
        }
        else
        {    //posso abbassare la difficolt� solo se il livello

            if (timeToWaitForSpawn < MIN_TIME_TO_WAIT_SPAWN)
            {
                timeToWaitForSpawn++;
            }

            if (game_speed < SPEED_LIMIT ) //speed bounds
            {
                game_speed += 20;
            }
            else
            {
                game_speed = SPEED_LIMIT;
            }
        }
    }
}



/**
    Funzione che abilita l'accelerazione manuale, usata in devMode.
*/
void LevelManager::manualAccelerator()
{
    if (GetAsyncKeyState(KEY_E) && game_speed < SPEED_LIMIT && devMode) game_speed++; //81 e 69 sono i keycode di E e Q rispettivamente.
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
        //check elements that are on screen
        if (!availableCollectablesIndices[i])
        {
            //element got hit by car
            Collider* col_ptr = collectables[i].getCollider_ptr();

            if (playerCar.checkCollision(col_ptr))
            {
                if (collectables[i].getTypeOfCollectable() == Gas) gas_tanks_counter++;
                else if (collectables[i].getTypeOfCollectable() == Puddle) puddle_counter++;

                CollisionHandler(i);
                points += collectables[i].getEffect();
                if (collectables[i].getEffect() > 0) totalPoints += collectables[i].getEffect();
            }

            //element goes offscreen
            if (col_ptr->bottomLine > LOWER_SCREEN_BOUNDARY - 1 && col_ptr->bottomLine < DELETED_COL - 1)
            {
                CollisionHandler(i);
            }
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
    randomValue = rand() % 3;

    int j = 0;

    //find the first available space (true if available, false if not)
    while  (j < MAX_ON_SCREEN_OBJECTS)
    {
        if (availableCollectablesIndices[j])
        {

            k = j; //k � l'id che scelgo
            availableCollectablesIndices[j] = false;    //la cella ora non � pi� disponibile (perch� presa)
            j = MAX_ON_SCREEN_OBJECTS; //Uscita da loop senza break
        }
        j++;
    }

    //se dopo tutti questi passaggi, k � != -1 (quindi si � trovato un posto libero) finalizza lo spawn
    if (k != -1)
    {
        if (randomValue == 0) collectables[k] = gas;
        else if (randomValue == 1) collectables[k] = puddle;
        else if (randomValue == 2) collectables[k] = enemyCar;

        randomValue = rand() % 54 + 11;
        collectables[k].moveTo(randomValue, 1); //possibile seg fault?

        availableObjects--;
        onScreenObjects++;

        //its ALIVEEE!
        //Posso renderizzare l'oggetto.
        collectables[k].renderSprite(hConsole);
    }
}

/**
    Elimina l'elemento i nel piano di gioco che ha subito una collisione.
*/

//funzione chiamata ogni volta che avviene una collisione
void LevelManager::CollisionHandler(int i)
{
    //elimina l'immagine del collider (i quattro punti ai vertici) se in devMode quando avviene una collisione...
    if (devMode) collectables[i].deleteCollider_render(hConsole);

    collectables[i].Collision(hConsole);

    //libera lo spazio in i
    availableCollectablesIndices[i] = true;

    //update dei dati di gioco
    availableObjects++;
    onScreenObjects--;
}



//////FUNCTIONS FOR LEVEL AESTETIC//////

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
            SetConsoleTextAttribute(hConsole, 7);

            if (j % 2 == 0) cout << '|';
            else if (j % 2 != 0) cout << ' ';

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
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLACK);

            if (j % 2 != 0) cout << '|';
            else if (j % 2 == 0) cout << ' ';

            gotoPos(RIGHT_SCREEN_BOUNDARY, j);

            if (j % 2 == 0) SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            else if (j % 2 != 0) SetConsoleTextAttribute(hConsole, BACKGROUND_RED);

            cout << ' ';
        }
    }

    frameAnimationEnvBool = !frameAnimationEnvBool;
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
            else if (i > LEFT_SCREEN_BOUNDARY && i < RIGHT_SCREEN_BOUNDARY) SetConsoleTextAttribute(hConsole, BACKGROUND_BLACK);
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

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);

    cout << "      Game Info";
    window_position.Y++;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << " SPEED: " << setw(6) << (SPEED_LIMIT + 50) - game_speed << " km/h";
    window_position.Y++;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

    cout << " Level: " << setw(11) << levelCounter;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << " Score: " << setw(11) << points;

    /*
    gotoPos(80,18);
    cout << "LF:" << levelCounterFloor << " TTW:" << timeToWaitForSpawn  << " sp:" << game_speed << "   ";
    */
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

    //85
    SetConsoleTextAttribute(hConsole, BLACK_B_YELLOW_F);
    gotoPos(UI_POS_X + 5, j - 2);
    cout << "Good Luck!";
}


/**
    Ritorna il puntatore alle statistiche finali sul livello
*/
infolist* LevelManager::getStats()
{
    return level_list;
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
    level_list->setLevelInfo(levelCounter, points, gas_tanks_counter, puddle_counter);
    gas_tanks_counter = 0;
    puddle_counter = 0;
    level_list->addElement(new infolist());
    level_list = level_list->next;
    list_size++;

    if (list_size > MAX_LIST_SIZE)
    {
        level_list->deleteFirst();
        list_size--;
    }
}

bool LevelManager::isPlayerDead()
{
    return isDead;
}

//DEBUG FUNCTIONS
/**
    Getter della velocit� di gioco. Usato per debugging.
*/
int LevelManager::getGameSpeed()
{
    return game_speed;
}
