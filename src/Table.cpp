#include "../include/Table.h"



Table::Table(int t_capacity): capacity(t_capacity) , open(false) {}


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
    return *this;
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
    std::vector<OrderPair> temp;
    for(OrderPair p : orderList){
        if(p.first != id){
            temp.push_back(p);
        }

    }
    orderList.clear();
    for(OrderPair p2 : temp){
        orderList.push_back(p2);
    }


}

int Table::getCapacity() const {
    return this->capacity;
}

void Table::addCustomer(Customer *customer) {

    customersList.push_back(customer);
    /*for (Dish d : customer->getMyOrder()) {
        orderList.push_back(OrderPair(customer->getId(), d));
    }*/
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
        bill += c->getBill();
    }
    return bill;
}

void Table::order(const std::vector <Dish> &menu) {
    for (Customer *c : customersList) {
        std::vector<int> toAdd = c->order(menu);
        for(int i : toAdd){
            for(Dish d : menu){
                if(d.getId() == i){
                   orderList.emplace_back(c->getId(),d);
                }
            }
        }
    }
}



Table::Table(const Table &table) : capacity(table.capacity), open (table.open){
    for(Customer* c : table.customersList){
        customersList.push_back(c->clone());
    }
   /* for(OrderPair o : table.orderList){
        orderList.push_back(o);
    }*/
}



void Table::clear() {
    for(Customer* c : customersList){
        delete c;
    }
}




Table::~Table() {
    clear();
}


//this function has no use in pur design


std::vector<OrderPair>& Table::getOrders() {

    return orderList;
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
    for(OrderPair o : orderList){
        toReturn->orderList.push_back(o);
    }
    return toReturn;
}

void Table::clearOrders() {
        orderList.clear();
    }














