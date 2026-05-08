#include "ui/Dashboard.hpp"
#include "config/AppConfig.hpp"
#include "ui/panels/CheckpointSettingPanel.hpp"
#include "ui/panels/CheckpointTablePanel.hpp"
#include "ui/panels/HeaderPanel.hpp"
#include "ui/panels/LogsPanel.hpp"
#include "ui/panels/MetricsPanel.hpp"
#include "ui/panels/SystemSettingPanel.hpp"

Dashboard::Dashboard(AppContainer &container, const AppConfig &config)
    : container_(container),
      checkpoint_needs_update_(std::make_shared<std::atomic<bool>>(false)) {
  this->buildLayout(config);
}

void Dashboard::run() { this->app_.run(this->root_layout_); }

void Dashboard::triggerCheckpointUpdate() {
  *this->checkpoint_needs_update_ = true;
}

void Dashboard::buildLayout(const AppConfig &config) {
  auto root = std::make_shared<cpptui::Vertical>();

  // Top Header
  root->add(ui::panels::createHeader(" RFID Trucking System "));

  auto setting_panel = std::make_shared<cpptui::Horizontal>();
  setting_panel->fixed_height = 13;

  // System Setting Panel
  auto system_setting_panel =
      ui::panels::createSystemSettingPanel(config.system);
  auto checkpoint_setting_panel =
      ui::panels::createCheckpointSettingPanel(config);
  auto metrics_panel = ui::panels::createMetricsPanel();

  setting_panel->add(system_setting_panel);
  setting_panel->add(checkpoint_setting_panel);
  setting_panel->add(metrics_panel);
  root->add(setting_panel);

  // Horizontal split (left | right)
  auto splitpane_layout = std::make_shared<cpptui::ScrollableVertical>();

  auto split_h = std::make_shared<cpptui::SplitPane>();
  split_h->vertical = false;
  split_h->ratio = 0.5;
  split_h->fixed_height = 30;

  auto checkpoint_table_panel = ui::panels::createCheckpointTablePanel();
  auto checkpoint_service = this->container_.getCheckpointService();

  checkpoint_table_panel->onPageRequested = [checkpoint_service,
                                             checkpoint_table_panel](int page) {
    auto data = checkpoint_service->getAll(page);
    int total = 20;
    checkpoint_table_panel->setCheckpoints(data, total);
  };
  checkpoint_table_panel->onPageRequested(1);

  auto logs_panel = ui::panels::createLogsPanel(
      &this->app_, this->container_.getMessageStore());

  split_h->set_panes(checkpoint_table_panel, logs_panel);
  splitpane_layout->add(split_h);
  splitpane_layout->add(std::make_shared<cpptui::VerticalSpacer>(1));
  root->add(splitpane_layout);

  this->app_.add_timer(1000, [metrics_panel]() { metrics_panel->update(); });
  this->app_.add_timer(500, [logs_panel]() { logs_panel->update(); });

  auto needs_update = this->checkpoint_needs_update_;
  this->app_.add_timer(500, [checkpoint_table_panel, needs_update]() {
    if (*needs_update) {
      *needs_update = false;
      checkpoint_table_panel->update();
    }
  });

  this->app_.register_exit_key('q');

  this->root_layout_ = root;
}
