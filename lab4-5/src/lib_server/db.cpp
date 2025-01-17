#include "db.hpp"
#include <sqlite3.h> 
#include <vector> 
#include <iostream> 
#include <utility> 
#include <ctime> 
#include <cstdio> 
#include "sql.hpp"

std::string table_to_string(Table table) {
    if (table == Table::TSec) {
        return std::string("Sec");
    } else if (table == Table::THour) {
        return std::string("Hour");
    } else if (table == Table::TDay) {
        return std::string("Day");
    }
    return std::string("");
}

Db::Db(std::string p) : path(p), db_mut() {
    char *zErrMsg = 0;
    // db_mut.lock();
    int res = sqlite3_open(path.c_str(), &db);
    if (res != SQLITE_OK) std::cerr << "Error open db" << std::endl;

    res = sqlite3_exec(db, create_tables_sql.c_str(), NULL, 0, &zErrMsg);
    // db_mut.unlock();
    if (res != SQLITE_OK) std::cerr << "Error create tables: " << zErrMsg << std::endl;
}

void Db::add_to_db(time_t time, double temp, Table table) {
    char *zErrMsg = 0;
    std::string sql = std::format(
        "INSERT INTO {} (Time, Temp) VALUES ({}, {});", table_to_string(table), time, temp);
    // db_mut.lock();
    int res = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    // db_mut.unlock();
    if (res != SQLITE_OK) std::cerr << "Error insert table: " << zErrMsg << std::endl;
}

int callback_select(void* data, int argc, char **argv, char **azColName) {   
    ((std::vector<std::pair<time_t, double>> *)data)->push_back(
        std::make_pair(std::stoll(argv[0]), std::stod(argv[1])));
    return 0;
}

std::vector<std::pair<time_t, double>>* Db::get_from_db(std::vector<std::pair<time_t, double>> *data, Table table) {
    char *zErrMsg;
    std::string sql = std::format(
        "SELECT Time, Temp from {};", table_to_string(table));

    // db_mut.lock();
    int res = sqlite3_exec(db, sql.c_str(), callback_select, (void*)data, &zErrMsg);
    // db_mut.unlock();
    if (res != SQLITE_OK) std::cerr << "Error select table: " << zErrMsg << std::endl;

    return data;
}

Db::~Db() {
    db_mut.lock();
    sqlite3_close(db);
    db_mut.unlock();
}
