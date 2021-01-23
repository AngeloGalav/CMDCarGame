#include "List.hpp"

infolist::infolist(){
    prev = NULL;
    next = NULL;
    level.level_number = 1;
    level.points = 0;
    level.mud = 0;
    level.gas_tanks = 0;
}

infolist::infolist(int level_number, int points, int n_gas_tanks, int n_mud)
{
    next = NULL;
    setLevelInfo(level_number, points, n_gas_tanks, n_mud);
}


void infolist::setLevelInfo(int level_number, int points, int n_gas_tanks, int n_mud)
{
    level.level_number = level_number;
    level.points = points;
    level.mud = n_mud;
    level.gas_tanks = n_gas_tanks;
}


void infolist::printLevelInfo()
{
    cout << "In level " << level.level_number << ", you've made " << level.points << " pts.";
    cout << " You've hit " << level.mud << " puddles and " << level.gas_tanks << " gas tanks." ;
}
