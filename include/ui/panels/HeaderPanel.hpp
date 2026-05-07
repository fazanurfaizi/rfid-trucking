#pragma once

#include "cpptui.hpp"
#include <memory>

namespace ui::panels {

inline std::shared_ptr<cpptui::Widget> createHeader(const std::string &title) {
  auto header = std::make_shared<cpptui::Label>(
      " RFID Trucking System ", cpptui::Theme::current().primary);
  header->fixed_height = 1;
  return header;
}

} // namespace ui::panels
