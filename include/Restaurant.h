#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <fstream>


class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
	void closeRestaurant();
    std::vector<Table*> getAllTables() const;
    void operator=(Restaurant& rest);
    void clear();
	~Restaurant();
    DishType stringToType (std::string s);
    void readAndSplit (const std::string& configFilePath);
    void setTables (std::string s );
    void setMenu (std::string s , int n);
    void Aopen(std::string s);
    void Aorder(std::string s);
    void Amove(std::string s);
    Customer* makeCustomer(int id, std::string name,std::string type);

private:
    bool open;
    int numOfTables;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int dishcounter;
    int customercounter;
};

#endif