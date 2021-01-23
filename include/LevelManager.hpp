#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP
#include "Car.hpp"
#include "Collectable.hpp"
#include "list.hpp"
#include <iomanip>
#include <string>
#include <time.h>

//TODO: change maxObjectOnScreen to private.
//TODO: change LevelVelocity by making it IN levelManager and OUTSIDE of the car class

class LevelManager
{
    protected:
        //spawner variables
        int timeToWaitForSpawn;
        int randomValue;
        int onScreenObjects;
        int time; //iterazioni da un momento a un altro
        int availableObjects; //oggetti che possiamo spawnare

        //the index of the level
        int levelCounter;
        int levelCounterFloor; //used to know when to start reducing the level difficulty parameters
        int pointsUpperBound;

        //enviroment aestetics functions:
        void enviromentAnimationRenderer();  //this renders the frame of the moving background
        bool frameAnimationEnvBool;          //this is used to change between frames of the background animation
        void drawBackground();               //this is used to render the background which does not move
        void gotoPos(int x, int y);          //these are used to handle the rendering without using the pixel class (for optimization)

        //UI elements
        void UIGameInfo();
        void UIGameInfoInit();

        //handle of console window
        HANDLE hConsole;

        //game parameters
        int game_speed;
        int points;
        int speed_decrementer;

        //level statistics system
        infolist* level_list;
        unsigned int list_size;
        unsigned int puddle_counter;
        unsigned int gas_tanks_counter;
        void addStats();
        int totalPoints;

        //Game mechanics functions
        void CollisionHandler(int i);
        void Spawn();
        void checkColliders(); //program events
        void manualAccelerator();
        void playerGameMechanics();


    public:
        LevelManager(HANDLE thConsole);
        LevelManager(HANDLE thConsole, Car player, Collectable gas);

        //the index of the level
        Collectable collectables[MAX_ON_SCREEN_OBJECTS]; //objects on screen;
        bool availableCollectablesIndices[MAX_ON_SCREEN_OBJECTS]; //available ""spaces"" on screen

        //devMode enabled or not.
        bool devMode;

        //deathBool
        bool isDead;
        //getter di death_bool
        bool isPlayerDead();

        //types of collectables
        Collectable gas;
        Collectable puddle;
        Collectable enemyCar;

        //the player
        Car playerCar;//("sprites/carSprite.txt", 43, 30);

        //game behaviour methods
        void Start();
        void Update();

        //function used to send stats to main menu
        infolist* getStats();
        int getTotalPoints();

        //debug functions
        int getGameSpeed();

};
#endif // LEVELMANAGER_HPP
