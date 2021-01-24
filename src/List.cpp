#include "List.hpp"

InfoList::InfoList(){
    prev = NULL;
    next = NULL;
    setLevelInfo(1, 0, 0, 0);
}

InfoList::InfoList(int level_number, int points, int n_gas_tanks, int n_mud)
{
    prev = NULL;
    next = NULL;
    setLevelInfo(level_number, points, n_gas_tanks, n_mud);
}


void InfoList::setLevelInfo(int level_number, int points, int n_gas_tanks, int n_mud)
{
    level.level_number = level_number;
    level.points = points;
    level.mud = n_mud;
    level.gas_tanks = n_gas_tanks;
}


void InfoList::printLevelInfo()
{
    cout << "In level " << level.level_number << ", you've made " << level.points << " pts.";
    cout << " You've hit " << level.mud << " puddles and " << level.gas_tanks << " gas tanks." ;
}

void InfoList::deleteFirst()
{
    InfoList* hd = this;

    while (hd->prev != NULL)
    {
        hd = hd->prev;
    }

    hd->next->prev = NULL;
    delete(hd);
}

void InfoList::addElement(InfoList* toAdd)
{
    this->next = toAdd;
    this->next->prev = this;
}

//Inizio funzioni di freeIndexQ

IndexQ::IndexQ()
{
    coda = NULL;
    testa = NULL;
}

IndexQ::IndexQ(int index)
{
    coda = new biList();

    coda->next = NULL;
    coda->prev = NULL;
    testa = coda;
    coda->index = index;
}

void IndexQ::enqueue(int toEnqueue)
{
    if (coda != NULL)
    {
        coda->next = new biList();
        coda->next->prev = coda;
        coda = coda->next;

        coda->index = toEnqueue;
    } else
    {
        coda = new biList();
        coda->next = NULL;
        coda->prev = NULL;
        testa = coda;
        coda->index = toEnqueue;
    }
}

int IndexQ::dequeue()
{
    if (testa->next != NULL)
    {
        biList* tmp = testa;
        testa = testa->next;
        testa->prev = NULL;

        int toReturn = tmp->index;
        delete(tmp);
        return toReturn;
    }
    else if (testa != NULL && testa == coda)
    {
        int toReturn = testa->index;
        testa = NULL;
        coda = NULL;
        return toReturn;
    }
    else return -1;
}

void IndexQ::debugPrint()
{
    biList* hd = testa;

    while (hd != NULL)
    {
        cout << hd->index << " ";
        hd = hd->next;
    }
    cout << "    ";
}
