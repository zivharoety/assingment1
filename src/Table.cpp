#include "../include/Table.h"



Table::Table(int t_capacity) {
    capacity = t_capacity;
}


Table& Table::operator=(const Table &other) {
    if(this != &other) {
        this->clear();
        this->customersList = other.customersList;
        this->open = other.open;
    } // should we make new customers ? to change !
    return *this;
}
Table::Table(Table &&other): capacity(other.capacity),open(other.open),customersList(other.customersList) {
    for(unsigned int i = 0 ; i < other.customersList.size() ; i ++){
        other.customersList.at(i) = nullptr ;
    }
}

Table& Table::operator=(Table &&other) {
    clear();
    capacity = other.capacity;
    open = other.open;
    customersList = other.customersList;
    for(unsigned int i = 0 ; i < other.customersList.size() ; i ++){
        other.customersList.at(i) = nullptr ;
    }
}
Customer* Table::getCustomer(int id) {
    for(Customer* c : customersList){
        if(c->getId()==id){
            return c;
        }
    }
    return nullptr;
} // ?!?!?!?!?!?!

void Table::removeCustomer(int id) {
    for(unsigned int i = 0 ; i < customersList.size() ; i++){
        if(customersList.at(i)->getId() == id){
            customersList.erase(customersList.begin()+i);
        }
    }


}

int Table::getCapacity() const {
    return this->capacity;
}

void Table::addCustomer(Customer *customer) {

    customersList.push_back(customer);

}
std::vector<Customer *> & Table::getCustomers() {

    return customersList ;
}

void Table::openTable() {

    this->open = true ;
}

bool Table::isOpen() {

     return open ;
}
void Table::closeTable() {
    this->open = false;
}
int Table::getBill() {
    int bill = 0 ;
    for(Customer* c : customersList){
        bill = bill + c->getBill();
    }
    return bill;
}

void Table::order(const std::vector <Dish> &menu) {
    for (Customer *c : customersList) {
    c->order(menu);
    }
}



Table::Table(const Table &table) : capacity(table.capacity), open (table.open){
    for(Customer* c : table.customersList){
        customersList.push_back(c->clone());
    }
}



void Table::clear() {
    for(Customer* c : customersList){
        delete c;
    }
}




Table::~Table() {
    clear();
}

std::vector<OrderPair>& Table::getOrders() {
    std::vector<OrderPair> toReturn;
    for(Customer* c : customersList){
        const Customer& temp = *c; //is it copy or assignment
        for(Dish d : c->getMyOrder()){
            OrderPair toAdd(c->getId(),d);
            toReturn.push_back(toAdd);
        }

    }
    return toReturn;
}

bool Table::isThere(int id)  {
    return getCustomer(id) != nullptr;

}

Table* Table::clone() {
    Table * toReturn = new Table(capacity);
    toReturn->open = this->open;
    for(Customer * c : customersList){
        toReturn->addCustomer(c->clone());
    }
    return toReturn;
}














