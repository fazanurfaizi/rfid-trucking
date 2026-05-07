#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include "db/Database.hpp"
#include "handlers/TruckCycle.hpp"
#include "mqtt/MqttReader.hpp"
#include "mqtt/TopicDispatcher.hpp"
#include "ui/LogBox.hpp"
#include "ui/MessageStore.hpp"
#include "ui/panels/CheckpointSettingPanel.hpp"
#include "ui/panels/CheckpointTablePanel.hpp"
#include "ui/panels/HeaderPanel.hpp"
#include "ui/panels/LogsPanel.hpp"
#include "ui/panels/MetricsPanel.hpp"
#include "ui/panels/SystemSettingPanel.hpp"
#include "utils/Logger.hpp"

#include <memory>
#include <sys/types.h>

int main() {
  AppConfig config = loadConfig();

  cpptui::App app;

  auto db = std::make_shared<Database>("locahost", "checkpoint.db");
  db->initTable();

  auto dispatcher = std::make_shared<TopicDispatcher>();
  auto store = std::make_shared<MessageStore>();
  auto logger = std::make_shared<Logger>(store);

  store->setUpdateCallback([&app]() {
    cpptui::Event wake_event;
    wake_event.type = cpptui::EventType::Key;
  });

  auto truckCycleHandler = std::make_shared<TruckCycle>(db, logger);

  dispatcher->registerHandler("truck/cycle", truckCycleHandler);

  MqttReader reader(config.system.mqtt_broker, config.system.mqtt_client_id,
                    dispatcher, logger);

  if (reader.connect()) {
    reader.subscribe("truck/cycle");
    logger->info("MQTT Connected & Subscribed to truck/cycle");
  } else {
    logger->error("Failed to connect to MQTT broker");
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

  // Horizontal split (left | right)
  auto splitpane_layout = std::make_shared<cpptui::ScrollableVertical>();

  auto split_h = std::make_shared<cpptui::SplitPane>();
  split_h->vertical = false;
  split_h->ratio = 0.5;
  split_h->fixed_height = 30;

  auto checkpoint_table_panel = ui::panels::createCheckpointTablePanel();
  auto logs_panel = ui::panels::createLogsPanel(&app, store);

  split_h->set_panes(checkpoint_table_panel, logs_panel);
  splitpane_layout->add(split_h);
  splitpane_layout->add(std::make_shared<cpptui::VerticalSpacer>(1));
  root->add(splitpane_layout);

  app.add_timer(1000, [metrics_panel]() { metrics_panel->update(); });
  app.add_timer(500, [logs_panel]() { logs_panel->update(); });

  app.register_exit_key('q');
  app.run(root);

  return 0;
}
