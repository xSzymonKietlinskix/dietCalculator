#include <iostream>
#include "dataBase.hpp"

using namespace std;

int main(void) {
	const char* location = "dataBase/newDataBase.db";
	dataBase mydata(location);
	mydata.creatTable();
	return 0;
}