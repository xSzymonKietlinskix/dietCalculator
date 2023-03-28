#include "products.hpp"



product::product(string t, string n, float c, float p) {
	type = t;
	name = n;
	calories = c;
	price = p;
}

void product::showInConsole() {
	cout << type << endl << name << endl << calories << " kcal" << endl << price << " zl" << endl <<endl;
}