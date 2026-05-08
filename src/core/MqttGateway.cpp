#include "core/MqttGateway.hpp"
#include "handlers/TruckCycle.hpp"
#include <memory>

MqttGateway::MqttGateway(AppContainer &container, const AppConfig &config)
    : container_(container), config_(config) {
  this->dispatcher_ = std::make_shared<TopicDispatcher>();

  this->reader_ = std::make_unique<MqttReader>(
      this->config_.system.mqtt_broker, this->config_.system.mqtt_client_id,
      this->dispatcher_, this->container_.getLogger());
}

void MqttGateway::start(std::function<void()> on_truck_cycle) {
  auto logger = this->container_.getLogger();

  auto truck_cycle_handler = std::make_shared<TruckCycle>(
      this->container_.getCheckpointService(), logger);

  truck_cycle_handler->setOnUpdateCallback(on_truck_cycle);

  this->dispatcher_->registerHandler("truck/cycle", truck_cycle_handler);

  if (this->reader_->connect()) {
    this->reader_->subscribe("truck/cycle");
    logger->info("MQTT Connected & Subscribed to truck/cycle");
  } else {
    logger->error("Failed to connect to MQTT broker");
  }
}

void MqttGateway::stop() {
  this->container_.getLogger()->info("Shutting down MQTT Gateway...");
  if (this->reader_) {
    this->reader_->disconnect();
  }
}
