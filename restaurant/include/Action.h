#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include<iterator>
#include "Table.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();

    BaseAction(const BaseAction& other);

    ActionStatus getStatus() const;

    virtual void act(Restaurant& restaurant) = 0;

    virtual std::string toString() const = 0;

    virtual BaseAction* copy() = 0;

    virtual ~BaseAction();

protected:
    void complete();

    void copy2me(const BaseAction& other);

    void error(std::string errorMsg);

    std::string getErrorMsg() const;

private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction{
public:
    ~OpenTable();
    void clear();
    OpenTable(int id, std::vector<Customer*>& customersList);
    void act(Restaurant& restaurant);
    std::string toString() const;
    BaseAction* copy();

private:
    const int tableId;
    std::string str;
    std::vector<Customer*> customers;
};


class Order : public BaseAction{
public:
    Order(int id);

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
    const int tableId;
};


class MoveCustomer : public BaseAction{
public:
    MoveCustomer(int src, int dst, int customerId);

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction{
public:
    Close(int id);

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
    const int tableId;
};


class CloseAll : public BaseAction{
public:
    CloseAll();

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
};


class PrintMenu : public BaseAction{
public:
    PrintMenu();

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

    std::string castT(DishType);

private:
};


class PrintTableStatus : public BaseAction{
public:
    PrintTableStatus(int id);

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
    const int tableId;
};


class PrintActionsLog : public BaseAction{
public:
    PrintActionsLog();

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
};


class BackupRestaurant : public BaseAction{
public:
    BackupRestaurant();

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

private:
    std::vector<Table> tables1;
};


class RestoreResturant : public BaseAction{
public:
    RestoreResturant();

    void act(Restaurant& restaurant);

    std::string toString() const;

    BaseAction* copy();

};


#endif
