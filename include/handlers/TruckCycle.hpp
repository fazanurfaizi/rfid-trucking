#pragma once

#include "handlers/IMesssageHandler.hpp"
#include "services/CheckpointService.hpp"
#include "utils/Logger.hpp"
#include <functional>
#include <memory>

class TruckCycle : public IMessageHandler {
public:
  TruckCycle(std::shared_ptr<CheckpointService> service,
             std::shared_ptr<Logger> logger);

  void handleMessage(const std::string &topic,
                     const std::string &payload) override;

  void setOnUpdateCallback(std::function<void()> cb);

private:
  std::shared_ptr<CheckpointService> service_;
  std::shared_ptr<Logger> logger_;
  std::function<void()> on_update_;
};
