#ifndef LIST_HPP
#define LIST_HPP
#include "Pixel.hpp"
#include <iomanip>

//struct delle informazioni di livello.
struct level_info   ///DO NOT DELETE THIS!!!!!!!!
{
    int level_number;
    int points;
    int mud;
    int gas_tanks;
};

void setLevelInfo(level_info* info, int level_number, int points, int n_gas_tanks, int n_mud);

//bilist delle informazioni di livello.
struct info_bilist
{
    level_info info;
    info_bilist *next;
    info_bilist *prev;
};

//coda delle informazioni di livello.
class InfoQ
{
    private:
        info_bilist* coda;
        info_bilist* testa;
        void setLevelInfo(level_info* info, int level_number, int points, int n_gas_tanks, int n_mud);
        level_info error_lvl; // se la funzione ritorna questo, vuol dire che c'e' stato un errore.
        int list_size;

    public:
        InfoQ();
        InfoQ(int level_number, int points, int n_gas_tanks, int n_mud);

        int getSize();
        bool isEmpty();

        void printInfo(level_info info);
        void enqueueInfo(int level_number, int points, int n_gas_tanks, int n_mud);

        level_info dequeue();
        level_info pop();
};

//lista degli indici (usata per indicare gli indici liberi in levelManager).
struct index_bilist
{
    unsigned int index;
    index_bilist *next;
    index_bilist *prev;
};


class IndexQ
{
    protected:
        index_bilist* coda;
        index_bilist* testa;

    public:
        IndexQ();
        IndexQ(int index);

        void debugPrint();
        void enqueue(int toEnqueue);
        int dequeue();
};

///DELETE UP OF THIS////

//elemento generico di una queue
template <class T>
struct QElem
{
    T elem;
    QElem<T> *next;
    QElem<T> *prev;
};

template <class T>
class Queue
{
    protected:
        QElem<T>* coda;
        QElem<T>* testa;
        int list_size;

    public:
        Queue();
        Queue(T elem);

        int getSize();
        bool isEmpty();

        T dequeue();
        T pop();
        void enqueue(T toEnqueue);

        //type specific functions
        void printInfo(level_info info);
        void debugPrint();
};


#endif // LIST_HPP
