#include "db/Database.hpp"
#include "fmt/core.h"

Database::Database(const std::string &host, const std::string &database)
    : host_(host), database_(database) {}

void Database::initTable() { fmt::print("Init Table\n"); }

void Database::query() { fmt::print("Query\n"); }
