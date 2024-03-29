#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "Car.hpp"
#include "Collectable.hpp"
#include "List.hpp"
#include <iomanip>  //usato per formattare il testo nella UI
#include <string>


struct CollectableMemory    //� true se dentro il posto � libero, false altrimenti.
{                           //questa struttura rappresenta la "cache" degli oggetti sullo schermo.
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
        int levelCounterFloor; //variabile usata per sapere quando smettere di aumentare la difficolt� (e quando ricominciare)
        int pointsUpperBound;


        //Sistema delle statistiche
        Queue<level_info> level_list;
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

        Queue<int> indexQueue;
        bool isDead;

        //debug window
        void DebugWindow();

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
        Queue<level_info>* getStats();
        int getTotalPoints();

        void displayCarsPositions(COORD* pos);
        void listCleanup();
};
#endif // LEVELMANAGER_HPP
