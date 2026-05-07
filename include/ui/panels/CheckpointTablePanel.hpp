#pragma once

#include "cpptui.hpp"
#include <memory>

namespace ui::panels {

class CheckpointTablePanel : public cpptui::Border {
public:
  CheckpointTablePanel();
};

std::shared_ptr<CheckpointTablePanel> createCheckpointTablePanel();

} // namespace ui::panels
