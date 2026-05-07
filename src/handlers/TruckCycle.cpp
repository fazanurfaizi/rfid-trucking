#include "handlers/TruckCycle.hpp"
#include "services/CheckpointService.hpp"
#include <chrono>
#include <fmt/core.h>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TruckCycle::TruckCycle(std::shared_ptr<CheckpointService> service,
                       std::shared_ptr<Logger> logger)
    : service_(service), logger_(logger) {}

void TruckCycle::handleMessage(const std::string &topic,
                               const std::string &payload) {
  try {
    json j = json::parse(payload);
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto timemillis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    if (j.contains("data") && j["data"].is_array()) {
      for (const auto &item : j["data"]) {
        if (item.contains("epcHex") && item["epcHex"].is_string()) {
          std::string epc = item["epcHex"];

          this->service_->processRfidScan(topic, epc, timemillis);

          this->logger_->info("Saved EPC {} to Database", epc);

          if (this->on_update_) {
            this->on_update_();
          }
        } else {
          this->logger_->warn("No EPC in desired antenna found.");
        }
      }
    } else {
      this->logger_->error(
          "Unexpected data type: 'data' is missing or not an array");
    }

    // this->logger_->info("Processing Truck Entry: RFID={}, Weight={}\n", rfid,
    //                     weight);

    // this->service_->processRfidScan(const std::string &checkpoint_code, const
    // std::string &rfid_code, long long current_millis)
  } catch (const json::parse_error &e) {
    this->logger_->error("JSON Parse Error on topic {}: {}\n", topic, e.what());
  } catch (const std::exception &e) {
    this->logger_->error("Database Error: {}\n", e.what());
  }
}

void TruckCycle::setOnUpdateCallback(std::function<void()> cb) {
  this->on_update_ = cb;
}
