


#include <iostream>

#include <vector>
#include <Table.h>

#include "../include/Customer.h"
#include "../include/Dish.h"
#include "../include/Table.h"

using namespace std;


Table::Table(int t_capacity) : BillSum(0),tableNum(0),capacity(t_capacity), open(false){}

Table::~Table() {

    clear();
}

void Table::clear() {
    for(Customer* customer : customersList){
      delete customer;
    }
    this->BillSum=0;
    open = false;
    this->customersList.clear();
    this->orderList.clear();
}

void Table::setTableNum(int n){
    this->tableNum=n;
}

Table::Table( Table &&other):BillSum(other.BillSum), tableNum(other.tableNum), capacity(other.capacity), open(other.open), customersList(move(other.customersList)), orderList(move(other.orderList)) {
    other.clear();
}


Table::Table(const Table &other) :BillSum(other.BillSum), tableNum(other.tableNum), capacity(other.capacity), open(other.open), customersList(), orderList(){
    for(Customer* customer : other.customersList)
        this->customersList.push_back(customer->copy());
    orderList=other.orderList;
}

int Table::getTableNum() {
    return this->tableNum;
}

Table &Table::operator=(const Table &t) {
    if (this != &t) {
        clear();
        for(Customer* customer : customersList)
            this->customersList.push_back(customer->copy());

        this->BillSum = t.BillSum;
        this->tableNum = t.tableNum;
        this->open = t.open;
        this->capacity = t.capacity;
        for(OrderPair orderPair : t.orderList)
        this->orderList.push_back(orderPair);
    }
    return *this;
}

Table &Table::operator=(Table &&t) {//move assignment

    if (this != &t) {

        this->getCustomers()=move(t.customersList);
        this->orderList = move(t.getOrders());

        this->BillSum = t.getBill();
        this->tableNum = t.getTableNum();
        this->open = t.isOpen();
        this->capacity = t.getCapacity();

        t.clear();
    }
    return *this;
}


void Table::addCustomer(Customer *customer) {
    this->customersList.push_back(customer);
    for (unsigned int i = 0; i < customer->getOrders().size(); i++) {
        BillSum = BillSum + customer->getOrders()[i].getPrice();
        OrderPair d(customer->getId(), customer->getOrders()[i]);
        this->orderList.push_back(d);
    }
}

void Table::removeCustomer(int id) {

    vector<Customer *>::iterator iter;

    bool stop = false;

    for (iter = customersList.begin(); iter != customersList.end() && !stop; iter++) {


        if (iter.operator*()->getId() == id) {
            fixOrders(*iter);
            customersList.erase(iter);
            stop = true;
        }
    }
    if(this->customersList.empty()){
        //this->closeTable();
        this->open = false;
    }

}

void Table::fixOrders(Customer *&toRemove) {
    vector<OrderPair> ordersss;
    for (vector<OrderPair>::iterator i = orderList.begin(); i != orderList.end(); i++) {
        if (i.operator*().first == toRemove->getId()) {
            BillSum = BillSum - i.operator*().second.getPrice();
        }
        else{
        ordersss.push_back(*i);
        }
    }
    orderList.clear();
    for (vector<OrderPair>::iterator i = ordersss.begin(); i != ordersss.end(); i++){
        orderList.push_back(*i);
    }

}

Customer *Table::getCustomer(int id) {//check if it could happened that customer not found

    vector<Customer *>::iterator iter;

    for (iter = customersList.begin(); iter != customersList.end(); iter++) {
        if (iter.operator*()->getId() == id) {
            return iter.operator*();
        }
    }
    return nullptr;
}

Table* Table::copy(){
    return new Table(*this);
}

vector<Customer *> &Table::getCustomers() {
    return this->customersList;
}

vector<OrderPair> &Table::getOrders() {
 return this->orderList;
}

void Table::order(const vector<Dish> &menu) {

    vector<Customer *>::iterator iter;


    for (iter = customersList.begin(); iter != customersList.end(); iter++) {


        vector<int> v = iter.operator*()->order(menu);
        for (unsigned int i = 0; i < v.size(); i++) {
            int place=v[i];
            cout<<iter.operator*()->getName()<<" ordered "<<menu[place].getName()<<endl;

            BillSum = BillSum + menu[place].getPrice();
            OrderPair p(iter.operator*()->getId(), menu[place]);
            orderList.push_back(p);
        }
    }
}


void Table::openTable() {
    if (!open) {
        this->open = true;
    }
}

void Table::closeTable() {

    if(isOpen())
        cout << "Table " << this->tableNum << " was closed. Bill " << this->BillSum << "NIS" << endl;

    this->open = false;
    clear();
}

int Table::getBill() {
    return BillSum;
}

bool Table::isOpen() {
    return this->open;
}

void Table::addNum(int n) {
    this->tableNum = n;
}


size_t Table::getCapacity() {
    return this->capacity;
}





