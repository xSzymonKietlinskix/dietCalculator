#include <iostream>
#include "dataBase.hpp"

using namespace std;

int main(void) {
	const char* location = "dataBase/newDataBase.db";
	dataBase mydata(location);
	mydata.creatTable();
	mydata.addSingleRecord();
	return 0;
}