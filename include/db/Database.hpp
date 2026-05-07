#pragma once

#include "utils/Logger.hpp"
#include <memory>
#include <sqlite3.h>
#include <string>

class Database {
public:
  Database(const std::string &db_path, std::shared_ptr<Logger> logger);
  ~Database();

  // Disable copy
  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  sqlite3 *getHandle() const;

  void execute(const std::string &query);

private:
  sqlite3 *db_ = nullptr;
  std::shared_ptr<Logger> logger_;
};
