#include "../include/Restaurant.h"

Restaurant::Restaurant(const std::string &configFilePath): dishcounter(0),customercounter(0) {
    Restaurant::readAndSplit(configFilePath);
}

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

void Restaurant::readAndSplit(const std::string &configFilePath) {
    std::ifstream file;
    file.open(configFilePath);
    std::string line;
    int countH =0;
    int countDishes=1;
    while (!file.eof()) {
        std::getline(file, line);
        if (countH==1){
            numOfTables=line.at(0);
        }
        if (countH==2){
            Restaurant::setTables(line);
        }
        if (countH==3){
            Restaurant::setMenu(line,countDishes);
            countDishes++;
        }
        if (line.at(0) == '#') {
            countH++;
        }
    }
}

void Restaurant::setTables(std::string s) {
    int k = 1;
    int i=-1;
    while (k!=numOfTables){
        i = s.find_first_of(',' ,static_cast<unsigned>(i+1));
        Table *toAdd = new Table(s.at(static_cast<unsigned>(i-1)));
        tables.push_back(toAdd);
        k++;
    }


}

void Restaurant::setMenu(std::string s , int n) {
    int i = s.find_first_of(',');
    std::string name = s.substr(0, (static_cast<unsigned>(i-1)));
    int j = s.find_first_of(',',static_cast<unsigned>(i+1));
    auto type = static_cast<DishType>(std::stoi(s.substr(static_cast<unsigned>(i+1),static_cast<unsigned>(j-1))));
    int price = std::stoi(s.substr(static_cast<unsigned>(j+1)));
    Dish *toAdd = new Dish(n, name, price, type);
    menu.push_back(*toAdd);
}



