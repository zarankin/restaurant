
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <iostream>
#include <fstream>
extern Restaurant *backup;



class Restaurant{
public:
	Restaurant();
	~Restaurant();
	void clear();
	Restaurant(const std::string &configFilePath);
	Restaurant(const Restaurant &other);
	Restaurant(Restaurant &&r);
	Restaurant& operator=(Restaurant &&r);
	Restaurant& operator=(const Restaurant &r);
	void start();
    int getNumOfTables() const;
	std::vector<Table*> getTables();
	Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
	DishType enumCast(std::string type);
	void closeRest();
private:

    bool open;
	int numOfTables;
	int cusID;
	std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif
