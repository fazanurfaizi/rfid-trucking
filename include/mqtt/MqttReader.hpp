
#pragma once

#include "TopicDispatcher.hpp"
#include "utils/Logger.hpp"
#include <memory>
#include <mqtt/async_client.h>
#include <string>

class MqttReader : public virtual mqtt::callback {
public:
  MqttReader(const std::string &server_address, const std::string &client_id,
             std::shared_ptr<TopicDispatcher> dispatcher,
             std::shared_ptr<Logger> logger);

  ~MqttReader();

  void say() const;
  bool connect();
  bool subscribe(const std::string &topic, int qos = 1);
  void disconnect();

private:
  std::string server_address_;
  std::string client_id_;

  // Paho async client
  mqtt::async_client cli_;

  std::shared_ptr<TopicDispatcher> dispatcher_;
  std::shared_ptr<Logger> logger_;

  // mqtt::callback
  void connection_lost(const std::string &cause) override;
  void message_arrived(mqtt::const_message_ptr msg) override;
  void delivery_complete(mqtt::delivery_token_ptr token) override;
};
