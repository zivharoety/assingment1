#include "../include/Restaurant.h"

Restaurant::Restaurant(const std::string &configFilePath): dishcounter(0),customercounter(0) {
    Restaurant::readAndSplit(configFilePath);
}

Restaurant::Restaurant(): dishcounter(1),customercounter(1) {}

std::vector<Dish> & Restaurant::getMenu() {
    return this->menu;
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
    std::cout<<"Restaurant is now open!"<<std::endl;
    std::string line;
     Aopen("2 Alice,spc Bob,alc");//~for debugging
     Aopen("3 Guy,chp");
     Aorder("2");
     Aorder("2");
    Aorder("2");
    Amove("2 3 1");

   /* while(open) {
        std::getline(std::cin , line);
        if(line.substr(0,line.find_first_of(' '))=="open")
            Aopen(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' '))=="order")
            Aorder(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' '))=="move")
            Amove(line.substr(line.find_first_of(' ')+1));
    } */

}

void Restaurant::clear() {
for(Table* t : tables){
    delete t ;
}
for(BaseAction* b : actionsLog){
    delete b ;}
}

 void Restaurant::operator=(Restaurant &rest) {
    clear();
    open = rest.open;
    dishcounter = rest.dishcounter;
    customercounter = rest.customercounter;
    for (Dish d : rest.menu) {
        menu.push_back(d);
    }
    for (Table *t : rest.tables) {
        Table *toAdd = new Table(*t);
        tables.push_back(toAdd);
    }
     for (BaseAction *b : getActionsLog()) {
         actionsLog.push_back(b->clone());
     }

     //   return this ;
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
            numOfTables= std::stoi(line);
           countH++;

        }
        else if (countH==3){
            Restaurant::setTables(line);
            countH++;

        }
        else if (countH>4){
            Restaurant::setMenu(line,countDishes);
            countDishes++;
        }
       else  if (line.at(0) == '#') {
            countH++;
        }
    }
}

void Restaurant::setTables(std::string s) {
    int k = 0;

    std::vector<int> cap ;

    while(s.size()>0 & k<numOfTables ){
        std::string toAdd = s.substr(0,s.find_first_of(','));
        Table* newTable = new Table(std::stoi(toAdd));
        tables.push_back(newTable);

        s = s.substr(s.find_first_of(',')+1) ;
        k++;
    }
    while (k!=numOfTables){


    }


}


void Restaurant::setMenu(std::string s , int n) {
    int i = static_cast<unsigned>(s.find_first_of(','));
    std::string name = s.substr(0, (static_cast<unsigned>(i)));
    s = s.substr(i+1);
    DishType type =  stringToType(s.substr(0,s.find_first_of(',')));
    s = s.substr(s.find_first_of(',')+1);
    int price = std::stoi(s);
    Dish *toAdd = new Dish(n, name, price, type);
    menu.push_back(*toAdd);
}

Restaurant::~Restaurant() {
    clear();
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
}

DishType Restaurant::stringToType(std::string s)  {
    if(s=="ALC")
        return ALC;
    if(s=="SPC")
        return SPC;
    if(s=="VEG")
        return VEG;
    if(s=="BVG")
        return BVG;
}

void Restaurant::Aopen(std::string s) {
    int tableID = std::stoi(s.substr(0,s.find_first_of(' ')));
    s=s.substr(s.find_first_of(' ')+1);
    std::vector<Customer*> cl;
    while(s != ""){
        std::string name = s.substr(0,s.find_first_of(','));
        s = s.substr(s.find_first_of(',')+1);
        std::string c_type = s.substr(0,3);
        cl.push_back(makeCustomer(customercounter,name,c_type));
        s = s.substr(3);
        if(s.size()>0 && s.at(0)==' '){
            s= s.substr(1); //changing string
        }

    }
    OpenTable *toOpen = new OpenTable(tableID, cl);
    toOpen->act(*this);
    actionsLog.push_back(toOpen);

}

Customer* Restaurant::makeCustomer(int id, std::string name, std::string type) {
    customercounter++;
    if(type=="alc"){
        Customer* toReturn = new AlchoholicCustomer(name,id);
        return  toReturn;
    }
    if(type=="spc"){
        Customer* toReturn = new SpicyCustomer(name,id);
        return toReturn;
    }
    if(type=="chp"){
        Customer* toReturn = new CheapCustomer(name,id);
        return toReturn;
    }
    if(type=="veg") {
        Customer *toReturn = new VegetarianCustomer(name, id);

        return toReturn;
    }
}

void Restaurant::Aorder(std::string s) {
    int tableid = std::stoi(s);
    Order *order = new Order(tableid);
    order->act(*this);
    actionsLog.push_back(order);

}

void Restaurant::Amove(std::string s) {
    int originT = std::stoi(s.substr(0,s.find_first_of(' ')));
    s = s.substr(s.find_first_of(' ')+1);
    int destT = std::stoi(s.substr(0,s.find_first_of(' ')));
    s = s.substr(s.find_first_of(' ')+1);
    int customerId = std::stoi(s);
    MoveCustomer* toMove = new MoveCustomer(originT,destT,customerId);
    toMove->act(*this);
    actionsLog.push_back(toMove);
}


