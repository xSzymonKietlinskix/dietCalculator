#include "dataBase.hpp"


dataBase::dataBase(const char* loc) {
	sqlite3* dB;
	location = loc;

	int exit = 0;
	exit = sqlite3_open(loc, &dB);

	sqlite3_close(dB);
}

void dataBase::creatTable() {
	sqlite3* dB;
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS PRODUCTS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TYPE      TEXT NOT NULL, "
		"NAME     TEXT NOT NULL, "
		"CALORIESS      FLOAT  NOT NULL, "
		"PRICE   FLOAT  NOT NULL);";

	try
	{
		int exit = 0;
		exit = sqlite3_open(location, &dB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createTable function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created Successfully" << endl;
		sqlite3_close(dB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}
}


