#pragma once 
#include <string>

std::string create_tables_sql = "CREATE TABLE IF NOT EXISTS Sec(" \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "Time           TEXT    NOT NULL," \
                                "Temp           REAL    NOT NULL);" \
                                "CREATE TABLE IF NOT EXISTS Hour(" \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "Time           TEXT    NOT NULL," \
                                "Temp           REAL    NOT NULL);"
                                "CREATE TABLE IF NOT EXISTS Day(" \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "Time           TEXT    NOT NULL," \
                                "Temp           REAL    NOT NULL);";
