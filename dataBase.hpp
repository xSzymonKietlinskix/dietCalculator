#ifndef _dataBase
#define _dataBase
#include <iostream>
#include <fstream>
#include <vector>
#include "dataBase/sqlite3.h"

#endif // !_fileReader

using namespace std;

class dataBase {
	const char* location;
public:
	dataBase(const char* loc);
	void creatTable();
};