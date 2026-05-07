#pragma once

#include "cpptui.hpp"
#include "models/Checkpoint.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace ui::panels {

class CheckpointTablePanel : public cpptui::Border {
public:
  CheckpointTablePanel();

  void update();
  void setCheckpoints(const std::vector<Checkpoint> &data, int total_pages);

  std::function<void(int page)> onPageRequested;

private:
  int current_page_ = 1;
  std::shared_ptr<cpptui::TablePaginated> table_;

  void handleInput(char key);
};

std::shared_ptr<CheckpointTablePanel> createCheckpointTablePanel();

} // namespace ui::panels
