#pragma once

#include <string>

class IMessageHandler {
public:
  virtual ~IMessageHandler() = default;

  virtual void handleMessage(const std::string &topic,
                             const std::string &payload) = 0;
};
