#pragma once

#include "handlers/IMesssageHandler.hpp"
#include <memory>
#include <string>
#include <unordered_map>

#include <fmt/core.h>

class TopicDispatcher {
public:
  void registerHandler(const std::string &topic,
                       std::shared_ptr<IMessageHandler> handler);
  void dispatch(const std::string &topic, const std::string &payload);

private:
  std::unordered_map<std::string, std::shared_ptr<IMessageHandler>> handlers_;
};
