#include "ui/MessageStore.hpp"
#include <mutex>
#include <vector>

void MessageStore::addMessage(const std::string &msg) {
  {
    // Lock only while modifying the data
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->messages_.push_back(msg);
    if (this->messages_.size() > this->max_lines_) {
      this->messages_.pop_front();
    }
  }
}

std::vector<std::string> MessageStore::getMessages() {
  std::lock_guard<std::mutex> lock(this->mutex_);
  return std::vector<std::string>(this->messages_.begin(),
                                  this->messages_.end());
}
