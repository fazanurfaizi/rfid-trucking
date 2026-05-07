#include "ui/panels/LogsPanel.hpp"
#include "ui/LogBox.hpp"
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>

using namespace cpptui;

namespace ui::panels {

LogsPanel::LogsPanel(App *app, std::shared_ptr<MessageStore> store)
    : Border(BorderStyle::Rounded, Theme::current().secondary), store_(store) {
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

  filter_dd->on_change = [this](int idx, std::string val) {
    this->log_level_filter_ = val;
    this->last_log_count_ = 0;
    this->refresh_logs();
  };

  toolbar->add(filter_dd);
  log_layout->add(toolbar);
  log_layout->add(std::make_shared<VerticalSpacer>(1));

  this->log_view_ = std::make_shared<ScrollableVertical>();
  log_layout->add(this->log_view_);

  this->add(log_layout);

  this->refresh_logs();
}

void LogsPanel::update() {
  auto msgs = this->store_->getMessages();
  if (msgs.size() != this->last_log_count_) {
    this->last_log_count_ = msgs.size();
    this->refresh_logs();
  }
}

void LogsPanel::refresh_logs() {
  if (!this->log_view_)
    return;

  this->log_view_->clear_children();

  auto msgs = this->store_->getMessages();

  const size_t MAX_LINE_WIDTH = 80;

  for (const auto &msg : msgs) {
    std::string level = "INFO";
    std::string display_msg = msg;

    size_t start = msg.find('[');
    size_t end = msg.find(']');
    if (start != std::string::npos && end != std::string::npos && end > start) {
      level = msg.substr(start + 1, end - start - 1);
      // Map general system logs to INFO for the filter
      if (level == "System")
        level = "INFO";
      if (level == "Error")
        level = "ERROR";

      size_t skip_index = end + 1;

      while (skip_index < msg.length() && msg[skip_index] == ' ') {
        skip_index++;
      }

      if (skip_index < msg.length()) {
        display_msg = msg.substr(skip_index);
      } else {
        display_msg = "";
      }
    }

    // Apply Dropdown Filter
    if (this->log_level_filter_ != "ALL" && level != this->log_level_filter_) {
      continue;
    }

    // Create the UI Row
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;

    // Determine Colors
    Color c = Theme::current().foreground;
    if (level == "INFO")
      c = Theme::current().primary;
    else if (level == "WARN")
      c = Theme::current().warning;
    else if (level == "ERROR")
      c = Theme::current().error;

    std::istringstream stream(display_msg);
    std::string line;
    bool is_first_line = true;

    while (std::getline(stream, line)) {
      size_t current_pos = 0;

      while (current_pos < line.length() ||
             (current_pos == 0 && line.empty())) {
        std::string chunk = line.substr(current_pos, MAX_LINE_WIDTH);
        current_pos += MAX_LINE_WIDTH;

        auto row = std::make_shared<Horizontal>();

        std::string tag_text =
            is_first_line ? ("[" + level + "]") : "          ";

        // Label for the tag
        auto lbl_lvl = std::make_shared<Label>(tag_text);
        lbl_lvl->fixed_width = 8;
        lbl_lvl->fg_color = c;

        // Label for the actual text
        auto lbl_txt = std::make_shared<Label>(chunk);

        row->add(lbl_lvl);
        row->add(lbl_txt);

        this->log_view_->add(row);
        is_first_line = false;

        if (line.empty())
          break;
      }
    }
  }
}

std::shared_ptr<LogsPanel>
createLogsPanel(App *app, std::shared_ptr<MessageStore> store) {
  return std::make_shared<LogsPanel>(app, store);
}

} // namespace ui::panels
