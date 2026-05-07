#pragma once

#include "db/Database.hpp"
#include "handlers/IMesssageHandler.hpp"
#include "utils/Logger.hpp"
#include <memory>

class TruckCycle : public IMessageHandler {
public:
  TruckCycle(std::shared_ptr<Database> db, std::shared_ptr<Logger> logger);

  void handleMessage(const std::string &topic,
                     const std::string &payload) override;

private:
  std::shared_ptr<Database> db_;
  std::shared_ptr<Logger> logger_;
};
