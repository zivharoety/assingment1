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

void Restaurant::clear() {
for(Table* t : tables){
    delete t ;
}
for(BaseAction* b : actionsLog){
    delete b ;}
}

Restaurant Restaurant::operator=(Restaurant &rest) {
    clear();
    open = rest.open;
    dishcounter = rest.dishcounter;
    customercounter = rest.customercounter;
    for(Dish d : rest.menu){
        menu.push_back(d);
    }
    for (Table *t : rest.tables) {
        Table *toAdd = new Table(*t);
        tables.push_back(toAdd);
        for (BaseAction *b : rest.getActionsLog()) {
            actionsLog.push_back(b->clone());
        }


    }

}

Restaurant::~Restaurant() {
    clear();
}


