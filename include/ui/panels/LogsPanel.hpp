#pragma once

#include "cpptui.hpp"
#include <memory>

namespace ui::panels {

class LogsPanel : public cpptui::Border {
public:
  LogsPanel(cpptui::App *app);
};

std::shared_ptr<LogsPanel> createLogsPanel(cpptui::App *app);

} // namespace ui::panels
