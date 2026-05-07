#include "ui/MessageStore.hpp"
#include <mutex>
#include <vector>

void MessageStore::setUpdateCallback(std::function<void()> callback) {
  std::lock_guard<std::mutex> lock(this->mutex_);
  this->on_update_ = callback;
}

void MessageStore::addMessage(const std::string &msg) {
  {
    // Lock only while modifying the data
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->messages_.push_back(msg);
    if (this->messages_.size() > this->max_lines_) {
      this->messages_.pop_front();
    }
  }

  if (this->on_update_) {
    this->on_update_();
  }
}

std::vector<std::string> MessageStore::getMessages() {
  std::lock_guard<std::mutex> lock(this->mutex_);
  return std::vector<std::string>(this->messages_.begin(),
                                  this->messages_.end());
}
