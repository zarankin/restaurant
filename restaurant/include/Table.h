#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"


typedef std::pair<int, Dish> OrderPair;

class Table {
public:
    Table(int t_capacity);

    Table( Table &&other);

   Table(const Table &other);

    ~Table();

    void clear();

    void setTableNum(int n);

    size_t getCapacity();

    void addCustomer(Customer *customer);

    void removeCustomer(int id);

    Customer *getCustomer(int id);

    std::vector<Customer *> &getCustomers();

    std::vector<OrderPair> &getOrders();

    void order(const std::vector<Dish> &menu);

    void openTable();

    void closeTable();

    int getBill() ;

    bool isOpen();

    void addNum(int n);

    int getTableNum() ;

    Table* copy();

    Table& operator=(const Table &t);
    Table& operator=( Table &&t);


private:
    void fixOrders( Customer* & toRemove);
    int BillSum;
    int tableNum;
    int capacity;
    bool open;
    std::vector<Customer *> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif
