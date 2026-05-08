#pragma once

#include "config/AppConfig.hpp"
#include "db/Database.hpp"
#include "repo/CheckpointRepo.hpp"
#include "services/CheckpointService.hpp"
#include "ui/MessageStore.hpp"
#include "utils/Logger.hpp"
#include <memory>

class AppContainer {
public:
  explicit AppContainer(const AppConfig &config);

  std::shared_ptr<CheckpointService> getCheckpointService() const {
    return this->checkpoint_service_;
  }
  std::shared_ptr<Logger> getLogger() const { return this->logger_; }
  std::shared_ptr<MessageStore> getMessageStore() const { return this->store_; }

private:
  std::shared_ptr<MessageStore> store_;
  std::shared_ptr<Logger> logger_;

  std::unique_ptr<Database> db_;
  std::unique_ptr<CheckpointRepo> checkpoint_repo_;

  std::shared_ptr<CheckpointService> checkpoint_service_;
};
