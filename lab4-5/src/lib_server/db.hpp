#pragma once 
#include <sqlite3.h> 
#include <string> 
#include <vector> 
#include <mutex> 

enum Table {
    TSec,
    THour,
    TDay,
};

std::string table_to_string(Table table);
int callback_select(int argc, char **argv, char **azColName);

class Db {
    sqlite3 *db;
    std::string path;
    std::mutex db_mut;

 public:
    Db(std::string p);

    void add_to_db(time_t time, double temp, Table table);

    std::vector<std::pair<time_t, double>>* get_from_db(std::vector<std::pair<time_t, double>> *data, Table table);

    ~Db();
};