#include "../include/Restaurant.h"

Restaurant::Restaurant(const std::string &configFilePath): dishcounter(0),customercounter(0) {
    Restaurant::readAndSplit(configFilePath);
}

Restaurant::Restaurant(): dishcounter(0),customercounter(0) {}

Restaurant::Restaurant(const Restaurant &other): dishcounter(other.dishcounter), customercounter(other.customercounter) {
    for(Table * t : other.tables){
        tables.push_back(t->clone());
    }
    for(BaseAction* b : other.actionsLog){
        actionsLog.push_back(b->clone());
    }
    for(Dish d : other.menu){
        menu.push_back(d);
    } //to check

}

Restaurant::Restaurant(Restaurant &&other): numOfTables(other.numOfTables), open(other.open),tables(other.tables),actionsLog(other.actionsLog) {
    for(int i = 0 ; i<other.tables.size();i++){
        other.tables.at(i) = nullptr;
    }
    for(int i = 0 ; i<other.actionsLog.size();i++){
        other.actionsLog.at(i) = nullptr;
    }
    other.clear();
}

Restaurant& Restaurant::operator=(Restaurant &&rest) {
    clear();
    tables = rest.tables;
    actionsLog = rest.actionsLog;
    open = rest.open;
    for(Dish d : rest.menu){
        menu.push_back(d);
    }
    dishcounter = rest.dishcounter;
    customercounter = rest.customercounter;
    for(int i = 0 ; i<rest.tables.size();i++){
        rest.tables.at(i) = nullptr;
    }
    for(int i = 0 ; i<rest.actionsLog.size();i++){
        rest.actionsLog.at(i) = nullptr;
    }
    rest.clear();

}

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



    while(open) {
        getline(std::cin, line);

        if (line.substr(0, line.find_first_of(' ')).compare("open") == 0) {
            Aopen(line.substr(line.find_first_of(' ') + 1)); }
        if(line.substr(0,line.find_first_of(' ')).compare("order")==0)
            Aorder(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' ')).compare("move")==0)
            Amove(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' ')).compare("close")==0)
            Aclose(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' ')).compare("closeall")==0)
            AcloseAll();
        if(line.substr(0,line.find_first_of(' ')).compare("menu")==0)
            Amenu();
        if(line.substr(0,line.find_first_of(' ')).compare("status")==0)
            Astatus(line.substr(line.find_first_of(' ')+1));
        if(line.substr(0,line.find_first_of(' ')).compare("log")==0)
            Alog();
        if(line.substr(0,line.find_first_of(' ')).compare("backup")==0)
            Abackup();
        if(line.substr(0,line.find_first_of(' ')).compare("restore")==0)
            Arestore();
   }


}

void Restaurant::clear() {
for(Table* t : tables){
    delete t ;
}
for(Table* t : tables){
    tables.pop_back();
}
for(BaseAction* b : actionsLog) {
    delete b;
}
for(Dish d : menu){
    menu.pop_back();
}
for(BaseAction* b : actionsLog){
    actionsLog.pop_back();
}

}

Restaurant& Restaurant::operator=(Restaurant &rest) {
    clear();
    open = rest.open;
    dishcounter = rest.dishcounter;
    customercounter = rest.customercounter;
    for (Dish d : rest.menu) {
        menu.push_back(d);
    }
    for (Table *t : rest.tables) {
        tables.push_back(t->clone());
    }
     for (BaseAction *b : rest.getActionsLog()) {
         actionsLog.push_back(b->clone());
     }

       return *this ;
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

void Restaurant::Aclose(std::string s) {
    int tableId = std::stoi(s);
    Close* c = new Close(tableId);
    c->act(*this);
    actionsLog.push_back(c);
}

void Restaurant::AcloseAll() {
    CloseAll* ca = new CloseAll();
    ca->act(*this);
    actionsLog.push_back(ca);
}

void Restaurant::Amenu() {
    PrintMenu* m = new PrintMenu();
    m->act(*this);
    actionsLog.push_back(m);
}

void Restaurant::Astatus(std::string s) {
    int tableID = std::stoi(s);
    PrintTableStatus* ps = new PrintTableStatus(tableID);
    ps->act(*this);
    actionsLog.push_back(ps);
}
void Restaurant::Alog() {
    PrintActionsLog* log = new PrintActionsLog();
    log->act(*this);
    actionsLog.push_back(log);
}

void Restaurant::Abackup() {
    BackupRestaurant* bk = new BackupRestaurant();
    bk->act(*this);
    actionsLog.push_back(bk);
}

void Restaurant::Arestore() {
    RestoreResturant* restore = new RestoreResturant();
    restore->act(*this);
    actionsLog.push_back(restore);
}
