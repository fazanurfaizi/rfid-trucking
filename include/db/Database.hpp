#pragma once

#include <string>

class Database {
public:
  Database(const std::string &host, const std::string &database);

  void initTable();
  void query();

private:
  const std::string &host_;
  const std::string &database_;
};
