#include "utils/Logger.hpp"
#include "fmt/core.h"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>

namespace fs = std::filesystem;

Logger::Logger(std::shared_ptr<MessageStore> store) : store_(store) {
  if (!fs::exists("logs")) {
    fs::create_directory("logs");
  }

  this->current_date_ = this->getCurrentDate();

  std::string filepath = "logs/" + this->current_date_ + ".log";
  this->file_.open(filepath, std::ios::app);
}

Logger::~Logger() {
  if (this->file_.is_open()) {
    this->file_.close();
  }
}

std::string Logger::getCurrentDate() {
  auto now = std::chrono::system_clock::now();
  auto time_t_now = std::chrono::system_clock::to_time_t(now);
  std::tm tm_now;
  localtime_r(&time_t_now, &tm_now);
  std::ostringstream oss;
  oss << std::put_time(&tm_now, "%Y-%m-%d");
  return oss.str();
}

std::string Logger::getCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto time_t_now = std::chrono::system_clock::to_time_t(now);
  std::tm tm_now;
  localtime_r(&time_t_now, &tm_now);
  std::ostringstream oss;
  oss << std::put_time(&tm_now, "%H:%M:%S");
  return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
  switch (level) {
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARN:
    return "WARN";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::DEBUG:
    return "DEBUG";
  default:
    return "UNKNOWN";
  }
}

void Logger::write(LogLevel level, const std::string &msg) {
  std::lock_guard<std::mutex> lock(this->mutex_);

  std::string today = this->getCurrentDate();

  if (today != this->current_date_) {
    if (this->file_.is_open())
      this->file_.close();
    this->current_date_ = today;

    if (!fs::exists("logs")) {
      fs::create_directory("logs");
    }

    std::string filepath = "logs/" + this->current_date_ + ".log";
    this->file_.open(filepath, std::ios::app);
  }

  std::string time_str = this->getCurrentTime();
  std::string level_str = this->levelToString(level);
  std::string formatted_msg =
      fmt::format("[{}] [{}] {}", level_str, time_str, msg);

  // Remove trailing newlines for clean UI/File handling
  if (!formatted_msg.empty() && formatted_msg.back() == '\n') {
    formatted_msg.pop_back();
  }

  // Write file
  if (this->file_.is_open()) {
    this->file_ << formatted_msg << "\n";
    this->file_.flush();
  }

  if (this->store_) {
    this->store_->addMessage(formatted_msg);
  }
}
