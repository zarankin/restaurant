#include "../include/Restaurant.h"
#include <string>
#include <algorithm>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Restaurant.h>


using namespace std;

Restaurant::Restaurant() {}

Restaurant::~Restaurant() {
    clear();
}

void Restaurant::clear() {
    for (Table* table : tables) {
        delete (table);
    }
    for (BaseAction* action : actionsLog) {
        delete action;
    }

    this->tables.clear();
    this->actionsLog.clear();
    this->menu.clear();
}

Restaurant::Restaurant(const string &configFilePath) :open(false),numOfTables(0),cusID(0){

    cusID = 0;

    fstream my_stream;
    string str;
    string file;
    size_t i = 0;
    int counter = 0;


    my_stream.open(configFilePath);//open txt
    while (getline(my_stream, file) && (file.empty() || file[0] == '#' || file == " ")) {
        file.erase(std::remove(file.begin(), file.end(), '\r'), file.end());
    }

    this->numOfTables = stoi(file);

    while (getline(my_stream, file) && (file.empty() || file[0] == '#' || file == " ")) {
        file.erase(std::remove(file.begin(), file.end(), '\r'), file.end());
    }
    file = file + ',';
    int n=0;
    for (i = 0; file.length() > i; i++) {//adding the tables size
        if (file.at(i) != ',') {
            str = str + file.at(i);
        } else {
            Table *table = new Table(stoi(str));
            table->setTableNum(n);
            n++;
            this->tables.push_back(table);
            str = "";
        }
    }

    file = "";

    while (getline(my_stream, file)) {
        file.erase(std::remove(file.begin(), file.end(), '\r'), file.end());
        while ((file.empty() || file[0] == '#' || file == " ") && getline(my_stream, file)) {
            file.erase(std::remove(file.begin(), file.end(), '\r'), file.end());
        }
        if (!file.empty()) {
            str = "";
            string name = "";
            string type = "";
            int price = 0;
            i = 0;
            while (file.at(i) != ',') {//saving the name of the dish
                name += file.at(i);
                i++;
            }
            str = "";
            i++;

            while (file.at(i) != ',') {//saving the type of the dish
                type += file.at(i);
                i++;
            }
            DishType t = enumCast(type);
            str = "";
            i++;
            while (file.length() > (unsigned)i) {
                str = str + file.at(i);
                i++;
            }
            price = stoi(str);
            Dish dish(counter, name, price, t);
            menu.push_back(dish);

            counter = counter + 1;
        }
        this->open = true;
    }
}

Restaurant::Restaurant(const Restaurant &other) : open(true), numOfTables(other.numOfTables), cusID(other.cusID){//copy constractor
    for(Table* table : other.tables)
        tables.push_back(new Table(*table));
    for(BaseAction* action: other.actionsLog)
        actionsLog.push_back(action->copy());
    for (unsigned int i = 0; i < other.menu.size(); ++i) {
        menu.push_back(other.menu[i]);
    }
}

Restaurant::Restaurant(Restaurant &&r) : open(true), numOfTables(r.numOfTables), tables(move(r.tables)),
                                         actionsLog(move(r.actionsLog)) {}//move constractor

Restaurant &Restaurant::operator=(Restaurant &&r) {//move assignment
    if (this != &r) {
        this->clear();
        this->open = r.open;
        this->numOfTables = r.numOfTables;
        menu=move(r.menu);
        this->actionsLog = move(r.actionsLog);
        this->tables = move(r.tables);
        r.clear();

    }
    return *this;
}

Restaurant &Restaurant::operator=(const Restaurant &r) {//copy assignment

    if (this != &r) {
        this->clear();
        this->open = r.open;

        for(BaseAction* action : r.actionsLog)
            this->actionsLog.push_back(action);
        for(Table* table : r.tables)
            this->tables.push_back(table);
        for (unsigned int i = 0; i < r.menu.size(); ++i) {
            menu.push_back(r.menu[i]);
        }

        this->numOfTables = r.numOfTables;
    }
    return *this;
}

void Restaurant::closeRest() {
    open =false;

}

