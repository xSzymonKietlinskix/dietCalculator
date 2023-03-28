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
		"CALORIES      FLOAT  NOT NULL, "
		"PRICE   FLOAT  NOT NULL, "
		"USAGE INT DEFAULT 0, "
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
	string sql = "INSERT INTO PRODUCTS (TYPE, NAME, CALORIES, PRICE) VALUES(?, ?, ?, ?); ";
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, p.type.c_str(),p.type.length(),NULL);
		sqlite3_bind_text(stmt, 2, p.name.c_str(), p.name.length(), NULL);
		sqlite3_bind_double(stmt, 3, p.calories);
		sqlite3_bind_double(stmt, 4, p.price);

		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
}
static int printBase(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) 
		ImGui::Text("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	ImGui::Text("\n");
	
	return 0;
}

void dataBase::showBase() {
	string sql = "SELECT * from PRODUCTS";
	char* zErrMsg = 0;
	int rc = sqlite3_exec(dB, sql.c_str(), printBase, NULL, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}


static int countRows(void* rows, int argc, char** argv, char** azColName) {
	int* c = (int*)rows;
	*c = atoi(argv[0]);
	return 0;
}

static int getData(void* rows, int argc, char** argv, char** azColName) {
	int* c = (int*)rows;
	*c = atoi(argv[0]);
	return 0;
}

int dataBase::countProf() {
	double prof = 0;

	char* zErrMsg = 0;
	int rows = 0;
	int rc = sqlite3_exec(dB, "select count(*) from PRODUCTS", countRows, &rows, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		printf("count: %d\n", rows);
	}
	string sql;
	for (int i = 0; i < rows; i++) {
		sql = "SELECT PRICE,CALORIES from PRODUCTS WHERE id = ?";
		sqlite3_stmt* stmt;
		
		int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
		if (rc == SQLITE_OK)
		{
			sqlite3_bind_int(stmt, 1, i + 1);

			rc = sqlite3_step(stmt);

			double price= sqlite3_column_double(stmt, 0);
			double calories = sqlite3_column_double(stmt, 1);
			prof = calories/price;

			sqlite3_finalize(stmt);

			sql = "UPDATE PRODUCTS SET PROFITABILITY = ? WHERE id = ?";
			rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

			sqlite3_bind_double(stmt, 1, prof);
			sqlite3_bind_int(stmt, 2, i + 1);

			rc = sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}


	}
	return 0;
}

vector<int> dataBase::countDiet(int _days, float _cal, string _typ) {
	resetUsage();
	float calBf = 0.35 * _cal;
	float calLun = 0.5 * _cal;
	float calDin = 0.15 * _cal;
	float cost = 0;
	double calTotal = 0;
	string sql;
	vector<int> result;

		if (_typ == "vegetarian") {
			//breakfast
			while (calBf > 0) {
				sql = "SELECT id, calories, price, usage from PRODUCTS WHERE NOT type = 'meat' AND CALORIES < ? AND USAGE = 0 ORDER BY PROFITABILITY DESC";
				sqlite3_stmt* stmt;
				int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
				if (rc == SQLITE_OK)
				{
					sqlite3_bind_int(stmt, 1, 0.5 * _cal);
					rc = sqlite3_step(stmt);

					int id = sqlite3_column_int(stmt, 0);
					int usage = sqlite3_column_int(stmt, 3);
					double price = sqlite3_column_double(stmt, 2);
					double calories = sqlite3_column_double(stmt, 1);
					result.push_back(id);
					cost += price;
					calTotal += calories;
					calBf -= calories;
					sqlite3_finalize(stmt);
					sql = "UPDATE PRODUCTS SET USAGE = ? WHERE id = ?";
					rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

					sqlite3_bind_double(stmt, 1, ++usage);
					sqlite3_bind_int(stmt, 2, id);

					rc = sqlite3_step(stmt);
					sqlite3_finalize(stmt);
					cout << "Cena: "<< cost << "kalorie: " << calTotal << endl;
				}
			}
			result.push_back(-99);
			//lunch
			while (calLun > 0) {

				sql = "SELECT id, calories, price, usage from PRODUCTS WHERE NOT type = 'meat' AND CALORIES < ? AND USAGE = 0 ORDER BY PROFITABILITY DESC";
				sqlite3_stmt* stmt;
				int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
				if (rc == SQLITE_OK)
				{
					sqlite3_bind_int(stmt, 1, 0.5 * _cal);
					rc = sqlite3_step(stmt);

					int id = sqlite3_column_int(stmt, 0);
					int usage = sqlite3_column_int(stmt, 3);
					double price = sqlite3_column_double(stmt, 2);
					double calories = sqlite3_column_double(stmt, 1);
					result.push_back(id);
					cost += price;
					calTotal += calories;
					calLun -= calories;
					sqlite3_finalize(stmt);
					sql = "UPDATE PRODUCTS SET USAGE = ? WHERE id = ?";
					rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

					sqlite3_bind_double(stmt, 1, ++usage);
					sqlite3_bind_int(stmt, 2, id);

					rc = sqlite3_step(stmt);
					sqlite3_finalize(stmt);
					cout << "Cena: " << cost << "kalorie: " << calTotal << endl;
				}
			}
			result.push_back(-98);
		}
			else if (_typ == "vegan") {
				;
			}
			else if (_typ == "standard") {
				;
			}
	
	
	return result;
}


void dataBase::resetUsage() {
	string sql = "UPDATE PRODUCTS SET USAGE=0 WHERE 1";
	int exit;
	exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, NULL);
}

product dataBase::getProduct(int id) {
	string sql = "SELECT calories, price, usage, name, type from PRODUCTS WHERE id = ?";
	sqlite3_stmt* stmt;
	product result;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id);
		rc = sqlite3_step(stmt);
		
		result.calories = sqlite3_column_double(stmt, 0);
		result.price = sqlite3_column_double(stmt, 1);
		result.usage = sqlite3_column_int(stmt, 2);
		const unsigned char *name = sqlite3_column_text(stmt, 3);
		string _name(name, name + sizeof(name));
		result.name = _name;
		const unsigned char* type = sqlite3_column_text(stmt, 4);
		string _type(type, type + sizeof(type));
		result.type = _type;
		sqlite3_finalize(stmt);
	}
	return result;
}

