
#include "../include/Dish.h"
#include <string>


Dish::Dish(const int id,const std::string name,const int price,const DishType d_type) :
id(id),name(name),price(price),type(d_type)
{}

int Dish::getId() const {
	return this->id;
}

std::string Dish::getName() const {
	return this->name;
}

int Dish::getPrice() const {
	return this->price;
}

DishType Dish::getType() const {
	return this->type;
}





