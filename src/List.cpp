#include "List.hpp"

InfoQ::InfoQ(){
    coda = NULL;
    testa = NULL;
    list_size = 0;

    setLevelInfo(&error_lvl, -1, -1, -1, -1);
}

InfoQ::InfoQ(int level_number, int points, int n_gas_tanks, int n_mud)
{
    coda = new info_bilist();
    testa = coda;
    setLevelInfo(&(coda->info), level_number, points, n_gas_tanks, n_mud);
    list_size = 1;

    setLevelInfo(&error_lvl, -1, -1, -1, -1);
}

void InfoQ::setLevelInfo(level_info* info, int level_number, int points, int n_gas_tanks, int n_mud)
{
    info->level_number = level_number;
    info->points = points;
    info->mud = n_mud;
    info->gas_tanks = n_gas_tanks;
}

void InfoQ::printInfo(level_info info)
{
    cout << "In level " << info.level_number;
    if (info.points >= 0) cout << ", you've made " << info.points << " pts.";
    else cout << ", you've gone down of a level.";

    cout << " You've hit " << info.mud << " puddles and " << info.gas_tanks << " gas tanks.";
}

int InfoQ::getSize()
{
    return list_size;
}

bool InfoQ::isEmpty()
{
    return (coda == NULL && testa == NULL);
}

level_info InfoQ::dequeue()
{
    if (testa->next != NULL)
    {
        info_bilist* tmp = testa;
        testa = testa->next;
        testa->prev = NULL;

        level_info toReturn = tmp->info;

        list_size--;

        delete(tmp);
        return toReturn;
    }
    else if (testa != NULL && testa == coda)
    {
        level_info toReturn = testa->info;
        testa = NULL;
        coda = NULL;

        list_size--;

        return toReturn;
    }

    return error_lvl;
}

level_info InfoQ::pop()
{
    if (coda->prev != NULL)
    {
        info_bilist* tmp = coda;
        coda = coda->prev;
        coda->next = NULL;

        level_info toReturn = tmp->info;
        delete(tmp);

        list_size--;

        return toReturn;
    }
    else if (coda != NULL && testa == coda)
    {
        level_info toReturn = coda->info;
        testa = NULL;
        coda = NULL;

        list_size--;

        return toReturn;
    }
    return error_lvl;
}

void InfoQ::enqueueInfo(int level_number, int points, int n_gas_tanks, int n_mud)
{
    if (coda != NULL)
    {
        coda->next = new info_bilist();
        coda->next->prev = coda;
        coda = coda->next;

        list_size++;
        setLevelInfo(&(coda->info), level_number, points, n_gas_tanks, n_mud);
    } else
    {
        coda = new info_bilist();
        coda->next = NULL;
        coda->prev = NULL;
        testa = coda;

        list_size++;
        setLevelInfo(&(coda->info), level_number, points, n_gas_tanks, n_mud);
    }
}


//Inizio funzioni di IndexQ

IndexQ::IndexQ()
{
    coda = NULL;
    testa = NULL;
}

IndexQ::IndexQ(int index)
{
    coda = new index_bilist();

    coda->next = NULL;
    coda->prev = NULL;
    testa = coda;
    coda->index = index;
}

void IndexQ::enqueue(int toEnqueue)
{
    if (coda != NULL)
    {
        coda->next = new index_bilist();
        coda->next->prev = coda;
        coda = coda->next;

        coda->index = toEnqueue;
    } else
    {
        coda = new index_bilist();
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
        index_bilist* tmp = testa;
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
    index_bilist* hd = testa;

    while (hd != NULL)
    {
        cout << hd->index << " ";
        hd = hd->next;
    }
    cout << "    ";
}
