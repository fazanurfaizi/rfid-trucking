#pragma once

#include "cpptui.hpp"
#include <memory>
#include <vector>

using namespace cpptui;

namespace ui::panels {

class CheckpointTablePanel : public Border {
public:
  CheckpointTablePanel()
      : Border(BorderStyle::Single, Theme::current().primary) {
    this->set_title(" Checkpoint Table ", Alignment::Center);

    auto table = std::make_shared<TablePaginated>();
    table->columns = {"RFID Code", "Checkpoint Code", "Timemillis", "Status"};
    table->rows = {
        {"XX123", "gate_in_0", "00000", "True"},
    };
    table->auto_page_size = true;
    table->fixed_height = 0;

    this->add(table);
  }
};

inline std::shared_ptr<CheckpointTablePanel> createCheckpointTablePanel() {
  return std::make_shared<CheckpointTablePanel>();
}

} // namespace ui::panels
