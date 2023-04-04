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

string product::getDataAsString() {
	string result;
	result = type + "\n" + name + "\n" + to_string(calories) + " kcal" + "\n" + to_string(price) + " zl" + "\n\n";
	return result;
}

const char* product::getDataAsChar() {
	string result;
	result = type + "\n" + name + "\n" + to_string(calories) + " kcal" + "\n" + to_string(price) + " zl" + "\n\n";
	const char* _res = result.c_str();
	return _res;

}