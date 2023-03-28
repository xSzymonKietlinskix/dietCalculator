#ifndef _dataBase
#define _dataBase
#include <iostream>
#include <fstream>
#include "dataBase/sqlite3.h"
#include "products.hpp"
#include "imGui/imgui.h"
#include <vector>
#endif // !_dataBase

using namespace std;

class dataBase{
	const char* location;
	bool isOpen;
	sqlite3* dB;
public:
	dataBase(const char* loc);
	void creatDefaultTable();
	void addSingleRecord(product &p);
	void changeLocation(const char* loc);
	void openBase();
	void closeBase();
	void showBase();
	int countProf();
	vector<int> countDiet(int _days, float _cal, string _typ);
	void resetUsage();
	product getProduct(int id);
};