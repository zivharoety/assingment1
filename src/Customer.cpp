//
// Created by guy ofeck on 02/11/2018.
//

#include "../include/Customer.h"


Customer::Customer(std::string c_name, int c_id, std::string t): name(c_name),id(c_id),numberOfOrders(0),bill(0),type(t) {}

Customer::Customer( std::string  c_name, int c_id):name(c_name),id(c_id),numberOfOrders(0),bill(0) {}
const std::string & Customer::getName() const {
    return this->name;
}

int Customer::getId() const {
    return this->id;
}
int Customer::getNumberOfOrder() const{
    return this->numberOfOrders ;
}

void Customer::addCardinality() {
    this->numberOfOrders = this->numberOfOrders+1;
}

int Customer::getBill() const{
    return this->bill;
}
void Customer::addToBill(int b) {
    bill += b;
}

const std::vector<Dish*> Customer::getMyOrder() const {
    return myOrder;
}

 Customer& Customer::operator=(const Customer &customer) {
    if (this != &customer) {
        this->bill = customer.getBill();
        this->numberOfOrders = customer.getNumberOfOrder();
        myOrder = customer.getMyOrder();
    }
} //not sure it's needed

Customer::~Customer() {} //no needed

Customer::Customer(const Customer &customer):name(customer.getName()),id(customer.getId()),
numberOfOrders(customer.getNumberOfOrder()),bill(customer.getBill()),
myOrder(customer.getMyOrder()) {}



VegetarianCustomer::VegetarianCustomer(std::string name, int id): Customer(name,id,"veg") {

}

void Customer::addToMyOrder(Dish *d) {
    myOrder.push_back(d);
}



std::vector<int> VegetarianCustomer::order(const std::vector <Dish> &menu) {

    std::vector<int> toReturn;
    if(getNumberOfOrder()==0) {
        int min = INT8_MAX;
        int minPrice = 0;
        int exp = -1;
        int expPrice = 0;
        bool wasFound = false; //to implement
        for (Dish d : menu) {
            if (d.getType() == VEG & min > d.getId()) {
                min = d.getId();
                minPrice = d.getPrice();
            }
            if (d.getType() == BVG & exp < d.getPrice()) {
                exp = d.getId();
                expPrice = d.getPrice();

            }
        } // searching in the menu
        toReturn.push_back(min); // to add an if clause ?
        toReturn.push_back(exp); // same same
        addToBill(minPrice + expPrice);
        for (Dish d : menu) {
            if (d.getId() == min || d.getId() == exp) {
                addToMyOrder(&d);

            }
        } // adding the order to the customer log

    }
    else{
        toReturn.push_back(getMyOrder().at(0)->getId());
        addToMyOrder(getMyOrder().at(0));
        toReturn.push_back(getMyOrder().at(1)->getId());
        addToMyOrder(getMyOrder().at(1));

        addToBill(getMyOrder().at(0)->getPrice() + getMyOrder().at(1)->getPrice()) ;


    }
    addCardinality();
    printMyCurrOrder(toReturn , menu);
    return toReturn ;
}

std::string VegetarianCustomer::toString() const {
    return getName() + " " + std::to_string(getId()) ;
}

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id,"spc") {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> toReturn;
    if(getNumberOfOrder() == 0){
        int expID = -1 ;
        int expPrice = 0 ;
        for(Dish d : menu){
            if((d.getType() == SPC & d.getPrice()> expPrice) || (d.getType()== SPC & d.getPrice()==expPrice & d.getId()<expID)){
                expID = d.getId();
                expPrice = d.getPrice() ;
            }
        } // searching for the most expensive spicy dish
        toReturn.push_back(expID); // toReturn
        addToBill(expPrice); // add to bill
        for(Dish d : menu){
            if(d.getId() == expID){
                addToMyOrder(&d); // add to log

            }
        }
        addCardinality();

    }
    else if(getNumberOfOrder() == 1){
        int bvgID = -1;
        int bvgPrice = INT8_MAX ;
        for(Dish d : menu){
            if(((d.getType() == BVG )& (d.getPrice() < bvgPrice)) |((d.getType() ==BVG) & (d.getPrice()==bvgPrice) & (d.getId()<bvgID))){
                bvgPrice = d.getPrice() ;
                bvgID = d.getId() ;
            }
        } // searching for ther cheapest non-alcoholic bvg
        toReturn.push_back(bvgID) ;
        addToBill(bvgPrice) ;
        for(Dish d : menu){
            if(d.getId() == bvgID){
                addToMyOrder(&d); // add to log
            }
        } // end loop
        addCardinality();
    }
    else{
        Dish toAdd  = *getMyOrder().back() ;
        toReturn.push_back(toAdd.getId());
        addToBill(toAdd.getPrice());
        addCardinality();
        addToMyOrder(&toAdd);



    }
    printMyCurrOrder(toReturn,menu);
    return  toReturn;
}

std::string SpicyCustomer::toString() const {
    return getName() + " " + std::to_string(getId()) ;
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer( name,id,"chp") {};

std::vector<int> CheapCustomer::order(const std::vector <Dish> &menu) {
    std::vector<int> toReturn;
    if(getNumberOfOrder()==0) {
        addCardinality();
        int minPrice = INT8_MAX;
        int minID = 0;
        for (Dish d : menu) {
            if ((d.getPrice() < minPrice) | (d.getPrice() == minPrice & d.getId() < minID)) {
                minPrice = d.getPrice();
                minID = d.getId();
            }
        }
        toReturn.push_back(minID);
        addToBill(minPrice);
        for (Dish d : menu) {
            if (d.getId() == minID)
                addToMyOrder(&d);
        }

    }
    printMyCurrOrder(toReturn,menu);
    return toReturn;
    }

std::string CheapCustomer::toString() const {
    return getName() + " " + std::to_string(getId()) ;
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id): Customer(name,id,"alc") {};

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> toReturn;
    int minID = -1;
    int minPrice = INT8_MAX ;
    bool wasFound = false;
    if(getNumberOfOrder()==0) {
        for (Dish d : menu) {
            if ((d.getType() == ALC) & ((d.getPrice() < minPrice) | (d.getPrice() == minPrice & d.getId() < minID))) {
                minID = d.getId();
                minPrice = d.getPrice();
                wasFound = true;
            } // searching for cheapest alc
        } // end loop
    }
    else{
        Dish temp = *getMyOrder().back();

        for(Dish d : menu){
            if(((d.getType() == ALC) & ((d.getPrice()>=temp.getPrice()) &
                    (d.getPrice()<=minPrice)) &
                    (((d.getPrice()==temp.getPrice()) & (d.getId()>temp.getId())) | (d.getPrice()>temp.getPrice())))){
                minID = d.getId();
                minPrice = d.getPrice();
                wasFound = true;
            }
        } // searching for relevant bvg
    }
    if(!wasFound){
        return toReturn;
    }
    toReturn.push_back(minID);
    addToBill(minPrice);
    for(Dish d : menu){
        if(d.getId()==minID){
            addToMyOrder(&d);
        }
    }
    addCardinality();
    printMyCurrOrder(toReturn,menu);
    return toReturn;

}
std::string AlchoholicCustomer::toString() const {
    return getName() + " " + std::to_string(getId()) ;
}

void Customer::printMyCurrOrder(const std::vector<int> dishes, const std::vector<Dish> &menu) {
    for(int id : dishes){
        for(Dish d : menu){
            if(d.getId() == id){
                std::cout<< getName() << " ordered " << d.getName() ;
            }
        }
    }
}
std::string Customer::getType() const {
    return type;
}
