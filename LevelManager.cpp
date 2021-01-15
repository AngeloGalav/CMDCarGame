#include "LevelManager.hpp"

//TODO: rework devMode

LevelManager::LevelManager(HANDLE thConsole)
{
    devMode = false;
    maxOnScreenObjects = 5;
    availableObjects = maxOnScreenObjects;
    onScreenObjects = 0;

    points = 0;
    levelCounter = 0;
    levelCounterFloor = 0;

    timeToWaitForSpawn = 16;
    time = 0;
    hConsole = thConsole;
    frameAnimationEnvBool = true;

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

    //enables the abilility to accelerate with E and Q
    playerCar.enableAccelerator(devMode);

    //u alive when u start :)
    deathBool = false;
}


void LevelManager::Start()
{
    //reinitialization of game values, so that nothing changes when the player restarts
    maxOnScreenObjects = 5;
    availableObjects = maxOnScreenObjects;
    onScreenObjects = 0;
    playerCar.speed = 100;

    points = 0;
    levelCounter = 0;

    timeToWaitForSpawn = 16;
    time = 0;

    playerCar.enableAccelerator(devMode);
    pointsUpperBound = 1000;

    for(int i=0; i<maxOnScreenObjects; i++)
    {
        availableCollectablesIndexes[i] = true;
    }

    drawBackground();

    UIGameInfoInit();
    playerCar.RenderObject(hConsole);

}

void LevelManager::Update()
{
    Sleep(playerCar.speed);

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
           if (!availableCollectablesIndexes[i]){

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

    //aggiunto letteralmente solo perche ci dava fastidio vedere 129 e non 130 per velocita
    if (playerCar.speed == 21)
    {
        playerCar.speed = 20;
    }

    if (points >= pointsUpperBound) //next level
    {

        points -= pointsUpperBound;
        levelCounter++;

        if (timeToWaitForSpawn > 6){
            timeToWaitForSpawn -= 2;
        }

        if (playerCar.speed - 20 > 0 && !devMode) //speedbounds
        {
            playerCar.speed -= 20;
        }
        else
        {
            playerCar.speed = 1;
        }

        if (timeToWaitForSpawn <= 6 && playerCar.speed <= 1){ //punto in cui la difficoltà finisce di aumentare
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
            deathBool = true;
        }

        if (levelCounterFloor > 1)
        { //se la condizione è vera, abbiamo raggiunto la difficoltà massima

            levelCounterFloor--;
        }
        else
        {    //posso abbassare la difficoltà solo se il livello

            if (timeToWaitForSpawn < 16)
            {
                timeToWaitForSpawn += 2;
            }

            int tmp = playerCar.getSpeed();

            if (tmp + 20 < 120 ) //speed bounds
            {
                playerCar.setSpeed(tmp + 20);
            }
            else
            {
                playerCar.setSpeed(120);
            }
        }
    }

    time++;
    //renders the background animaiton
    enviromentAnimationRenderer();

    if (time >= timeToWaitForSpawn && availableObjects > 0){
        Spawn();
        time = 0;
    }


    //dcheckColliders();
    //playerCar.Movement(hConsole);

    for(int i=0; i<maxOnScreenObjects; i++)
    {
        if (!availableCollectablesIndexes[i])
        {

            collectables[i].moveForward(hConsole);

            //collider visualization in devMode for sprites
            if (devMode)
            {
                SetConsoleTextAttribute(hConsole, 172);
                gotoPos(collectables[i].collider.leftLine, collectables[i].collider.topLine);
                cout << "o";
                gotoPos(collectables[i].collider.rightLine, collectables[i].collider.bottomLine);
                cout << "o";
                gotoPos(collectables[i].collider.leftLine, collectables[i].collider.bottomLine);
                cout << "o";
                gotoPos(collectables[i].collider.rightLine, collectables[i].collider.topLine);
                cout << "o";
            }
        }
    }

    playerCar.Movement(hConsole);

    //collider visualization in devMode for sprites for cars
    if (devMode)
    {
        SetConsoleTextAttribute(hConsole, 164);
        gotoPos(playerCar.collider.leftLine, playerCar.collider.topLine);
        cout << "o";
        gotoPos(playerCar.collider.rightLine, playerCar.collider.bottomLine);
        cout << "o";
        gotoPos(playerCar.collider.leftLine, playerCar.collider.bottomLine);
        cout << "o";
        gotoPos(playerCar.collider.rightLine, playerCar.collider.topLine);
        cout << "o";
    }

    checkColliders();

    //eliminates annoying ghosting effect
    playerCar.RenderObject(hConsole);
}

void LevelManager::checkColliders()
{

    for(int i=0; i<maxOnScreenObjects; i++)
    {
        //check elements that are on screen
        if (!availableCollectablesIndexes[i]){

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
                    CollisionHandle(i);
                    points += collectables[i].effect;
                }
            }

            //element goes offscreen
            if (collectables[i].collider.bottomLine >= 43 && collectables[i].collider.bottomLine < 998)
            {
                CollisionHandle(i);
            }

        }
    }
}

