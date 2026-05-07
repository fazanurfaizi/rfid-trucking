#include "ui/panels/SystemSettingPanel.hpp"
#include "fmt/core.h"

using namespace cpptui;

namespace ui::panels {

std::shared_ptr<Widget> createSystemSettingPanel(const SystemConfig &config) {
  auto panel = std::make_shared<Border>(BorderStyle::Rounded);
  panel->set_title(" System Setting ", Alignment::Left);
  panel->fixed_width = 50;

  auto layout = std::make_shared<Vertical>();
  layout->add(std::make_shared<Label>(
      fmt::format("API Username: {}", config.api_username)));
  layout->add(std::make_shared<Label>(
      fmt::format("MQTT Broker: {}", config.mqtt_broker)));
  layout->add(std::make_shared<Label>(
      fmt::format("MQTT Client ID: {}", config.mqtt_client_id)));
  layout->add(std::make_shared<VerticalSpacer>(1));
  layout->add(std::make_shared<Label>(fmt::format(
      "Checkpoint Sync Time: {}s", config.checkpoint_sync_seconds)));
  layout->add(std::make_shared<Label>(fmt::format(
      "Checkpoint Remove Time: {}days", config.checkpoint_remove_days)));

  panel->add(layout);
  return panel;
}

} // namespace ui::panels
