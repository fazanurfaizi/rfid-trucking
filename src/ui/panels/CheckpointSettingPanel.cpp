#include "ui/panels/CheckpointSettingPanel.hpp"
#include "fmt/core.h"

using namespace cpptui;

namespace ui::panels {

CheckpointTabsPanel::CheckpointTabsPanel(const AppConfig &config)
    : Border(BorderStyle::Rounded) {
  this->set_title(" Checkpoint Settings ", Alignment::Left);
  this->fixed_width = 50;

  auto main_layout = std::make_shared<Vertical>();

  // Tab Header
  auto tab_header = std::make_shared<Horizontal>();
  tab_header->fixed_height = 1;

  auto btn_cp1 = std::make_shared<Button>(
      " Checkpoint 1 ", [this]() { this->switchTab(CheckpointTab::One); });

  auto btn_cp2 = std::make_shared<Button>(
      " Checkpoint 2 ", [this]() { this->switchTab(CheckpointTab::Two); });

  tab_header->add(btn_cp1);
  tab_header->add(btn_cp2);

  main_layout->add(tab_header);
  main_layout->add(std::make_shared<VerticalSpacer>(1));

  this->content_container_ = std::make_shared<Vertical>();
  main_layout->add(this->content_container_);

  this->add(main_layout);

  this->panel_cp_1_ = buildCheckpointContent(config.checkpoint_1);
  this->panel_cp_2_ = buildCheckpointContent(config.checkpoint_2);

  this->switchTab(CheckpointTab::One);
}

void CheckpointTabsPanel::switchTab(CheckpointTab tab) {
  this->content_container_->clear_children();

  if (tab == CheckpointTab::One) {
    this->content_container_->add(this->panel_cp_1_);
  } else if (tab == CheckpointTab::Two) {
    this->content_container_->add(this->panel_cp_2_);
  }
}

std::shared_ptr<Widget>
CheckpointTabsPanel::buildCheckpointContent(const CheckpointConfig &config) {
  auto layout = std::make_shared<Vertical>();
  layout->add(std::make_shared<Label>(
      fmt::format("Status: {}", config.active ? "Active" : "Inactive")));
  layout->add(std::make_shared<Label>(fmt::format("Code: {}", config.code)));
  layout->add(std::make_shared<Label>(fmt::format("URL: {}", config.url)));
  layout->add(
      std::make_shared<Label>(fmt::format("MQTT RFID: {}", config.mqtt_rfid)));
  layout->add(std::make_shared<VerticalSpacer>(1));
  layout->add(std::make_shared<Label>(
      fmt::format("Wait Time: {}h", config.waiting_hour)));

  const std::string &loc_from =
      config.locations.size() > 0 ? config.locations.at(0) : "";

  const std::string &loc_to =
      config.locations.size() > 1 ? config.locations.at(1) : "";

  layout->add(
      std::make_shared<Label>(fmt::format("Location From: {}", loc_from)));

  layout->add(std::make_shared<Label>(fmt::format("Location To: {}", loc_to)));

  return layout;
}

std::shared_ptr<Widget> createCheckpointSettingPanel(const AppConfig &config) {
  return std::make_shared<CheckpointTabsPanel>(config);
}

} // namespace ui::panels
