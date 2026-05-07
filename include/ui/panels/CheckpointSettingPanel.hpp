#pragma once

#include "config/AppConfig.hpp"
#include "cpptui.hpp"
#include <memory>

namespace ui::panels {

enum class CheckpointTab { One, Two };

class CheckpointTabsPanel : public cpptui::Border {
public:
  CheckpointTabsPanel(const AppConfig &config);

private:
  std::shared_ptr<cpptui::Vertical> content_container_;
  std::shared_ptr<cpptui::Widget> panel_cp_1_;
  std::shared_ptr<cpptui::Widget> panel_cp_2_;

  void switchTab(CheckpointTab tab);
  std::shared_ptr<cpptui::Widget>
  buildCheckpointContent(const CheckpointConfig &config);
};

std::shared_ptr<cpptui::Widget>
createCheckpointSettingPanel(const AppConfig &config);

} // namespace ui::panels
