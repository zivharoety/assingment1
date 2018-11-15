//
// Created by guy ofeck on 02/11/2018.
//

#include "../include/Action.h"
#include "../include/Restaurant.h"

extern Restaurant* backup;

BaseAction::BaseAction():status(PENDING),errorMsg("") {
}

BaseAction::~BaseAction() {}

void BaseAction::setStatus(ActionStatus a) {
    status = a ;
}
ActionStatus BaseAction::getStatus() const {
    return this->status;
}
//std::string BaseAction::toString() const {}
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id),customers(customersList) {

}

void OpenTable::act(Restaurant &restaurant) {
    if ((restaurant.getNumOfTables()<tableId) ||
        (restaurant.getTable(this->tableId)->isOpen()) |
        (((unsigned)restaurant.getTable(this->tableId)->getCapacity()) < ((unsigned)this->customers.size()))) {
        error("Table is already open or does not exist");
        std::cout<<getErrorMsg()<<std::endl;
        for(Customer * c : customers){
            delete c;
        }
        customers.clear();
        return;
    }
    std::string s = "";
    Table* t = restaurant.getTable(tableId);
    t->openTable();
    for(Customer* c : customers){
        t->addCustomer(c->clone());
        s = s + " " + c->getName() + ","+c->getType();
    }
    setString(s);
    complete();

}
std::string OpenTable::toString() const {
    std::string s = "open "+ std::to_string(tableId);

    return s +desc+" "+ getStringStatus() +" "+getErrorMsg();
}

void OpenTable::setString(std::string s) {
    desc = s;
}
OpenTable::~OpenTable() {
   for(Customer * c : customers){
       delete c;
   };

}
OpenTable* OpenTable::clone() const {
    std::vector<Customer *> cl ;
    for(Customer * c : customers){
        cl.push_back(c->clone());
    }
    OpenTable* toReturn = new OpenTable(tableId,cl);
    toReturn->setString(desc);
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;


}

Order::Order(int id): tableId(id) {
}
void Order::act(Restaurant &restaurant) {
    if(((tableId) > (restaurant.getNumOfTables())) | (!restaurant.getTable(tableId)->isOpen())){
        error("Table Does not exist or is not open");
        std::cout<<"Table Does not exist or is not open"<<std::endl;
    }
    else {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
}
std::string Order::toString() const {
    return "order "+std::to_string(tableId) +" "+ getStringStatus()+ " " + getErrorMsg();
}
int Order::getTableId() const {
    return tableId;
}

Order* Order::clone() const {
    Order* toReturn = new Order(this->getTableId());
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;

}
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src),dstTable(dst),id(customerId) {}

std::string MoveCustomer::toString() const {
    return "move " + std::to_string(srcTable) + " " + std::to_string(dstTable)+" "+std::to_string(id) + " "+getStringStatus();
}


MoveCustomer* MoveCustomer::clone() const {
    MoveCustomer* toReturn = new MoveCustomer(srcTable,dstTable,id);
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}
void MoveCustomer::act(Restaurant &restaurant) {
    if((((unsigned) srcTable) > ((unsigned) restaurant.getNumOfTables())) ||
    (((unsigned)  dstTable) > ((unsigned) restaurant.getNumOfTables()) )||
    (!restaurant.getTable(srcTable)->isOpen()) |
    (!restaurant.getTable(dstTable)->isOpen()) |
    (((unsigned) restaurant.getTable(dstTable)->getCustomers().size()+1) > ((unsigned) restaurant.getTable(dstTable)->getCapacity()))|
    (!restaurant.getTable(srcTable)->isThere(id)))
     {
        std::cout << "Cannot move customer"<<std::endl;
        error("Cannot move customer");

    }
    else {
        Customer* toMove = restaurant.getTable(srcTable)->getCustomer(id);
        restaurant.getTable(dstTable)->addCustomer(toMove);
        restaurant.getTable(srcTable)->removeCustomer(id);
        if((unsigned) restaurant.getTable(srcTable)->getCustomers().size() == 0){
            restaurant.getTable(srcTable)->closeTable();
         }
    complete();
    }

}

Close::Close(int id): tableId(id){};

