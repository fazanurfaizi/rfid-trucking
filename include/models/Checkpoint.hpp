#pragma once

#include <string>

struct Checkpoint {
  int id = 0;
  std::string checkpoint_code;
  std::string rfid_code;
  long long timemillis = 0;
  bool is_sync = false;
};
