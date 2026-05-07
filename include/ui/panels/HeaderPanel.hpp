#pragma once

#include "cpptui.hpp"
#include <memory>
#include <string>

namespace ui::panels {

std::shared_ptr<cpptui::Widget> createHeader(const std::string &title);

} // namespace ui::panels
