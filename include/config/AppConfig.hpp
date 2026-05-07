#pragma once

#include <string>
#include <vector>

struct CheckpointConfig {
  bool active;
  std::string url;
  std::string code;
  std::string mqtt_rfid;
  unsigned int waiting_hour;
  std::vector<std::string> locations;
};

struct SystemConfig {
  std::string api_username;
  std::string mqtt_broker;
  std::string mqtt_client_id;
  int checkpoint_sync_seconds;
  int checkpoint_remove_days;
};

struct AppConfig {
  SystemConfig system;
  CheckpointConfig checkpoint_1;
  CheckpointConfig checkpoint_2;
};

AppConfig loadConfig();
