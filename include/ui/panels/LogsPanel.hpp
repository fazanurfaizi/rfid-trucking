#pragma once

#include "cpptui.hpp"
#include "ui/MessageStore.hpp"
#include <memory>

namespace ui::panels {

class LogsPanel : public cpptui::Border {
public:
  LogsPanel(cpptui::App *app, std::shared_ptr<MessageStore> store);
  void update();

private:
  std::shared_ptr<MessageStore> store_;
  std::shared_ptr<cpptui::ScrollableVertical> log_view_;

  std::string log_level_filter_ = "ALL";
  size_t last_log_count_ = 0;

  void refresh_logs();
};

std::shared_ptr<LogsPanel> createLogsPanel(cpptui::App *app,
                                           std::shared_ptr<MessageStore> store);

} // namespace ui::panels