void Restaurant::start() {//dont forget to set status for "open table"
    cout << "Restaurant is now open!" << endl;
    string line;
    int space = 0;
    size_t i = 0;
    int table_id = -1;
    string name, custumer_type;
    while (this->open) {
        vector<string> words;
        getline(cin, line);
        space = line.find(" ");
        while (space != -1) {
            words.push_back(line.substr(0, space));
            line = line.substr(space + 1);
            space = line.find(" ");
        }
        words.push_back(line);
            if (words[0] == "open") {
                vector<Customer *> customers;
                table_id = atoi(words[1].c_str());

                for (i = 2; i < words.size(); i++) {
                    name = words[i].substr(0, words[i].find(","));
                    custumer_type = words[i].substr(words[i].find(",")+1);
                    Customer* customer= nullptr;
                    if (custumer_type == "veg") {
                        customer=(new VegetarianCustomer(name, cusID));
                    } else if (custumer_type == "chp") {
                        customer=(new CheapCustomer(name, cusID));
                    } else if (custumer_type == "spc") {
                        customer=(new SpicyCustomer(name, cusID));
                    } else if (custumer_type == "alc") {
                        customer=new AlchoholicCustomer(name, cusID);
                    }
                    customers.push_back(customer);
                    cusID++;


                }
                OpenTable *t=new OpenTable(table_id, customers);
                t->act(*this);
                for(Customer* customer1 :customers) {
                        delete customer1;
                }
                customers.clear();
                actionsLog.push_back(t);


            }
            else if (words[0] == "order") {
                int table_id = atoi(words[1].c_str());

                Order *o= new Order(table_id);
                o->act(*this);
                actionsLog.push_back(o);
            }
            else if (words[0] == "status") {
                int table_id = atoi(words[1].c_str());

                PrintTableStatus *t=new PrintTableStatus(table_id);
                t->act(*this);
                actionsLog.push_back(t);
            }
            else if (words[0] == "move") {
                int origin_table_id = atoi(words[1].c_str());
                int dest_table_id = atoi(words[2].c_str());
                int cus_id = atoi(words[3].c_str());
                MoveCustomer *move=new MoveCustomer(origin_table_id, dest_table_id, cus_id);
                move->act(*this);
                actionsLog.push_back(move);

            }
            else if (words[0] == "backup") {
                BackupRestaurant *backupRest=new BackupRestaurant();
                backupRest->act(*this);
                actionsLog.push_back(backupRest);
            }
            else if (words[0] == "restore") {
                RestoreResturant *restoreRes=new RestoreResturant;
                restoreRes->act(*this);
                actionsLog.push_back(restoreRes);
            }
            else if (words[0] == "close") {
                int table_id = atoi(words[1].c_str());
                Close *c=new Close(table_id);
                c->act(*this);
                actionsLog.push_back(c);
            }
            else if(words[0] == "menu"){
                PrintMenu *men =  new PrintMenu();
                men->act(*this);
                actionsLog.push_back(men);
            }
            else if (words[0] == "log") {
                PrintActionsLog *log=new PrintActionsLog();
                log->act(*this);
                actionsLog.push_back(log);

            }

            else if (words[0] == "closeall") {
                CloseAll *end = new CloseAll();
                actionsLog.push_back(end);
                end->act(*this);
            }


            words.clear();
            table_id = 0 ;


    }
}

int Restaurant::getNumOfTables() const {
    return this->numOfTables;
}

vector<Table *> Restaurant::getTables() {
    return this->tables;
}

Table *Restaurant::getTable(int ind) {
    if ((unsigned)ind < tables.size()) {
        return tables[ind];
    } else return nullptr;
}

const vector<BaseAction *> &Restaurant::getActionsLog() const {
    return this->actionsLog;
} // Return a reference to the history of actions

vector<Dish> &Restaurant::getMenu() {

    return this->menu;
}

DishType Restaurant::enumCast(string type) {

    if (type == "VEG") {
        return VEG;
    } else if (type == "ALC") {
        return ALC;
    } else if (type == "SPC") {
        return SPC;
    } else //(type == "BVG") {
        return BVG;


}


