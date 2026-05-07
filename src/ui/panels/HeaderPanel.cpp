#include "ui/panels/HeaderPanel.hpp"

namespace ui::panels {

std::shared_ptr<cpptui::Widget> createHeader(const std::string &title) {
  auto header =
      std::make_shared<cpptui::Label>(title, cpptui::Theme::current().primary);
  header->fixed_height = 1;
  return header;
}

} // namespace ui::panels
