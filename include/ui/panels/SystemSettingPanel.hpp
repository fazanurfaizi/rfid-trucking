#pragma once

#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include <memory>

namespace ui::panels {

std::shared_ptr<cpptui::Widget>
createSystemSettingPanel(const SystemConfig &config);

} // namespace ui::panels
