#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include <algorithm>
#include "Dish.h"
#include <iostream>

enum CustomerType{
    chp, veg, spc, alc
};

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getType() const ;
    void setType(std::string t);
    virtual Customer* clone() const = 0;
    const std::string & getName() const;
    int getId() const;
    int getNumberOfOrder() const;
    void addCardinality() ;
    int getBill() const;
    void addToBill(int b);
    Customer(const Customer &customer);
    Customer & operator=(const Customer &customer);
    const std::vector<Dish> getMyOrder() const;
    void addToMyOrder(Dish d) ;
    virtual ~Customer();
    void printMyCurrOrder(const std::vector<int> dishes, const std::vector<Dish> &menu);




private:
    std::vector<Dish> myOrder;
    const std::string name;
    const int id;
    int numberOfOrders ;
    int bill;
    std::string type;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	VegetarianCustomer* clone() const ;

private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	CheapCustomer* clone() const ;
private:
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	SpicyCustomer* clone() const ;

private:
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
	AlchoholicCustomer* clone() const ;



private:
};


#endif