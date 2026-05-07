#include "ui/panels/CheckpointTablePanel.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace cpptui;

namespace ui::panels {

CheckpointTablePanel::CheckpointTablePanel()
    : Border(BorderStyle::Rounded, Theme::current().primary) {
  this->set_title(" Checkpoint Table ", Alignment::Center);

  this->table_ = std::make_shared<TablePaginated>();
  this->table_->columns = {"RFID Code", "Checkpoint Code", "Timemillis",
                           "Status"};
  this->table_->auto_page_size = true;
  this->table_->fixed_height = 0;

  this->add(this->table_);
}

void CheckpointTablePanel::update() {
  if (this->onPageRequested) {
    this->onPageRequested(this->current_page_);
  }
}

std::string millis_to_date(long long ms) {
  std::time_t seconds = ms / 1000;
  int remainder_ms = ms % 1000;

  std::tm *tm_ptr = std::localtime(&seconds);

  std::ostringstream oss;
  oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
  oss << '.' << std::setfill('0') << std::setw(3) << remainder_ms;

  return oss.str();
}

void CheckpointTablePanel::setCheckpoints(const std::vector<Checkpoint> &data,
                                          int total_pages) {
  std::vector<std::vector<std::string>> rows;

  rows.reserve(data.size());

  for (const auto &cp : data) {
    rows.push_back({cp.rfid_code, cp.checkpoint_code,
                    millis_to_date(cp.timemillis),
                    cp.is_sync ? "Synced" : "Pending"});
  }

  this->table_->rows = std::move(rows);
  this->table_->current_page = this->current_page_;
}

void CheckpointTablePanel::handleInput(char key) {
  if (key == 'n') {
    this->current_page_++;
    if (this->onPageRequested) {
      onPageRequested(this->current_page_);
    }
  }
}

std::shared_ptr<CheckpointTablePanel> createCheckpointTablePanel() {
  return std::make_shared<CheckpointTablePanel>();
}

} // namespace ui::panels