void Close::act(Restaurant &restaurant) {
    if((tableId) > (restaurant.getNumOfTables()) ||(!restaurant.getTable(tableId)->isOpen())){
        std::cout<<"Table does not exist or is not open"<< std::endl;
        error("Table does not exist or is not open");
        return;
    }
    int bill = restaurant.getTable(tableId)->getBill();
    for(Customer* c : restaurant.getTable(tableId)->getCustomers()){
        delete c;
    }
    restaurant.getTable(tableId)->getCustomers().clear();
    restaurant.getTable(tableId)->closeTable();
    std::cout<<"Table "<<tableId<<" was closed. Bill "<<bill<<"NIS"<< std::endl;
    complete();
}

Close* Close::clone() const {
    Close* toReturn = new Close(tableId);
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

std::string Close::toString() const {
    return "close" + std::to_string(tableId) +" "+ getStringStatus();
}

CloseAll::CloseAll() {};

void CloseAll::act(Restaurant &restaurant) {
    for(unsigned int i = 0 ; i < (unsigned) restaurant.getNumOfTables();i++){
        if(restaurant.getTable(i)->isOpen()){
            Close c = Close(i) ;
            c.act(restaurant);
        }
    }
    restaurant.closeRestaurant();
    complete();
}

std::string CloseAll::toString() const {
    return "closeall " + getStringStatus();
}

CloseAll* CloseAll::clone() const {
    CloseAll* toReturn = new CloseAll();
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for(Dish d : restaurant.getMenu()){
        std::cout<<d.getName()<<" "<<d.getTypeName()<<" "<<d.getPrice()<<"NIS"<<std::endl;
    }
    complete();
}

PrintMenu* PrintMenu::clone() const {
    PrintMenu* toReturn = new PrintMenu();
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

std::string PrintMenu::toString() const {
    return "menu " + getStringStatus();
}
PrintTableStatus::PrintTableStatus(int id):tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table& t = *restaurant.getTable(tableId);
    std::string status;
    if(t.isOpen()){
        status = "open";
    }
    else{
        status = "close";
    }
    std::cout<<"Table "<<tableId<<" status: "<<status<<std::endl;
    if(t.isOpen()){
        std::cout<<"Customers:"<<std::endl;
        for(Customer* c : t.getCustomers()){
            std::cout<<c->getId()<<" "<<c->getName()<<std::endl;
        }
        std::cout<<"Orders:"<<std::endl;
        for(Customer* c : t.getCustomers()){
            for(Dish d : c->getMyOrder()){
                std::cout<<d.getName()<<" "<<d.getPrice()<<"NIS "<<c->getId()<<std::endl;
            }
        }
        std::cout<<"Current Bill: "<<t.getBill()<<std::endl;
    }
    complete();
}

PrintTableStatus* PrintTableStatus::clone() const {
    PrintTableStatus* toReturn = new PrintTableStatus(tableId);
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId) + " " + getStringStatus();
}
void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg ;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    for(BaseAction* b : restaurant.getActionsLog())
        std::cout<< b->toString() << std::endl ;

    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    PrintActionsLog* toReturn = new PrintActionsLog();
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

std::string PrintActionsLog::toString() const {
    return getStringStatus() + getErrorMsg();
}

BackupRestaurant::BackupRestaurant() {
}

BackupRestaurant* BackupRestaurant::clone() const {
    BackupRestaurant* toReturn = new BackupRestaurant();
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup == nullptr){
        backup = new Restaurant(restaurant);
    }
    else {
        delete backup;
    Restaurant *r  = new Restaurant(restaurant);
    backup = r ;
    }
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup " + getStringStatus();
}

RestoreResturant::RestoreResturant() {}

RestoreResturant* RestoreResturant::clone() const {
    RestoreResturant* toReturn = new RestoreResturant();
    if(this->getStatus() == ERROR){
        toReturn->error(this->getErrorMsg());
    }
    else{
        toReturn->setStatus(this->getStatus());
    }
    return toReturn;
}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup == nullptr){
        error("No backup available");
        std::cout<<toString()<<std::endl;
    }
    else{
        restaurant = *backup ;
        complete();
    }
}

std::string RestoreResturant::toString() const {
    return "restore "+ getStringStatus() + " " + getErrorMsg();
}

std::string BaseAction::getStringStatus() const {
    if(this->getStatus() == ERROR){
        return "ERROR";
    }
    if(this->getStatus() == PENDING){
        return "PENDING";
    }
    if(this->getStatus() == COMPLETED){
        return "COMPLETED";
    }
    return "";
}





