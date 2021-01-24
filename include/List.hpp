#ifndef LIST_HPP
#define LIST_HPP
#include "Pixel.hpp"
#include <iomanip>

struct level_info
{
    int level_number;
    int points;
    int mud;
    int gas_tanks;
};

//Classe lista. Funziona come una queue.

class InfoList
{
    private:
        level_info level;

    public:
        InfoList *prev;
        InfoList *next;

        InfoList();
        InfoList(int level_number, int points, int n_gas_tanks, int n_mud);

        void setLevelInfo(int level_number, int points, int n_gas_tanks, int n_mud);
        void printLevelInfo();

        void deleteFirst();
        void deleteLast();
        void addElement(InfoList* toAdd);
};


struct biList
{
    unsigned int index;
    biList *next;
    biList *prev;
};

//lista degli indici liberi in levelManager.
class IndexQ
{
    protected:
        biList* coda;
        biList* testa;

    public:
        IndexQ();
        IndexQ(int index);

        void debugPrint();
        void enqueue(int toEnqueue);
        int dequeue();
};


#endif // LIST_HPP
