#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include "db/Database.hpp"
#include "handlers/TruckCycle.hpp"
#include "mqtt/MqttReader.hpp"
#include "mqtt/TopicDispatcher.hpp"
#include "repo/CheckpointRepo.hpp"
#include "services/CheckpointService.hpp"
#include "ui/LogBox.hpp"
#include "ui/MessageStore.hpp"
#include "ui/panels/CheckpointSettingPanel.hpp"
#include "ui/panels/CheckpointTablePanel.hpp"
#include "ui/panels/HeaderPanel.hpp"
#include "ui/panels/LogsPanel.hpp"
#include "ui/panels/MetricsPanel.hpp"
#include "ui/panels/SystemSettingPanel.hpp"
#include "utils/Logger.hpp"

#include <atomic>
#include <memory>
#include <sys/types.h>

int main() {
  AppConfig config = loadConfig();

  cpptui::App app;

  auto dispatcher = std::make_shared<TopicDispatcher>();
  auto store = std::make_shared<MessageStore>();
  auto logger = std::make_shared<Logger>(store);
  auto db = std::make_shared<Database>("checkpoint.db", logger);

  store->setUpdateCallback([&app]() {
    cpptui::Event wake_event;
    wake_event.type = cpptui::EventType::Key;
  });

  auto checkpoint_repo = std::make_shared<CheckpointRepo>(db, logger);
  auto checkpoint_service =
      std::make_shared<CheckpointService>(checkpoint_repo, logger);

  auto truck_cycle_handler =
      std::make_shared<TruckCycle>(checkpoint_service, logger);

  auto checkpoint_needs_update = std::make_shared<std::atomic<bool>>(false);

  truck_cycle_handler->setOnUpdateCallback(
      [checkpoint_needs_update]() { *checkpoint_needs_update = true; });

  dispatcher->registerHandler("truck/cycle", truck_cycle_handler);

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
  checkpoint_table_panel->onPageRequested = [checkpoint_service,
                                             checkpoint_table_panel](int page) {
    auto data = checkpoint_service->getAll(page);
    int total = 20;
    checkpoint_table_panel->setCheckpoints(data, total);
  };
  checkpoint_table_panel->onPageRequested(1);

  auto logs_panel = ui::panels::createLogsPanel(&app, store);

  split_h->set_panes(checkpoint_table_panel, logs_panel);
  splitpane_layout->add(split_h);
  splitpane_layout->add(std::make_shared<cpptui::VerticalSpacer>(1));
  root->add(splitpane_layout);

  app.add_timer(1000, [metrics_panel]() { metrics_panel->update(); });
  app.add_timer(500, [logs_panel]() { logs_panel->update(); });
  app.add_timer(500, [checkpoint_table_panel, checkpoint_needs_update]() {
    if (*checkpoint_needs_update) {
      *checkpoint_needs_update = false;
      checkpoint_table_panel->update();
    }
  });

  app.register_exit_key('q');
  app.run(root);

  return 0;
}
