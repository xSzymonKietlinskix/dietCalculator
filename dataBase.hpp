#ifndef _dataBase
#define _dataBase
#include <iostream>
#include <fstream>
#include "dataBase/sqlite3.h"

#endif // !_dataBase

using namespace std;

class dataBase{
	const char* location;
	bool isOpen;
	sqlite3* dB;
public:
	dataBase(const char* loc);
	void creatDefaultTable();
	void addSingleRecord();
	void changeLocation(const char* loc);
	void openBase();
	void closeBase();
};