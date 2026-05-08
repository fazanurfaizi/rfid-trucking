#include "core/AppContainer.hpp"

AppContainer::AppContainer(const AppConfig &config) {
  this->store_ = std::make_shared<MessageStore>();
  this->logger_ = std::make_shared<Logger>(this->store_);

  this->db_ = std::make_unique<Database>("checkpoint.db", this->logger_);

  this->checkpoint_repo_ =
      std::make_unique<CheckpointRepo>(*this->db_, this->logger_);
  this->checkpoint_service_ = std::make_shared<CheckpointService>(
      *this->checkpoint_repo_, this->logger_);
}
