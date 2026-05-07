#include "mqtt/TopicDispatcher.hpp"
#include "fmt/core.h"

void TopicDispatcher::registerHandler(
    const std::string &topic, std::shared_ptr<IMessageHandler> handler) {
  this->handlers_[topic] = handler;
}

void TopicDispatcher::dispatch(const std::string &topic,
                               const std::string &payload) {
  auto it = this->handlers_.find(topic);
  if (it != this->handlers_.end()) {
    it->second->handleMessage(topic, payload);
  } else {
    fmt::print("No handler registered for topic: {}\n", topic);
  }
}
