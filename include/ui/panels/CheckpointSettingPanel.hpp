#pragma once

#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include "fmt/core.h"

#include <memory>

using namespace cpptui;

namespace ui::panels {

inline std::shared_ptr<Widget>
createCheckpointSettingPanel(const std::string &title,
                             const CheckpointConfig &config) {
  auto panel = std::make_shared<Border>(BorderStyle::Rounded);
  panel->set_title(fmt::format(" {} ", title), Alignment::Left);
  panel->fixed_width = 50;

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

  panel->add(layout);
  return panel;
}

} // namespace ui::panels
