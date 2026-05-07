#pragma once

#include "ui/MessageStore.hpp"
#include <fmt/core.h>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

enum class LogLevel { INFO, WARN, ERROR, DEBUG };

class Logger {
public:
  Logger(std::shared_ptr<MessageStore> store);
  ~Logger();

  template <typename... Args>
  void log(LogLevel level, fmt::format_string<Args...> fmt, Args &&...args) {
    std::string msg = fmt::format(fmt, std::forward<Args>(args)...);
    this->write(level, msg);
  }

  // 3. Convenience wrappers so you can just call logger->info("...")
  template <typename... Args>
  void info(fmt::format_string<Args...> fmt, Args &&...args) {
    this->log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void warn(fmt::format_string<Args...> fmt, Args &&...args) {
    this->log(LogLevel::WARN, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void error(fmt::format_string<Args...> fmt, Args &&...args) {
    this->log(LogLevel::ERROR, fmt, std::forward<Args>(args)...);
  }

private:
  std::shared_ptr<MessageStore> store_;
  std::mutex mutex_;
  std::ofstream file_;
  std::string current_date_;

  void write(LogLevel level, const std::string &msg);
  std::string getCurrentDate();
  std::string getCurrentTime();
  std::string levelToString(LogLevel level);
};