void LevelManager::Spawn()
{
    int k=-1;
    randomValue = rand() % 3;

    int j=0;


    //find the first available space (true if available, false if not)
    while  (j < maxOnScreenObjects)
    {
        if (availableCollectablesIndexes[j])
        {
            k=j;
            availableCollectablesIndexes[j] = false;//the cell is now unavailable (because it's used)
            j=maxOnScreenObjects; //a way out of the loop without using break
        }

        j++;
    }

    //se dopo tutti questi passaggi, k è != -1 (quindi si è trovato un posto libero) finalizza lo spawn
    if (k!=-1)
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
        collectables[k].RenderObject(hConsole);
    }

}

//funzione chiamata ogni volta che avviene una collisione
void LevelManager::CollisionHandle(int i)
{
    //deletes the sprite of the collider which is shown during devMode
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
    availableCollectablesIndexes[i] = true;
    availableObjects++;
    onScreenObjects--;

}



//////FUNCTIONS FOR LEVEL AESTETIC//////

void LevelManager::gotoPos(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    //posiziono il cursore per disegnare sulla console
    SetConsoleCursorPosition(hConsole, coord);
}


void LevelManager::enviromentAnimationRenderer()
{
    if (frameAnimationEnvBool){
        for (int j=0; j<43; j++)
        {
            gotoPos(10,j);
            if (j%2==0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            } else if (j%2!=0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            }
            cout << ' ';
            gotoPos(40,j);
            SetConsoleTextAttribute(hConsole, 7);
            if (j%2==0){
                cout << '|';
            } else if (j%2!=0){
                cout << ' ';
            }
            gotoPos(70,j);
            if (j%2==0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            } else if (j%2!=0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            }
            cout << ' ';
        }
    }
    else
    {
        for (int j=0; j<43; j++)
        {
            gotoPos(10,j);
            if (j%2==0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            } else if (j%2!=0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            }
            cout << ' ';
            gotoPos(40,j);
            SetConsoleTextAttribute(hConsole, 7);
            if (j%2!=0){
                cout << '|';
            } else if (j%2==0){
                cout << ' ';
            }
            gotoPos(70,j);
            if (j%2==0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_WHITE);
            } else if (j%2!=0){
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            }
            cout << ' ';
        }
    }
    frameAnimationEnvBool = !frameAnimationEnvBool;
}

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
    cout << " SPEED: " << setw(6) << 150 - playerCar.getSpeed() << " km/h";
    window_position.Y++;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);

    cout << " Level: " << setw(11) << levelCounter;
    window_position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), window_position);
    cout << " Score: " << setw(11) << points;

    gotoPos(80,18);
    cout << "LF:" << levelCounterFloor << " TTW:" << timeToWaitForSpawn  << " sp:" << playerCar.getSpeed() << "   ";

}

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
