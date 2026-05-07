#include "config/AppConfig.hpp"

AppConfig loadConfig() {
  AppConfig config;
  config.system = {"rfid_username", "localhost", "RFID/TRUCKING", 5, 1};
  config.checkpoint_1 = {.active = true,
                         .url = "https://geo-api.com/rfid-truckin",
                         .code = "gate_in",
                         .mqtt_rfid = "ROM1/GATEIN",
                         .waiting_hour = 1,
                         .locations = {"ISP", "SBL"}};
  return config;
}
