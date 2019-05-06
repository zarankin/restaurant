#include "../include/Action.h"
#include "../include/Restaurant.h"
#include <string>
#include <iostream>
#include "../include/Customer.h"
#include<iterator>
#include <Action.h>

#include "../include/Table.h"

using namespace std;


BaseAction::BaseAction() : errorMsg(), status(ActionStatus::PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

BaseAction::BaseAction(const BaseAction& other) : errorMsg(other.errorMsg), status(other.status) {

}

void BaseAction::complete() {
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " + errorMsg << endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::copy2me(const BaseAction& other) {
    status = other.status;
    errorMsg = other.errorMsg;
}

BaseAction::~BaseAction() = default;


OpenTable::OpenTable(int id, vector<Customer*>& customersList) : BaseAction(), tableId(id) {
    for(Customer* customer : customersList) {
        Customer* c = customer->copy();
        customers.push_back(c);
    }
}

OpenTable::~OpenTable() {
    clear();
}

void OpenTable::clear() {
    for(Customer* customer : customers) {
        delete customer;
    }
    customers.clear();
}


void OpenTable::act(Restaurant& restaurant) {
    Table* table = restaurant.getTable(tableId);

    for(Customer* customer : customers) {
        this->str = str + customer->getName() + "," + customer->getType() + " ";
    }

    if (table == nullptr || table->isOpen()) {
        error("Table does not exist or is already open");
    } else {
        restaurant.getTable(tableId)->openTable();
        for(Customer* customer : customers) {
            restaurant.getTable(tableId)->addCustomer(customer->copy());
        }
        complete();
    }

    for(Customer* customer : customers) {
        delete customer;
    }
    customers.clear();
}

string OpenTable::toString() const {

    if (getStatus() == COMPLETED) {
        return "open " + to_string(tableId) + " " + this->str + "Completed";
    } else
        return "open " + to_string(tableId) + " " + this->str + "Error: " + getErrorMsg();

}

BaseAction* OpenTable::copy() {
    vector<Customer*> newList;
    for(Customer* customer : customers)
        newList.push_back(customer);

    OpenTable* action = new OpenTable(tableId, newList);
    action->str = str;
    action->copy2me(*this);
    return action;
}

Order::Order(int id) : BaseAction(), tableId(id) {
}

void Order::act(Restaurant& restaurant) {

    if (restaurant.getTable(tableId)->isOpen()) {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    } else {
        error("Table does not exist or is not open");
    }
}

string Order::toString() const {
    if (getStatus() == COMPLETED) {
        return "order " + to_string(tableId) + " Completed";
    } else
        return "order " + to_string(tableId) + " Error: " + getErrorMsg();

}

BaseAction* Order::copy() {
    Order* action = new Order(tableId);
    action->copy2me(*this);
    return action;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : BaseAction(), srcTable(src), dstTable(dst), id(
        customerId) {
}

void MoveCustomer::act(Restaurant& restaurant) {

    if (restaurant.getTable(srcTable) && restaurant.getTable(dstTable) && restaurant.getTable(srcTable)->isOpen() &&
        restaurant.getTable(dstTable)->isOpen() &&
        restaurant.getTable(dstTable)->getCapacity() > restaurant.getTable(dstTable)->getCustomers().size() &&
        restaurant.getTable(srcTable)->getCustomer(id) != nullptr) {

        Customer* c = restaurant.getTable(srcTable)->getCustomer(id);
        restaurant.getTable(dstTable)->addCustomer(c);
        restaurant.getTable(srcTable)->removeCustomer(id);

        complete();

    } else {
        error("Cannot move customer");
    }

}

string MoveCustomer::toString() const {
    string str = "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id);
    if (getStatus() == COMPLETED) {
        return str + " Completed";
    } else
        return str + " Error: Cannot move customer";

}

BaseAction* MoveCustomer::copy() {
    MoveCustomer* action = new MoveCustomer(srcTable, dstTable, id);
    action->copy2me(*this);
    return action;
}

Close::Close(int id) : BaseAction(), tableId(id) {
}

void Close::act(Restaurant& restaurant) {

    if (restaurant.getTable(tableId)->isOpen() && restaurant.getNumOfTables() > tableId) {
        restaurant.getTable(tableId)->closeTable();
        complete();
    } else {
        error("Table does not exist or is not open");
    }
}

string Close::toString() const {
    if (getStatus() == COMPLETED) {
        return "close " + to_string(tableId) + " Completed";
    } else
        return "close " + to_string(tableId) + " Error: " + getErrorMsg();

}

BaseAction* Close::copy() {
    Close* action = new Close(tableId);
    action->copy2me(*this);
    return action;
}

CloseAll::CloseAll() : BaseAction() {
}

void CloseAll::act(Restaurant& restaurant) {//check again

    //for (vector<Table *>::iterator iter = restaurant.getTables().begin(); iter != restaurant.getTables().end(); ++iter) {
    for(unsigned int i = 0; i < restaurant.getTables().size(); i++) {
        restaurant.getTables()[i]->closeTable();
    }

    /* backup->clear();
     delete(backup);
     backup= nullptr;
     restaurant.clear();
     delete(&restaurant);*/
    restaurant.closeRest();
}

string CloseAll::toString() const {
    return "";
}

BaseAction* CloseAll::copy() {
    return new CloseAll(*this);
}

PrintMenu::PrintMenu() : BaseAction() {
}

void PrintMenu::act(Restaurant& restaurant) {
    vector<Dish> menu = restaurant.getMenu();
    for(unsigned int i = 0; i < menu.size(); i++) {
        cout << menu[i].getName() << " " << castT(menu[i].getType()) << " " << menu[i].getPrice() << "NIS" << endl;
    }
    complete();
}

string PrintMenu::castT(DishType s) {
    if (s == VEG) {
        return "VEG";
    }
    else if (s == SPC) {
        return "SPC";
    }
    else if (s == ALC) {
        return "ALC";
    }
    else {
        return "BVG";
    }
}

BaseAction* PrintMenu::copy() {
    return new PrintMenu(*this);
}

string PrintMenu::toString() const {
    return "menu Completed";
}


PrintTableStatus::PrintTableStatus(int id) : BaseAction(), tableId(id) {

}

void PrintTableStatus::act(Restaurant& restaurant) {

    if (restaurant.getTable(tableId)->isOpen()) {
        cout << "Table " << tableId << " status: open" << endl;
        cout << "Customers:" << endl;

        for(vector<Customer*>::iterator iter = restaurant.getTable(tableId)->getCustomers().begin();
            iter != restaurant.getTable(tableId)->getCustomers().end(); ++iter) {
            cout << iter.operator *()->toString() << endl;
        }
        cout << "Orders:" << endl;
        for(vector<OrderPair>::iterator i = restaurant.getTable(tableId)->getOrders().begin();
            i != restaurant.getTable(tableId)->getOrders().end(); ++i) {
            cout << i.operator *().second.getName() << " " << i.operator *().second.getPrice() << "NIS" << " "
                 << i.operator *().first << endl;
        }
        cout << "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << endl;

    } else {
        cout << "Table " << tableId << " status: closed" << endl;
    }
    complete();
}


string PrintTableStatus::toString() const {
    return "status " + to_string(tableId) + " Completed";
}

BaseAction* PrintTableStatus::copy() {
    return new PrintTableStatus(*this);
}

PrintActionsLog::PrintActionsLog() : BaseAction() {
}

void PrintActionsLog::act(Restaurant& restaurant) {

//    for (vector<BaseAction *, std::allocator<BaseAction *>>::const_iterator iter = restaurant.getActionsLog().begin();
//         iter != restaurant.getActionsLog().end(); iter++) {

    for(unsigned int i = 0; i < restaurant.getActionsLog().size(); i++) {

        cout << restaurant.getActionsLog()[i]->toString() << endl;
    }
    complete();

}

string PrintActionsLog::toString() const {
    return "log Completed";
}

BaseAction* PrintActionsLog::copy() {
    return new PrintActionsLog(*this);
}

BackupRestaurant::BackupRestaurant() : BaseAction() {
}

void BackupRestaurant::act(Restaurant& restaurant) {

    if (backup != nullptr)
        delete backup;

    backup = new Restaurant(restaurant);
    complete();
}

BaseAction* BackupRestaurant::copy() {
    return new BackupRestaurant(*this);
}

string BackupRestaurant::toString() const {
    return "backup Completed";
}


RestoreResturant::RestoreResturant() : BaseAction() {
}

void RestoreResturant::act(Restaurant& restaurant) {
    if (backup != nullptr) {
        restaurant.clear();
        restaurant = Restaurant(*backup);
        complete();
    } else {
        error("No backup available");
    }
}

string RestoreResturant::toString() const {
    if (getStatus() == COMPLETED) {
        return "restore Completed";
    } else
        return "restore Error: " + getErrorMsg();

}

BaseAction* RestoreResturant::copy() {
    return new RestoreResturant(*this);
}




