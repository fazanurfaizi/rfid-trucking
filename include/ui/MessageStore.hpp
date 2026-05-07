#pragma once

#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

class MessageStore {
public:
  void addMessage(const std::string &msg);
  void setUpdateCallback(std::function<void()> callback);
  std::vector<std::string> getMessages();

private:
  std::mutex mutex_;
  std::deque<std::string> messages_;
  size_t max_lines_ = 100;
  std::function<void()> on_update_;
};
