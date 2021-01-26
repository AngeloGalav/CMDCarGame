#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP
#include "Car.hpp"
#include "Collectable.hpp"
#include "list.hpp"
#include <iomanip>
#include <string>


struct CollectableMemory    //è true se dentro il posto è libero, false altrimenti.
{
    bool available;
    Collectable object;
};

class LevelManager
{
    protected:
        //spawner variables
        int timeToWaitForSpawn;
        int time;   //numero di iterazioni svolte ad ogni spawn.
        int availableObjects; //numero di oggetti che possiamo spawnare
        clock_t deltaTime;
        int fpsCount;
        double fps;

        //Funzioni di estetica
        void enviromentAnimationRenderer();
        bool frameAnimationEnvBool;
        void drawBackground();
        void gotoPos(int x, int y);
        bool checkRoadCenter(Collider* elem, int j);

        //UI elementi ui
        void UIGameInfo();
        void UIGameInfoInit();

        //puntatore al framebuffer
        HANDLE hConsole;

        //paramentri di gioco
        int game_speed;
        int points;
        int speed_decrementer;
        int levelCounter;
        int levelCounterFloor; //variabile usata per sapere quando smettere di aumentare la difficoltà (e quando ricominciare)
        int pointsUpperBound;


        //Sistema delle statistiche
        InfoQ level_list;
        unsigned int list_size;
        unsigned int puddle_counter;
        unsigned int gas_tanks_counter;
        void addStats();
        int totalPoints;

        //Meccaniche di gioco
        void CollisionHandler(int i);
        void Spawn();
        void checkColliders();
        void manualAccelerator();
        void playerGameMechanics();

        //Tipi di oggetti.
        Collectable gas;
        Collectable puddle;
        Collectable enemyCar;
        CollectableMemory collectables[MAX_ON_SCREEN_OBJECTS]; //Memoria contentente i riferimenti agli oggetti su schermo;
        Car playerCar;

        Collider* playerCarCol;//usato per sistemare il bug con linea di mezzeria

        IndexQ indexQueue;
        bool isDead;

    public:
        LevelManager(HANDLE thConsole);
        LevelManager(HANDLE thConsole, Car player, Collectable gas);

        //attivatore devMode
        bool devMode;
        //attivatore lightWeightMode
        bool lightWeightMode;

        bool isPlayerDead();

        //metodi ispirati alla classe MonoBehaviour di Unity
        void Start();
        void Update();

        //funzioni usate per passare i dati al menu
        InfoQ* getStats();
        int getTotalPoints();

        void displayCarsPositions(COORD* pos);

        //funzioni di debugging.
        int getGameSpeed();
        Car* getPlayerCarPtr();
        IndexQ* getQueuePtr();
};
#endif // LEVELMANAGER_HPP
