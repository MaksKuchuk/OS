#pragma once
#include <atomic>
#include <mutex>
#include "db.hpp"

void th_server(const std::atomic_bool *is_running, Db *db);