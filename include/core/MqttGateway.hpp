#pragma once

#include "config/AppConfig.hpp"
#include "core/AppContainer.hpp"
#include "mqtt/MqttReader.hpp"
#include "mqtt/TopicDispatcher.hpp"

class MqttGateway {
public:
  MqttGateway(AppContainer &container, const AppConfig &config);
  void start(std::function<void()> on_truck_cycle);
  void stop();

private:
  std::shared_ptr<TopicDispatcher> dispatcher_;
  std::unique_ptr<MqttReader> reader_;
  AppContainer &container_;
  const AppConfig &config_;
};
