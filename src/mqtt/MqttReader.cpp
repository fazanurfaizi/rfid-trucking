#include "mqtt/MqttReader.hpp"
#include "mqtt/TopicDispatcher.hpp"
#include <fmt/core.h>
#include <memory>
#include <mqtt/connect_options.h>

MqttReader::MqttReader(const std::string &server_address,
                       const std::string &client_id,
                       std::shared_ptr<TopicDispatcher> dispatcher,
                       std::shared_ptr<Logger> logger)
    : server_address_(server_address), client_id_(client_id),
      dispatcher_(dispatcher), logger_(logger),
      cli_(server_address, client_id) {
  this->cli_.set_callback(*this);
}

MqttReader::~MqttReader() { this->disconnect(); }

void MqttReader::say() const {
  fmt::print("Hello, {}! Welcome to MQTT Reader.\n", this->server_address_);
}

bool MqttReader::connect() {
  mqtt::connect_options conn_opts;
  conn_opts.set_keep_alive_interval(20);
  conn_opts.set_clean_session(true);

  try {
    this->logger_->info("Connecting to MQTT broker at {}...\n",
                        this->server_address_);
    mqtt::token_ptr conn_token = this->cli_.connect(conn_opts);
    conn_token->wait();
    this->logger_->info("Connected successfully!\n");
    return true;
  } catch (const mqtt::exception &e) {
    this->logger_->error("Connection error: {}\n", e.what());
    return false;
  }
}

bool MqttReader::subscribe(const std::string &topic, int qos) {
  try {
    this->logger_->info("Subscribing to topic '{}'...\n", topic);
    mqtt::token_ptr sub_token = this->cli_.subscribe(topic, qos);
    sub_token->wait();
    this->logger_->info("Subscribed!\n");
    return true;
  } catch (const mqtt::exception &e) {
    this->logger_->error("Subscription error: {}\n", e.what());
    return false;
  }
}

void MqttReader::disconnect() {
  if (this->cli_.is_connected()) {
    try {
      this->logger_->info("Disconnecting from broker...\n");
      this->cli_.disconnect()->wait();
      this->logger_->info("Disconnected.\n");
    } catch (const mqtt::exception &e) {
      this->logger_->warn("Disconnect error: {}\n", e.what());
    }
  }
}

void MqttReader::connection_lost(const std::string &cause) {
  fmt::print("\nConnection lost! Cause: {}\n",
             cause.empty() ? "unknown" : cause);
}

void MqttReader::message_arrived(mqtt::const_message_ptr msg) {
  std::string topic = msg->get_topic();
  std::string payload = msg->get_payload();

  this->dispatcher_.get()->dispatch(topic, payload);
}

void MqttReader::delivery_complete(mqtt::delivery_token_ptr token) {
  fmt::print("\n[Message Completed]");
}
