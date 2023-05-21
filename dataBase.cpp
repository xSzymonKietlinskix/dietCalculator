#include "dataBase.hpp"

enum dishes{breakfast, lunch, diner};

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
		"PORTION FLOAT DEFAULT 1, "
		"PROFITABILITY FLOAT);";

	try
	{
		int exit;
		exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in createDefaultTable function." << endl;
			sqlite3_free(messageError);
		}
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

int dataBase::countProf() {
	double prof = 0;

	char* zErrMsg = 0;
	int rows = 0;
	int rc = sqlite3_exec(dB, "select count(*) from PRODUCTS", countRows, &rows, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
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

string chooseCategory(string _type, int i, int dish) {
		switch (i)
		{
		case 0:
			if (dish == breakfast or dish == diner)
				return "fruit";
			if (_type == "vegan")
				return"fat-vegan";
			return "fat";
		case 1:
			if(_type == "standard")
				return "meat";
			if(dish != diner)
				return "leguminous";
			return "vegetable";
		case 2:
			if (dish == breakfast)
				return "nuts";
			return "vegetable";
		case 3:
			if (dish == breakfast)
				return "bread";
			return "dry";
		case 4:
			if (_type == "vegan")
				return "vegan";
			return "diary";
		case 5:
			if (_type != "standard")
				return "vegan";
			return "diary";
		default:
			return "vegetable";
		}
	return "vegetable";
}

double choosePortion(float& _cal, string category) {
	double multiplier = 1;
	if (category == "fat" or category == "fat-vegan")
		multiplier = 0.1;
	if (category == "nuts")
		multiplier = 0.2;

	if (_cal > 2800)
		return 1.7 * multiplier;
	if (_cal > 2400)
		return 1.2 * multiplier;
	if (_cal > 2000)
		return 1.1 * multiplier;

	return 1*multiplier;
}

int dataBase::getDataForCounting(string& sql, float & _cal, vector<int>& result, float& cost, float& calTotal, float& calDish, float partOfCal, string _type, int &i, int dish) {
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
	
	string category = chooseCategory(_type, i, dish);

	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, partOfCal * calDish);
		sqlite3_bind_text(stmt, 2, category.c_str(),category.length(), SQLITE_TRANSIENT);
		rc = sqlite3_step(stmt);

		if (sqlite3_column_type(stmt, 0) == SQLITE_NULL)
			return getDataForCounting(sql, _cal, result, cost, calTotal, calDish, ++partOfCal, _type, i, dish);

		int id = sqlite3_column_int(stmt, 0);
		int usage = sqlite3_column_int(stmt, 3);
		double price = sqlite3_column_double(stmt, 2);
		double calories = sqlite3_column_double(stmt, 1);
		double portion = choosePortion(_cal, category);
		result.push_back(id);

		float calTmp = portion * calories;
		if (calDish - calTmp < -1.0)
			portion = (calDish) / (calTmp);
		calTotal += portion * calories;
		cost += portion * price;
		calDish -= portion * calories;
		sqlite3_finalize(stmt);
		sql = "UPDATE PRODUCTS SET USAGE = ?, PORTION = ? WHERE id = ?";
		rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);

		sqlite3_bind_double(stmt, 1, ++usage);
		sqlite3_bind_double(stmt, 2, portion);
		sqlite3_bind_int(stmt, 3, id);

		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	return 0;
}

vector<int> dataBase::countDiet(int _days = 1, float _cal = 2000, string _typ = "standard") {
	float calBf = 0.35 * _cal;
	float calLun = 0.4 * _cal;
	float calDin = 0.25 * _cal;
	float cost = 0;
	float calTotal = 0;
	string sql;
	vector<int> result;


	//breakfast
	for (int i = 0; calBf > 1; i++) {
		sql = "SELECT id, calories, price, usage from PRODUCTS WHERE CALORIES < ? AND type = ? AND NOT NAME = 'beef' AND NOT NAME = 'pork' AND NOT NAME = 'chicken' AND NOT NAME = 'turkey' ORDER BY USAGE, PROFITABILITY DESC";
		getDataForCounting(sql, _cal, result, cost, calTotal, calBf, 1, _typ, i, breakfast);
	}
	result.push_back(-99);
	//lunch 
	for (int i = 0; calLun > 1; i++) {
		sql = "SELECT id, calories, price, usage from PRODUCTS WHERE CALORIES < ? AND type = ? ORDER BY USAGE,PROFITABILITY DESC";
		getDataForCounting(sql, _cal, result, cost, calTotal, calLun, 1, _typ, i, lunch);
	}
	result.push_back(-98);
	//diner
	for (int i = 0; calDin > 1; i++) {
		sql = "SELECT id, calories, price, usage from PRODUCTS WHERE CALORIES < ? AND type = ? ORDER BY USAGE,PROFITABILITY DESC";
		getDataForCounting(sql, _cal, result, cost, calTotal, calDin, 1, _typ, i, diner);
	}
	totalCalories = calTotal;
	totalCost = cost;
	
	return result;
}


void dataBase::resetUsage() {
	string sql = "UPDATE PRODUCTS SET USAGE=0, PORTION=1 WHERE 1";
	int exit = sqlite3_exec(dB, sql.c_str(), NULL, 0, NULL);
}

product dataBase::getProduct(int id) {
	string sql = "SELECT calories, price, usage, name, type, portion from PRODUCTS WHERE id = ?";
	sqlite3_stmt* stmt;
	product result;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id);
		rc = sqlite3_step(stmt);
		result.portion = sqlite3_column_double(stmt, 5);
		result.calories = sqlite3_column_double(stmt, 0) * result.portion;
		result.price = sqlite3_column_double(stmt, 1) * result.portion;
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

void dataBase::removeRecord(int id) {
	string sql = "DELETE from PRODUCTS WHERE id = ?";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(dB, sql.c_str(), strlen(sql.c_str()), &stmt, NULL);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, id);
		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
}

