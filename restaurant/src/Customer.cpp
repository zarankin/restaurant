

#include "../include/Customer.h"
#include <vector>
#include <string>
#include <Customer.h>

#include "../include/Dish.h"

using namespace std;


Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id), orders() {
}


string Customer::getName() const {
    return this->name;
}

void Customer::setOrders(Dish dish) {

    this->orders.push_back(dish);

}

string Customer::getType(){
    return this->type;
}

void Customer::setType(string s){
    this->type=s;
}

vector<Dish> Customer::getOrders() {
    return this->orders;
}


int Customer::getId() const {
    return this->id;
}

Customer::~Customer() = default;


VegetarianCustomer::VegetarianCustomer(string name, int id) : Customer(name, id),found(false), ordered(), toOrder(true) {
    setType("veg");
}


vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {

    bool found = false;
    int price = -1;
    int dishID = -1;
    int drinkID = -1;
    if (this->ordered.empty() && toOrder) {

        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == VEG && !found) {
                dishID = menu[i].getId();
                found = true;
            } else if (menu[i].getType() == BVG) {
                if (price < 0) {
                    price = menu[i].getPrice();
                    drinkID=menu[i].getId();
                } else if (price < menu[i].getPrice()) {
                    price = menu[i].getPrice();
                    drinkID = menu[i].getId();
                }
            }

        }

        if (found) {
            ordered.push_back(dishID);
            ordered.push_back(drinkID);
            setOrders(menu[dishID]);
            setOrders(menu[drinkID]);

        } else {
            this->toOrder = false;
        }
    } else {

        if (!ordered.empty()) {
            for (unsigned int i = 0; i < ordered.size(); ++i) {
                setOrders(menu[ordered[i]]);
            }
        }


    }
    return this->ordered;
}

string VegetarianCustomer::toString() const {

    string s = to_string(this->getId()) + " " + this->getName();
    return s;
}

Customer* VegetarianCustomer::copy(){
    VegetarianCustomer* customer=new VegetarianCustomer(getName(),getId());
    customer->ordered=ordered;
    customer->toOrder=toOrder;
    customer->found=found;
    for(Dish dish : getOrders())
        customer->setOrders(dish);
    return customer;
}

VegetarianCustomer::~VegetarianCustomer() {
    ordered.clear();
}


CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id), toOrder(true), ordered() {
    setType("chp");

}

vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    if (toOrder) {
        long price = (long)INT32_MAX +1;
        int dishID = -1;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (price > menu[i].getPrice()) {
                price = menu[i].getPrice();
                dishID=menu[i].getId();
            }
        }
        if(dishID!=-1) {
            ordered.push_back(dishID);
            setOrders(menu[ordered[0]]);
        }
        toOrder = false;

    } else {
        ordered.clear();
    }
    return ordered;
}

string CheapCustomer::toString() const {
    string s = to_string(this->getId()) + " " + this->getName();
    return s;
}

Customer* CheapCustomer::copy(){
    CheapCustomer* customer = new CheapCustomer(getName(),getId());
    customer->toOrder=toOrder;
    customer->ordered=ordered;
    for(Dish dish : getOrders())
        customer->setOrders(dish);
    return customer;
}

CheapCustomer::~CheapCustomer() {
    ordered.clear();
}


SpicyCustomer::SpicyCustomer(string name, int id) : Customer(name, id), toOrder(true), ordered(), bvgId(-1) {
    setType("spc");
}

vector<int> SpicyCustomer::order(const vector<Dish> &menu) {

    int dish = -1;
    long price=-1;
    int bvg = -1;
    int bvgP = -1;

    if (toOrder) {
        if (ordered.empty()) {
            for(const auto& item : menu) {
                if (item.getType() == SPC && price<item.getPrice()) {
                    dish = item.getId();
                    price=item.getPrice();
                } else if (item.getType() == BVG) {
                    if (bvgP < 0) {
                        bvgP = item.getPrice();
                        bvg = item.getId();
                    } else if (bvgP > item.getPrice()) {
                        bvgP = item.getPrice();
                        bvg = item.getId();
                    }
                }
            }

            if (dish > 0) {
                ordered.push_back(dish);
                bvgId = bvg;
                setOrders(menu[dish]);
                return ordered;
            } else {
                toOrder = false;
                return ordered;
            }
        } else if (menu[ordered.back()].getType() != BVG) {
            ordered.clear();
            ordered.push_back(bvgId);
        }
    }
    if (!ordered.empty()) {
        setOrders(menu[ordered[0]]);
    }
    return ordered;
}

string SpicyCustomer::toString() const {
    string s = to_string(this->getId()) + " " + this->getName();
    return s;
}

Customer* SpicyCustomer::copy(){
    SpicyCustomer* customer = new SpicyCustomer(getName(),getId());
    customer->toOrder=toOrder;
    customer->ordered=ordered;
    customer->bvgId=bvgId;
    for(Dish dish : getOrders())
        customer->setOrders(dish);
    return customer;
}

SpicyCustomer::~SpicyCustomer() {
    ordered.clear();
}

AlchoholicCustomer::AlchoholicCustomer(string name, int id) : Customer(name, id), toOrder(false), ordered(),
                                                              counted(false),
                                                              counter(0), alc(), num(0) {
    setType("alc");


}//need to check if i need to delete alc in the end

vector<int> AlchoholicCustomer::order(const vector<Dish> &menu) {

    if (!counted) {//if this is the first order
        num = count(menu);//counting the alc in the menu
        counted = true;

        if (num > 0) {//if there is alc in the menu

            toOrder = true;

            sort(this->alc, menu);//sorting the alc by price
            ordered.clear();
            ordered.push_back(alc[counter]);
            counter++;
        }
    } else if (toOrder) {
        ordered.clear();
        if (counter < num) {
            ordered.push_back(alc[counter]);
            counter++;
        }
        else{
            ordered.clear();
            toOrder=false;
        }
    }
    if (!ordered.empty()) {
        setOrders(menu[ordered[0]]);
    }
    return ordered;

}


void AlchoholicCustomer::sort(vector<int>& alc, const vector<Dish> &menu) {

    if (alc.size() < 2) {
       return;
    } else {
        int price2 = 0;
        int price1 = 0;
        int temp = 0;
        int k = 0;

        for (unsigned int i = 0; i < alc.size() - 1; i++) {
            k = i + 1;
            price1 = menu[alc[k]].getPrice();
            while (k > 0) {
                price2 = menu[alc[k - 1]].getPrice();
                if (price1 < price2) {
                    temp = alc[k - 1];
                    alc[k - 1] = alc[k];
                    alc[k] = temp;
                }
                k--;
            }
        }
    }
}

int AlchoholicCustomer::count(const vector<Dish> &menu) {

    int count = 0;
    for (unsigned int i = 0; i < menu.size(); i++) {
        if (menu[i].getType() == ALC) {
            alc.push_back(menu[i].getId());
            count = count + 1;
        }
    }
    counted = true;
    return count;
}

string AlchoholicCustomer::toString() const {
    string s = to_string(this->getId()) + " " + this->getName();
    return s;
}

Customer* AlchoholicCustomer::copy(){
    AlchoholicCustomer* customer = new AlchoholicCustomer(getName(),getId());
    customer->ordered=ordered;
    customer->alc=alc;
    customer->toOrder=toOrder;
    customer->counted=counted;
    customer->counter=counter;
    customer->num=num;
    for(Dish dish : getOrders())
        customer->setOrders(dish);
    return customer;
}

AlchoholicCustomer::~AlchoholicCustomer() {
ordered.clear();
alc.clear();
}
