#include "LevelManager.hpp"

//TODO: rework devMode

//TODO: rework game_collider disappereance

//TODO: CollisionFunctions

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
    oil = Collectable(filePath[1], 0, 0, 300);
    mud = Collectable(filePath[2], 0, 0, -100);
    enemyCar = Collectable(filePath[3], 0, 0, -1000);

    playerCar.setBoundaries(11,69,1,42);
}

/**
    Metodo che viene chiamato all'inizio del gioco.
*/
void LevelManager::Start()
{
    //reinitialization of game values, so that nothing changes when the player restarts
    maxOnScreenObjects = 5;
    availableObjects = maxOnScreenObjects;
    onScreenObjects = 0;

    time = 0;
    isDead = false;

    game_speed = 100;
    points = 0;
    levelCounter = 0;
    levelCounterFloor = 0;
    speed_limit = 120;
    maxTimeToWaitForSpawn = 6;
    minTimeToWaitForSpawn = 16;
    timeToWaitForSpawn = minTimeToWaitForSpawn;

    frameAnimationEnvBool = true;

    pointsUpperBound = 1000;

    for(int i=0; i<maxOnScreenObjects; i++)
    {
        availableCollectablesIndices[i] = true;
    }

    drawBackground();

    UIGameInfoInit();
    playerCar.RenderObject(hConsole);

    playerCar.initPointerCollider();

}

