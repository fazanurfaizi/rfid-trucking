#pragma once

#include "db/Database.hpp"
#include "handlers/IMesssageHandler.hpp"
#include "ui/MessageStore.hpp"
#include <memory>

class TruckCycle : public IMessageHandler {
public:
  TruckCycle(std::shared_ptr<Database> db,
             std::shared_ptr<MessageStore> logStore);

  void handleMessage(const std::string &topic,
                     const std::string &payload) override;

private:
  std::shared_ptr<Database> db_;
  std::shared_ptr<MessageStore> log_store_;
};
