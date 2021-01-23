#ifndef LIST_HPP
#define LIST_HPP
#include "Pixel.hpp"

struct simple_list
{
    int index;
    simple_list *prev;
    simple_list *next;
};

struct level_info
{
    int level_number;
    int points;
    int mud;
    int gas_tanks;
};

class infolist
{
    private:
        level_info level;
        int _size;


    public:
        infolist *prev;
        infolist *next;

        infolist();
        infolist(int level_number, int points, int n_gas_tanks, int n_mud);

        void setLevelInfo(int level_number, int points, int n_gas_tanks, int n_mud);
        void printLevelInfo();

        void deleteFirst();
        //void add();
};

typedef infolist* ptr_list;

#endif // LIST_HPP