/**
    Metodo che si ripete ad ogni frame.
*/
void LevelManager::Update()
{
    Sleep(game_speed);

    UIGameInfo();

    //code for collider visualization in DevMode (in this case, we set cancel the previus "collider image")
    if (devMode)
    {
        //we dont need to set an attribute, since it's already set to black by UIGameInfo();
        //car collider image clearance

        gotoPos(playerCar.collider.leftLine, playerCar.collider.topLine);
        cout << " ";
        gotoPos(playerCar.collider.rightLine, playerCar.collider.bottomLine);
        cout << " ";
        gotoPos(playerCar.collider.leftLine, playerCar.collider.bottomLine);
        cout << " ";
        gotoPos(playerCar.collider.rightLine, playerCar.collider.topLine);
        cout << " ";

        for(int i=0; i<maxOnScreenObjects; i++)
        {
           if (!availableCollectablesIndices[i]){

                //sprite collider image clearance
                gotoPos(collectables[i].collider.leftLine, collectables[i].collider.topLine);
                cout << " ";
                gotoPos(collectables[i].collider.rightLine, collectables[i].collider.bottomLine);
                cout << " ";
                gotoPos(collectables[i].collider.leftLine, collectables[i].collider.bottomLine);
                cout << " ";
                gotoPos(collectables[i].collider.rightLine, collectables[i].collider.topLine);
                cout << " ";
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

    if (time >= timeToWaitForSpawn && availableObjects > 0){
        Spawn();
        time = 0;
    }

    for(int i=0; i<maxOnScreenObjects; i++)
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

    //playerCar.Movement(hConsole);

    playerCar.Movement_debug(hConsole);


    //collider visualization in devMode for sprites for car
    if (devMode)
    {
        playerCar.renderColliders_fromptr(hConsole);
    }

    checkColliders();

    //eliminates annoying ghosting effect
    playerCar.RenderObject(hConsole); //TODO: DELETE GHOSTING
}

/**
    Funzione che crea e aggiorna le meccaniche di gioco, in particolare si occupa di aumentare il livello,
    la difficoltà, etc.. (oppure di diminuirli).

*/

void LevelManager::playerGameMechanics()
{
    //aggiunto letteralmente solo perche ci dava fastidio vedere 129 e non 130 per velocita
    //TODO: Game_speed adjustment.
    if (game_speed == 21)
    {
        game_speed = 20;
    }

    if (points >= pointsUpperBound) //next level
    {

        points -= pointsUpperBound;
        levelCounter++;

        if (timeToWaitForSpawn > maxTimeToWaitForSpawn){
            timeToWaitForSpawn -= 2;
        }

        if (game_speed - 20 > 0 && !devMode) //speedbounds
        {
            game_speed -= 20;
        }
        else
        {
            game_speed = 1;
        }

        if (timeToWaitForSpawn <= maxTimeToWaitForSpawn && game_speed <= 1)
        { //punto in cui la difficoltà finisce di aumentare
            levelCounterFloor++;
        }
    }

    else if (points < 0) //previous level

    {
        points =  pointsUpperBound + points;

        if (levelCounter >= 2)
        {
            levelCounter--;
        }
        else if (!devMode) //se il livello torna a 0, allora muori se points < 0
        {
            isDead = true;
        }

        if (levelCounterFloor > 1)
        { //se la condizione è vera, abbiamo raggiunto la difficoltà massima

            levelCounterFloor--;
        }
        else
        {    //posso abbassare la difficoltà solo se il livello

            if (timeToWaitForSpawn < minTimeToWaitForSpawn)
            {
                timeToWaitForSpawn += 2;
            }

            if (game_speed + 20 < speed_limit ) //speed bounds
            {
                game_speed += 20;
            }
            else
            {
                game_speed = speed_limit;
            }
        }
    }
}

/**
    Funzione che abilita l'accelerazione manuale, usata in devMode.
*/
void LevelManager::manualAccelerator()
{
    if (GetAsyncKeyState(81) && game_speed < 125 && devMode) game_speed++; //81 e 69 sono i keycode di E e Q rispettivamente.
    else if (GetAsyncKeyState(69) && game_speed > 1 && devMode) game_speed--;
}


/**
    Funzione che controlla i collider di ciascun oggetto nel gioco, per vedere se sbatte contro la macchina.

    Se ho una collisione, passo il controllo a CollisionHandler
*/
void LevelManager::checkColliders()
{

    for(int i=0; i<maxOnScreenObjects; i++)
    {
        //check elements that are on screen
        if (!availableCollectablesIndices[i]){

            //element got hit by car

            //the collision detection tests are made taking count of how the coordinates of the screen work (from 0 to x from left to right, from 0 to y from top to bottom)
            if ((playerCar.collider.topLine <= collectables[i].collider.bottomLine && playerCar.collider.topLine >= collectables[i].collider.topLine)
                || (playerCar.collider.bottomLine <= collectables[i].collider.bottomLine && playerCar.collider.bottomLine >= collectables[i].collider.topLine))
                {
                if ((playerCar.collider.leftLine >= collectables[i].collider.leftLine && playerCar.collider.leftLine <= collectables[i].collider.rightLine)
                    || (playerCar.collider.rightLine <= collectables[i].collider.rightLine && playerCar.collider.rightLine >= collectables[i].collider.leftLine)
                    //test in piu aggiunto nel caso l'oggetto sia piu piccolo della macchinina
                    || (collectables[i].collider.rightLine >=  playerCar.collider.leftLine && collectables[i].collider.rightLine <=  playerCar.collider.rightLine)
                    || (collectables[i].collider.leftLine >=  playerCar.collider.leftLine && collectables[i].collider.leftLine <=  playerCar.collider.rightLine))
                {
                    CollisionHandler(i);
                    points += collectables[i].effect;
                }
            }

            //element goes offscreen
            if (collectables[i].collider.bottomLine >= 43 && collectables[i].collider.bottomLine < 998)
            {
                CollisionHandler(i);
            }

        }
    }
}

/**
    Funzione che spawna un oggetto in posizione random dalla parte più alta del framebuffer.

    Per funzionare, guarda se c'è un posto libero fra i collectables di gioco e poi gli viene così assegnato un
    ID (l'indice del availableCollectablesIndixes).

    Dopodiché, viene renderizzato.
*/

void LevelManager::Spawn()
{
    int k=-1;
    randomValue = rand() % 3;

    int j=0;


    //find the first available space (true if available, false if not)
    while  (j < maxOnScreenObjects)
    {
        if (availableCollectablesIndices[j])
        {

            k = j; //k è l'id che scelgo
            availableCollectablesIndices[j] = false;    //la cella ora non è più disponibile (perché presa)
            j = maxOnScreenObjects; //Uscita da loop senza break
        }

        j++;
    }

    //se dopo tutti questi passaggi, k è != -1 (quindi si è trovato un posto libero) finalizza lo spawn
    if (k != -1)
    {
        if (randomValue == 0){
            collectables[k] = oil;
        }
        else if (randomValue == 1){
            collectables[k] = mud;
        }
        else if (randomValue == 2){
            collectables[k] = enemyCar;
        }


        randomValue = rand() % 54 + 11;
        collectables[k].moveTo(randomValue, 1);

        availableObjects--;
        onScreenObjects++;

        //its ALIVEEE!
        //Posso renderizzare l'oggetto.
        collectables[k].RenderObject(hConsole);
    }

}

/**
    Elimina l'elemento i nel piano di gioco che ha subito una collisione.
*/

//funzione chiamata ogni volta che avviene una collisione
void LevelManager::CollisionHandler(int i)
{
    //elimina l'immagine del collider (i quattro punti ai vertici) se in devMode quando avviene una collisione...
    if (devMode)
    {
        SetConsoleTextAttribute(hConsole, 7);
        gotoPos(collectables[i].collider.leftLine, collectables[i].collider.topLine);
        cout << " ";
        gotoPos(collectables[i].collider.rightLine, collectables[i].collider.bottomLine);
        cout << " ";
        gotoPos(collectables[i].collider.leftLine, collectables[i].collider.bottomLine);
        cout << " ";
        gotoPos(collectables[i].collider.rightLine, collectables[i].collider.topLine);
        cout << " ";
    }

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
    di mezzeria, dando un senso di velocità al gioco.
*/
void LevelManager::enviromentAnimationRenderer()
{
    if (frameAnimationEnvBool){
        for (int j = 0; j < 43; j++)
        {
            gotoPos(10,j);
            if (j % 2 == 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            } else if (j % 2 != 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            }
            cout << ' ';
            gotoPos(40,j);
            SetConsoleTextAttribute(hConsole, 7);
            if (j % 2 == 0){
                cout << '|';
            } else if (j % 2 != 0){
                cout << ' ';
            }
            gotoPos(70,j);
            if (j % 2 == 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            } else if (j % 2 != 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            }
            cout << ' ';
        }
    }
    else
    {
        for (int j = 0; j < 43; j++)
        {
            gotoPos(10,j);
            if (j%2 == 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            } else if (j%2 != 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            }
            cout << ' ';
            gotoPos(40,j);
            SetConsoleTextAttribute(hConsole, 7);
            if (j % 2 != 0){
                cout << '|';
            } else if (j % 2 == 0){
                cout << ' ';
            }
            gotoPos(70,j);
            if (j % 2 == 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            } else if (j % 2 != 0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            }
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
    for (int i=0; i<113; i++){
        for (int j=0; j<43; j++){
            gotoPos(i,j);
            if (j%2==0 && (i==10 || i==70)){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            } else if (j%2!=0 && (i==10 || i==70)){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            } else if (i>10 && i<70){
                SetConsoleTextAttribute(hConsole, 0x00);
            } else{
                SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
            }
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
    window_position.X = 80;
    window_position.Y = 10;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    SetConsoleTextAttribute(hConsole, 14);

    cout << "      Game Info";
    window_position.Y++;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << " SPEED: " << setw(6) << 150 - game_speed << " km/h";
    window_position.Y++;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

    cout << " Level: " << setw(11) << levelCounter;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << " Score: " << setw(11) << points;

    gotoPos(80,18);
    cout << "LF:" << levelCounterFloor << " TTW:" << timeToWaitForSpawn  << " sp:" << game_speed << "   ";

}

/**
    Inizializza la UI, creando il quadrato nero delle informazioni
    sul gioco, e stampando "Good Luck!".
*/
void LevelManager::UIGameInfoInit()
{
    int j = 10;
    for (int i=80; i < 100; i++)
    {
        for (j=10; j<20; j++)
        {
            gotoPos(i,j);
            SetConsoleTextAttribute(hConsole, 14);
            cout << " ";
        }
    }

    //85
    gotoPos(85,j-2);
    cout << "Good Luck!";
}

bool LevelManager::isPlayerDead()
{
    return isDead;
}

//DEBUG FUNCTIONS

/**
    Getter della velocità di gioco. Usato per debugging.
*/
int LevelManager::getGameSpeed()
{
    return game_speed;
}
