#pragma once 
#include <mutex>
#include <atomic>
#include "../lib_server/db.hpp"

void th_wr_sec(const std::atomic_bool *is_running, Db *db);
void th_wr_hour(const std::atomic_bool *is_running, Db *db);
void th_wr_day(const std::atomic_bool *is_running, Db *db);