#include "handlers/TruckCycle.hpp"
#include <fmt/core.h>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TruckCycle::TruckCycle(std::shared_ptr<Database> db,
                       std::shared_ptr<Logger> logger)
    : db_(db), logger_(logger) {}

void TruckCycle::handleMessage(const std::string &topic,
                               const std::string &payload) {
  try {
    json j = json::parse(payload);

    std::string rfid = j["epc"];
    double weight = j["weight"];

    this->logger_->info("Processing Truck Entry: RFID={}, Weight={}\n", rfid,
                        weight);
    // this->log_store_->addMessage(msg);

    // std::string logMsg = "[TruckCycle] Received data: " + payload;
    // this->log_store_->addMessage(logMsg);

    // this->db_->query();
  } catch (const json::parse_error &e) {
    this->logger_->error("JSON Parse Error on topic {}: {}\n", topic, e.what());
  } catch (const std::exception &e) {
    this->logger_->error("Database Error: {}\n", e.what());
  }
}
