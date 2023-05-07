#ifndef products_hpp
#define products_hpp
#include <string>
#include <iostream>
#endif // !products_hpp

using namespace std;


class product {
public:
	string type;
	string name;
	float calories;
	float price;
	int usage;
	float portion;
public:
	product(string t, string n, float c, float p);
	product() : type("none"), name("none"), calories(0), price(0), usage(0),portion(0) {};
	void showInConsole();
	string getDataAsString();
	const char * getDataAsChar();
};