#pragma once

#include "cpptui.hpp"
#include "fmt/core.h"
#include <chrono>
#include <cstdlib>
#include <memory>

using namespace cpptui;

namespace ui::panels {

enum class TabState { System, Network };

class MetricsPanel : public Border {
public:
  MetricsPanel() : Border(BorderStyle::Rounded) {
    this->set_title(" Live Metrics ", Alignment::Left);

    auto main_layout = std::make_shared<Vertical>();

    // --- Tab Header Controls ---
    auto tab_header = std::make_shared<Horizontal>();
    tab_header->fixed_height = 1;

    auto btn_sys = std::make_shared<Button>(
        " System ", [this]() { this->switchTab(TabState::System); });

    auto btn_net = std::make_shared<Button>(
        " Network ", [this]() { this->switchTab(TabState::Network); });

    tab_header->add(btn_sys);
    tab_header->add(btn_net);

    main_layout->add(tab_header);

    main_layout->add(std::make_shared<VerticalSpacer>(1));

    this->content_container_ = std::make_shared<Vertical>();
    main_layout->add(this->content_container_);

    this->add(main_layout);

    this->buildSystemComponents();
    this->buildNetworkComponents();

    this->switchTab(TabState::System);

    // Start background ticker
    this->start_time_ = std::chrono::steady_clock::now();
  }

  void update() {
    // Calculate uptime (Used in System tab)
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(
                    now - this->start_time_)
                    .count();
    int h = diff / 3600;
    int m = (diff % 3600) / 60;
    int s = diff % 60;
    this->uptime_txt_->set_text(fmt::format("{:02}:{:02}:{:02}", h, m, s));

    // Simulate CPU noise (System Tab)
    float noise = ((rand() % 100) - 50) / 500.0f;
    float current_cpu = this->cpu_pb_->value;
    current_cpu += (0.95f - current_cpu) * 0.2f + noise;
    if (current_cpu < 0)
      current_cpu = 0;
    if (current_cpu > 1)
      current_cpu = 1;

    this->cpu_pb_->value = current_cpu;
    this->cpu_txt_->set_text(fmt::format("{:.0f}%", current_cpu * 100));

    if (current_cpu > 0.8f)
      this->cpu_pb_->color = Theme::current().error;
    else if (current_cpu > 0.5f)
      this->cpu_pb_->color = Theme::current().warning;
    else
      this->cpu_pb_->color = Theme::current().success;

    // Simulate Network Activity (Network Tab)
    float rx_noise = ((rand() % 100) - 50) / 200.0f;
    float current_rx = this->rx_pb_->value;
    current_rx += (0.3f - current_rx) * 0.1f + rx_noise;
    if (current_rx < 0)
      current_rx = 0;
    if (current_rx > 1)
      current_rx = 1;

    this->rx_pb_->value = current_rx;
    this->rx_txt_->set_text(
        fmt::format("{:.1f} MB/s", current_rx * 100)); // Max sim 100MB/s

    float tx_noise = ((rand() % 100) - 50) / 200.0f;
    float current_tx = this->tx_pb_->value;
    current_tx += (0.15f - current_tx) * 0.1f + tx_noise;
    if (current_tx < 0)
      current_tx = 0;
    if (current_tx > 1)
      current_tx = 1;

    this->tx_pb_->value = current_tx;
    this->tx_txt_->set_text(
        fmt::format("{:.1f} MB/s", current_tx * 50)); // Max sim 50MB/s

    // Simulating active MQTT/TCP connections
    int active_conns = 12 + (rand() % 4);
    this->conn_txt_->set_text(fmt::format("{}", active_conns));
  }

private:
  TabState current_tab_ = TabState::System;
  std::shared_ptr<Vertical> content_container_;
  std::shared_ptr<Vertical> sys_panel_;
  std::shared_ptr<Vertical> net_panel_;

