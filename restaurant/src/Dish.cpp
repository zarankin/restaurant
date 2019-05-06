

#include "../include/Dish.h"

using namespace std;

#include <string>

Dish::Dish(int d_id, string d_name, int d_price, DishType d_type):

id (d_id), name(d_name), price(d_price), type(d_type) {}


int Dish::getId() const {
    return this->id;
}

string Dish::getName() const {
    return this->name;
}

int Dish::getPrice() const {
    return this->price;
}

DishType Dish::getType() const {
    return this->type;
}

Dish &Dish::operator=(const Dish& t){

    return *this;
}