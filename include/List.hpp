#ifndef LIST_HPP
#define LIST_HPP
#include "Pixel.hpp"

struct level_info
{
    int level_number;
    int points;
    int mud;
    int gas_tanks;
};

//Classe lista. Funziona come una queue.

class infolist
{
    private:
        level_info level;

    public:
        infolist *prev;
        infolist *next;

        infolist();
        infolist(int level_number, int points, int n_gas_tanks, int n_mud);

        void setLevelInfo(int level_number, int points, int n_gas_tanks, int n_mud);
        void printLevelInfo();

        void deleteFirst();
        void deleteLast();
        void addElement(infolist* toAdd);
};

typedef infolist* ptr_list;

#endif // LIST_HPP