  // --- System UI References ---
  std::shared_ptr<Label> cpu_txt_;
  std::shared_ptr<ProgressBar> cpu_pb_;
  std::shared_ptr<Label> mem_txt_;
  std::shared_ptr<ProgressBar> mem_pb_;
  std::shared_ptr<Label> disk_txt_;
  std::shared_ptr<ProgressBar> disk_pb_;
  std::shared_ptr<Label> uptime_txt_;

  // --- Network UI References ---
  std::shared_ptr<Label> rx_txt_;
  std::shared_ptr<ProgressBar> rx_pb_;
  std::shared_ptr<Label> tx_txt_;
  std::shared_ptr<ProgressBar> tx_pb_;
  std::shared_ptr<Label> conn_txt_;

  std::chrono::time_point<std::chrono::steady_clock> start_time_;

  void switchTab(TabState tab) {
    this->current_tab_ = tab;
    this->content_container_->clear_children();

    if (tab == TabState::System) {
      this->content_container_->add(this->sys_panel_);
    } else if (tab == TabState::Network) {
      this->content_container_->add(this->net_panel_);
    }
  }

  void buildSystemComponents() {
    this->sys_panel_ = std::make_shared<Vertical>();
    this->sys_panel_->add(createCpuPanel());
    this->sys_panel_->add(createMemPanel());
    this->sys_panel_->add(createDiskPanel());
    this->sys_panel_->add(createUptimePanel());
  }

  void buildNetworkComponents() {
    this->net_panel_ = std::make_shared<Vertical>();
    this->net_panel_->add(createRxPanel());
    this->net_panel_->add(createTxPanel());
    this->net_panel_->add(createConnectionsPanel());
  }

  std::shared_ptr<Widget> createCpuPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("CPU: "));

    this->cpu_txt_ = std::make_shared<Label>("0%");
    this->cpu_txt_->fixed_width = 8;
    row->add(this->cpu_txt_);

    container->add(row);
    this->cpu_pb_ = std::make_shared<ProgressBar>();
    container->add(this->cpu_pb_);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createMemPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("RAM: "));

    this->mem_txt_ = std::make_shared<Label>("0%");
    this->mem_txt_->fixed_width = 8;
    row->add(this->mem_txt_);

    container->add(row);
    this->mem_pb_ = std::make_shared<ProgressBar>();
    container->add(this->mem_pb_);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createDiskPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("DSK: "));

    this->disk_txt_ = std::make_shared<Label>("0%");
    this->disk_txt_->fixed_width = 8;
    row->add(this->disk_txt_);

    container->add(row);
    this->disk_pb_ = std::make_shared<ProgressBar>();
    container->add(this->disk_pb_);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createUptimePanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("Uptime: "));

    this->uptime_txt_ = std::make_shared<Label>("00:00:00");
    row->add(this->uptime_txt_);

    container->add(row);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createRxPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("RX (In):  "));

    this->rx_txt_ = std::make_shared<Label>("0.0 MB/s");
    this->rx_txt_->fixed_width = 12;
    row->add(this->rx_txt_);

    container->add(row);
    this->rx_pb_ = std::make_shared<ProgressBar>();
    container->add(this->rx_pb_);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createTxPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("TX (Out): "));

    this->tx_txt_ = std::make_shared<Label>("0.0 MB/s");
    this->tx_txt_->fixed_width = 12;
    row->add(this->tx_txt_);

    container->add(row);
    this->tx_pb_ = std::make_shared<ProgressBar>();
    container->add(this->tx_pb_);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }

  std::shared_ptr<Widget> createConnectionsPanel() {
    auto container = std::make_shared<Vertical>();
    auto row = std::make_shared<Horizontal>();
    row->fixed_height = 1;
    row->add(std::make_shared<Label>("Active Connections: "));

    this->conn_txt_ = std::make_shared<Label>("0");
    row->add(this->conn_txt_);

    container->add(row);
    container->add(std::make_shared<VerticalSpacer>(1));
    return container;
  }
};

inline std::shared_ptr<MetricsPanel> createMetricsPanel() {
  return std::make_shared<MetricsPanel>();
}

} // namespace ui::panels
