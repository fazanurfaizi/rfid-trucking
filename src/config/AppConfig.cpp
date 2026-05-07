#include "config/AppConfig.hpp"

AppConfig loadConfig() {
  dotenv::init();

  AppConfig config;
  config.system = {.api_username = std::getenv("API_USERNAME"),
                   .api_password = std::getenv("API_PASSWORD"),
                   .mqtt_broker = "localhost",
                   .mqtt_client_id = "RFID/TRUCKING",
                   .checkpoint_sync_seconds = 5,
                   .checkpoint_remove_days = 1};

  config.checkpoint_1 = {.active = true,
                         .url = "https://geo-api.com/rfid-truckin",
                         .code = "gate_in",
                         .mqtt_rfid = "ROM1/GATEIN",
                         .waiting_hour = 1,
                         .locations = {"ISP", "SBL"}};
  return config;
}
