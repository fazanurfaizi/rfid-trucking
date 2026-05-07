#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include "db/Database.hpp"
#include "handlers/TruckCycle.hpp"
#include "mqtt/MqttReader.hpp"
#include "mqtt/TopicDispatcher.hpp"
#include "ui/LogBox.hpp"
#include "ui/MessageStore.hpp"
#include "ui/panels/CheckpointSettingPanel.hpp"
#include "ui/panels/HeaderPanel.hpp"
#include "ui/panels/MetricsPanel.hpp"
#include "ui/panels/SystemSettingPanel.hpp"

#include <memory>
#include <sys/types.h>

int main() {
  AppConfig config = loadConfig();

  cpptui::App app;

  auto db = std::make_shared<Database>("locahost", "checkpoint.db");
  db->initTable();

  auto dispatcher = std::make_shared<TopicDispatcher>();
  auto store = std::make_shared<MessageStore>();

  store->setUpdateCallback([&app]() {
    cpptui::Event wake_event;
    wake_event.type = cpptui::EventType::Key;
  });

  auto truckCycleHandler = std::make_shared<TruckCycle>(db, store);

  dispatcher->registerHandler("truck/cycle", truckCycleHandler);

  MqttReader reader(config.system.mqtt_broker, config.system.mqtt_client_id,
                    dispatcher);

  if (reader.connect()) {
    reader.subscribe("truck/cycle");
    store->addMessage("[System] MQTT Connected & Subscribed to truck/cycle");
  } else {
    store->addMessage("[Error] Failed to connect to MQTT broker");
  }

  auto root = std::make_shared<cpptui::Vertical>();

  // Top Header
  root->add(ui::panels::createHeader(" RFID Trucking System "));

  auto setting_panel = std::make_shared<cpptui::Horizontal>();
  setting_panel->fixed_height = 13;

  // System Setting Panel
  setting_panel->add(ui::panels::createSystemSettingPanel(config.system));

  // Checkpoint Setting Panel
  setting_panel->add(ui::panels::createCheckpointSettingPanel(config));

  auto metrics_panel = ui::panels::createMetricsPanel();
  setting_panel->add(metrics_panel);

  root->add(setting_panel);

  app.add_timer(1000, [metrics_panel]() { metrics_panel->update(); });

  app.register_exit_key('q');
  app.run(root);

  return 0;
}
