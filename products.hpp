#ifndef products_hpp
#define products_hpp
#include <string>
#endif // !products_hpp

using namespace std;


class product {
public:
	string type;
	string name;
	float caloriess;
	float price;
	int usage;
public:
	product(string t, string n, float c, float p);
};