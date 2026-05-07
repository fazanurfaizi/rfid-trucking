#pragma once

#include "cpptui.hpp"
#include <chrono>
#include <memory>

namespace ui::panels {

enum class TabState { System, Network };

class MetricsPanel : public cpptui::Border {
public:
  MetricsPanel();
  void update();

private:
  TabState current_tab_ = TabState::System;
  std::shared_ptr<cpptui::Vertical> content_container_;
  std::shared_ptr<cpptui::Vertical> sys_panel_;
  std::shared_ptr<cpptui::Vertical> net_panel_;

  // --- System UI References ---
  std::shared_ptr<cpptui::Label> cpu_txt_;
  std::shared_ptr<cpptui::ProgressBar> cpu_pb_;
  std::shared_ptr<cpptui::Label> mem_txt_;
  std::shared_ptr<cpptui::ProgressBar> mem_pb_;
  std::shared_ptr<cpptui::Label> disk_txt_;
  std::shared_ptr<cpptui::ProgressBar> disk_pb_;
  std::shared_ptr<cpptui::Label> uptime_txt_;

  // --- Network UI References ---
  std::shared_ptr<cpptui::Label> rx_txt_;
  std::shared_ptr<cpptui::ProgressBar> rx_pb_;
  std::shared_ptr<cpptui::Label> tx_txt_;
  std::shared_ptr<cpptui::ProgressBar> tx_pb_;
  std::shared_ptr<cpptui::Label> conn_txt_;

  std::chrono::time_point<std::chrono::steady_clock> start_time_;

  void switchTab(TabState tab);
  void buildSystemComponents();
  void buildNetworkComponents();

  std::shared_ptr<cpptui::Widget> createCpuPanel();
  std::shared_ptr<cpptui::Widget> createMemPanel();
  std::shared_ptr<cpptui::Widget> createDiskPanel();
  std::shared_ptr<cpptui::Widget> createUptimePanel();
  std::shared_ptr<cpptui::Widget> createRxPanel();
  std::shared_ptr<cpptui::Widget> createTxPanel();
  std::shared_ptr<cpptui::Widget> createConnectionsPanel();
};

std::shared_ptr<MetricsPanel> createMetricsPanel();

} // namespace ui::panels
