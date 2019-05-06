#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include <iostream>


class Customer{
public:
	Customer(std::string c_name, int c_id);
	virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
	virtual std::string toString() const = 0;
	std::string getName() const;
	int getId() const;
	void setOrders(Dish dish);
	std::vector<Dish> getOrders();
	std::string getType();
	void setType(std::string s);
	virtual Customer* copy()=0;
	virtual ~Customer();
private:
	const std::string name;
	const int id;
	std::vector <Dish> orders;
	std::string type;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	 Customer* copy();
	 ~VegetarianCustomer() override;

private:
	bool found;
	std::vector<int> ordered;
	bool toOrder;
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	Customer* copy();
virtual ~CheapCustomer();
private:
	bool toOrder;
	std::vector<int> ordered;

};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	Customer* copy();
~SpicyCustomer();
private:
	bool toOrder;
	std::vector<int> ordered;
	int bvgId;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	int count(const std::vector<Dish> &menu);
	void sort(	std::vector<int>& alc ,const std::vector<Dish> &menu);
	Customer* copy();
~AlchoholicCustomer();
private:
	bool toOrder;
	std::vector<int> ordered;
	bool counted;
	int counter;
	std::vector<int> alc;
	int num;
};


#endif