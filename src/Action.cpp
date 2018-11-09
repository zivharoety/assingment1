//
// Created by guy ofeck on 02/11/2018.
//

#include "../include/Action.h"
#include "../include/Restaurant.h"

extern Restaurant* backup;

BaseAction::BaseAction():status(PENDING),errorMsg("") {
}
ActionStatus BaseAction::getStatus() const {
    return this->status;
}
//std::string BaseAction::toString() const {}
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id),customers(customersList) {

}
void OpenTable::act(Restaurant &restaurant) {
    if (restaurant.getNumOfTables()<tableId ||
        restaurant.getTable(this->tableId)->isOpen() |
        restaurant.getTable(this->tableId)->getCapacity() < this->customers.size()) {
        error("Table is already open or does not exist");
        std::cout<<getErrorMsg()<<std::endl;
        return;
    }
    Table* t = restaurant.getTable(tableId);
    t->openTable();
    for(Customer* c : customers){
        t->addCustomer(c);
    }
    complete();

}
std::string OpenTable::toString() const {
    std::string s = "open "+ std::to_string(tableId);
    for(Customer* c : customers){
        s = s + " " + c->getName() + ","+c->getType();
    }

    return s + " " + std::to_string(getStatus()) +" "+getErrorMsg();
}

Order::Order(int id): tableId(id) {
}
void Order::act(Restaurant &restaurant) {
    if(tableId > restaurant.getNumOfTables() || !restaurant.getTable(tableId)->isOpen()){
        error("Table Does not exist or is not open");
    }
    restaurant.getTable(tableId)->order(restaurant.getMenu());
    complete();
}
std::string Order::toString() const {
    return "order "+std::to_string(tableId) + std::to_string(getStatus());
}
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src),dstTable(dst),id(customerId) {}

std::string MoveCustomer::toString() const {
    return "move " + std::to_string(srcTable) + " " + std::to_string(dstTable)+" "+std::to_string(id);
}

void MoveCustomer::act(Restaurant &restaurant) {
    if(srcTable>restaurant.getNumOfTables() ||
    dstTable>restaurant.getNumOfTables() ||
    !restaurant.getTable(srcTable)->isOpen() |
    !restaurant.getTable(dstTable)->isOpen() |
    restaurant.getTable(dstTable)->getCustomers().size()+1 > restaurant.getTable(dstTable)->getCapacity()|
    !restaurant.getTable(srcTable)->isThere(id))
     {
        std::cout << "Cannot move customer"<<std::endl;
        error("Cannot move customer");

    }
    else {
        Customer* toMove = restaurant.getTable(srcTable)->getCustomer(id);
        restaurant.getTable(dstTable)->addCustomer(toMove);
        restaurant.getTable(srcTable)->removeCustomer(id);
        if(restaurant.getTable(srcTable)->getCustomers().size() == 0){
            restaurant.getTable(srcTable)->closeTable();
         }
    complete();
    }
}

Close::Close(int id): tableId(id){};
void Close::act(Restaurant &restaurant) {
    if(tableId>restaurant.getNumOfTables() ||!restaurant.getTable(tableId)->isOpen()){
        std::cout<<"Table does not exist or is not open"<< std::endl;
        error("Table does not exist or is not open");
        return;
    }
    int bill = restaurant.getTable(tableId)->getBill();
    for(Customer* c : restaurant.getTable(tableId)->getCustomers()){
        delete(c);
    }
    restaurant.getTable(tableId)->closeTable();
    std::cout<<"Table "<<tableId<<" was closed. Bill "<<bill<<"NIS"<< std::endl;
    complete();
}

std::string Close::toString() const {
    return "close" + std::to_string(tableId) +" "+ std::to_string(getStatus());
}

CloseAll::CloseAll() {};

void CloseAll::act(Restaurant &restaurant) {
    for(int i = 0 ; i < restaurant.getNumOfTables();i++){
        if(restaurant.getTable(i)->isOpen()){
            Close c = Close(i) ;
            c.act(restaurant);
        }
    }
    restaurant.closeRestaurant();
    complete();
}

std::string CloseAll::toString() const {
    return "closeall " + std::to_string(getStatus());
}

PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for(Dish d : restaurant.getMenu()){
        std::cout<<d.getName()<<" "<<d.getType()<<" "<<d.getPrice()<<"NIS"<<std::endl;
    }
    complete();
}

std::string PrintMenu::toString() const {
    return "menu " + std::to_string(getStatus());
}
PrintTableStatus::PrintTableStatus(int id):tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table& t = *restaurant.getTable(tableId);
    std::cout<<"Table "<<tableId<<" status: "<<t.isOpen()<<std::endl;
    if(t.isOpen()){
        std::cout<<"Customers:"<<std::endl;
        for(Customer* c : t.getCustomers()){
            std::cout<<c->getId()<<" "<<c->getName()<<std::endl;
        }
        std::cout<<"Orders:"<<std::endl;
        for(Customer* c : t.getCustomers()){
            for(Dish* d : c->getMyOrder()){
                std::cout<<d->getName()<<" "<<d->getPrice()<<"NIS "<<c->getId()<<std::endl;
            }
        }
    }
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId) + " " + std::to_string(getStatus());
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
}

BackupRestaurant::BackupRestaurant() {
}

void BackupRestaurant::act(Restaurant &restaurant) {

}


//Order::BaseAction() {}



