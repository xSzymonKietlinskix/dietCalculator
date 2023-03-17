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
		"USAGE INT, "
		"PROFITABILITY FLOAT);";

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

void dataBase::addSingleRecord(product &p) {
	char* messageError;
	int test = 2;
	string sql = "INSERT INTO PRODUCTS (TYPE, NAME, CALORIESS, PRICE) VALUES(?, ?, ?, ?); ";
	
	sqlite3_stmt* stmt;
	const char* pszTest;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, &pszTest);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, p.type.c_str(),p.type.length(),NULL);
		sqlite3_bind_text(stmt, 2, p.name.c_str(), p.name.length(), NULL);
		sqlite3_bind_double(stmt, 3, p.caloriess);
		sqlite3_bind_double(stmt, 4, p.price);

		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
}
static int _showBase(void* data, int argc, char** argv, char** azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) 
		ImGui::Text("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	ImGui::Text("\n");
	
	return 0;
}

void dataBase::showBase() {
	string sql;
	sql = "SELECT * from PRODUCTS";
	char* zErrMsg;
	const char* data = "Callback function called";
	int rc = sqlite3_exec(dB, sql.c_str(), _showBase, (void*)data, &zErrMsg);
}

void dataBase::countProf() {
	float prof = 0;

}

