#include "..\include\List.hpp"

/**
    Da un valore allo struct contenente le info di livello.

    @params: le info di livello da dare allo struct e l'istanze dello struct stesso, passato per riferimento.
*/
void setLevelInfo(level_info* info, int level_number, int points, int n_gas_tanks, int n_mud)
{
    info->level_number = level_number;
    info->points = points;
    info->mud = n_mud;
    info->gas_tanks = n_gas_tanks;
}

//costruttori...
template <class T>
Queue<T>::Queue()
{
    coda = NULL;
    testa = NULL;
    list_size = 0;
}

template <class T>
Queue<T>::Queue(T elem)
{
    coda = new QElem<T>();

    coda->next = NULL;
    coda->prev = NULL;
    testa = coda;
    coda->elem = elem;
    list_size = 1;
}

/**
   Ritorna le dimensioni della coda. (quantità di oggetti in essa...)
*/
template <class T>
int Queue<T>::getSize()
{
    return list_size;
}

/**
   Ritorna true se la coda è vuota, false altrimenti.
*/
template <class T>
bool Queue<T>::isEmpty()
{
    return (coda == NULL && testa == NULL);
}

/**
    Mette un elemento in coda.
*/
template <class T>
void Queue<T>::enqueue(T toEnqueue)
{
    if (coda != NULL)
    {
        coda->next = new QElem<T>();
        coda->next->prev = coda;
        coda = coda->next;
        list_size++;
        coda->elem = toEnqueue;
    } else
    {
        coda = new QElem<T>();
        coda->next = NULL;
        coda->prev = NULL;
        testa = coda;
        list_size++;
        coda->elem = toEnqueue;
    }
}

/**
   Rimuove l'elemento in testa.
*/
template <class T>
T Queue<T>::dequeue()
{
    if (testa->next != NULL)
    {
        T toReturn = testa->elem;
        testa = testa->next;

        delete(testa->prev);
        testa->prev = NULL;

        list_size--;
        return toReturn;
    }
    else if (testa != NULL && testa == coda)
    {
        T toReturn = coda->elem;

        delete(coda);
        coda = NULL;
        testa = NULL;

        list_size--;
        return toReturn;
    }

    return {};  //Usato per evitare i warning...
}

/**
    Elimina l'elemento in coda.
*/
template <class T>
T Queue<T>::pop()
{
    if (coda->prev != NULL)
    {
        T toReturn = coda->elem;
        coda = coda->prev;

        delete(coda->next);
        coda->next = NULL;

        list_size--;

        return toReturn;
    }
    else if (coda != NULL && testa == coda)
    {
        T toReturn = coda->elem;

        delete(coda);
        coda = NULL;
        testa = NULL;

        list_size--;

        return toReturn;
    }

    return {};  //Usato per evitare i warning...
}


/**
    Stampa un elemento di tipo levelInfo.
*/
template<>
void Queue<level_info>::printInfo(level_info info)
{
    cout << "In level " << info.level_number;

    if (info.points >= 0) cout << ", you've made " << info.points << " pts.";
    else cout << ", you've gone down of a level.";

    cout << " You've hit " << info.mud << " puddles and " << info.gas_tanks << " gas tanks.";
}

/**
    Stampa gli elementi di una queue di int.

    Viene usato nella debugWindow nella devMode.
*/
template<>
void Queue<int>::debugPrint()
{
    QElem<int>* hd = testa;

    while (hd != NULL)
    {
        cout << hd->elem << " ";
        hd = hd->next;
    }
    cout << "    ";
}


template class Queue<int>;
template class Queue<level_info>;
