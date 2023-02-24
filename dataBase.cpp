#include "dataBase.hpp"


dataBase::dataBase(const char* loc) {
	location = loc;
	openBase();
}

void dataBase::changeLocation(const char* loc) {
	location = loc;
}

void dataBase::openBase() {
	int opening_status;
	try {
		opening_status = sqlite3_open(location, &dB);
		if (opening_status != SQLITE_OK) {
			throw opening_status;
		}
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}
	isOpen = true;
}
void dataBase::closeBase() {
	sqlite3_close(dB);
	isOpen = false;
}

void dataBase::creatDefaultTable() {
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS PRODUCTS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TYPE      TEXT NOT NULL, "
		"NAME     TEXT NOT NULL, "
		"CALORIESS      FLOAT  NOT NULL, "
		"PRICE   FLOAT  NOT NULL, "
		"USAGE INT);";

	try
	{
		int exit;
		exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createDefaultTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}
}

void dataBase::addSingleRecord() {
	sqlite3* dB;
	char* messageError;

	string sql("INSERT INTO PRODUCTS (TYPE, NAME, CALORIESS, PRICE) VALUES('Meat', 'Chicken', 239, 1.2);");
		

	int exit = sqlite3_open(location, &dB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records inserted Successfully!" << endl;
}


