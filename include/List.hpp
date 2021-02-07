#ifndef LIST_HPP
#define LIST_HPP
#include "Pixel.hpp"

//struct delle informazioni di livello.
struct level_info
{
    int level_number;
    int points;
    int mud;
    int gas_tanks;
};

void setLevelInfo(level_info* info, int level_number, int points, int n_gas_tanks, int n_mud);

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
