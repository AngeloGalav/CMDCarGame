#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP
#include "Car.hpp"
#include "Collectable.hpp"
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <time.h>

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

        void CollisionHandle(int i);

        int points;


    public:
        //debug, to change when it's ready (to private)
        int maxOnScreenObjects;

        LevelManager(HANDLE thConsole);
        LevelManager(HANDLE thConsole, Car player, Collectable oil);

        //the index of the level
        Collectable collectables[20]; //objects on screen;
        bool availableCollectablesIndexes[20]; //available ""spaces"" on screen

        //devMode enabled or not.
        bool devMode;

        //deathBool
        bool deathBool;

        //types of collectables
        Collectable oil;
        Collectable mud;
        Collectable enemyCar;

        //the player
        Car playerCar;//("sprites/carSprite.txt", 43, 30);

        //game behaviour methods
        void Start();
        void Update();
        void Spawn();

        //collision physics handler
        void checkColliders(); //program events

};
#endif // LEVELMANAGER_HPP
