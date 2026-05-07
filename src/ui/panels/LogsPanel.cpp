#include "ui/panels/LogsPanel.hpp"

using namespace cpptui;

namespace ui::panels {

LogsPanel::LogsPanel(App *app)
    : Border(BorderStyle::Single, Theme::current().secondary) {
  this->set_title(" Logs ", Alignment::Center);

  auto log_layout = std::make_shared<Vertical>();

  // Log toolbar
  auto toolbar = std::make_shared<Horizontal>();
  toolbar->fixed_height = 1;
  toolbar->fixed_width = 18;
  toolbar->add(std::make_shared<Label>("Filter: "));

  auto filter_dd = std::make_shared<Dropdown>(app);
  filter_dd->set_options({"ALL", "INFO", "WARN", "ERROR"});
  filter_dd->selected_index = 0;
  filter_dd->fixed_width = 10;

  toolbar->add(filter_dd);

  log_layout->add(toolbar);
  log_layout->add(std::make_shared<VerticalSpacer>(1));

  auto log_sv = std::make_shared<ScrollableVertical>();
  log_layout->add(log_sv);

  this->add(log_layout);
}

std::shared_ptr<LogsPanel> createLogsPanel(App *app) {
  return std::make_shared<LogsPanel>(app);
}

} // namespace ui::panels
