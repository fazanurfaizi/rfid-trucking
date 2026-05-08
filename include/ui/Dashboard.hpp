#pragma once

#include "config/AppConfig.hpp"
#include "core/AppContainer.hpp"
#include "cpptui.hpp"
#include <atomic>
#include <memory>

class Dashboard {
public:
  Dashboard(AppContainer &container, const AppConfig &config);

  void run();
  void triggerCheckpointUpdate();

private:
  cpptui::App app_;
  AppContainer &container_;
  std::shared_ptr<std::atomic<bool>> checkpoint_needs_update_;
  std::shared_ptr<cpptui::Widget> root_layout_;

  void buildLayout(const AppConfig &config);
};
