#include "../include/Restaurant.h"

Restaurant::Restaurant(const std::string &configFilePath): dishcounter(0),customercounter(0) {}
Restaurant::Restaurant(): dishcounter(0),customercounter(0) {}
std::vector<Dish> & Restaurant::getMenu() {
	this->menu;
}
Table* Restaurant::getTable(int ind) {
    return tables.at(ind);
}
int Restaurant::getNumOfTables() const {
    return tables.size();
}
std::vector<Table*> Restaurant::getAllTables() const{
    return tables;
}

void Restaurant::closeRestaurant() {
    open = false;
}
void Restaurant::start() {
    open = true;
}

Restaurant Restaurant::operator=(Restaurant &rest) {
    open = rest.open;
    dishcounter = rest.dishcounter;
    customercounter = rest.customercounter;
    menu = rest.menu;
    for(Table* t : rest.tables){
        Table* toAdd = new Table(*t);
        tables.push_back(toAdd);

    }
    /*
     to add the copy of the action log
     */

}

}


