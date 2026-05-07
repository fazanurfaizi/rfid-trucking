#include "db/Database.hpp"
#include "fmt/core.h"
#include <memory>
#include <stdexcept>
#include <string>

Database::Database(const std::string &db_path, std::shared_ptr<Logger> logger)
    : logger_(logger) {
  if (sqlite3_open(db_path.c_str(), &this->db_) != SQLITE_OK) {
    std::string err = sqlite3_errmsg(this->db_);
    sqlite3_close(this->db_);
    this->logger_->error("Failed to open database: {}", err);
    throw std::runtime_error("Failed to open database: " + err);
  }
}

Database::~Database() {
  if (this->db_) {
    sqlite3_close(this->db_);
    this->db_ = nullptr;
  }
}

sqlite3 *Database::getHandle() const { return this->db_; }

void Database::execute(const std::string &query) {
  char *errMsg = nullptr;
  if (sqlite3_exec(this->db_, query.c_str(), nullptr, nullptr, &errMsg) !=
      SQLITE_OK) {
    std::string errStr = errMsg;
    sqlite3_free(errMsg);
    this->logger_->error("Query execution failed: {}", errStr);
    throw std::runtime_error("Query execution failed: " + errStr);
  }
}
