#include "cpptui.hpp"
#include "fmt/core.h"
#include <cstdlib>
#include <memory>

using namespace cpptui;

namespace ui::panels {

inline std::shared_ptr<Widget> createMetricsPanel() {
  auto panel = std::make_shared<Border>(BorderStyle::Rounded);
  panel->set_title(" Live Metrics ", Alignment::Left);

  auto layout = std::make_shared<Vertical>();

  // @NOTE: Simulate metrics
  float noise = ((rand() % 100) - 50) / 500.0f;
  float current_cpu = 0.0f;

  current_cpu += (0.95f - current_cpu) * 0.2f + noise;
  if (current_cpu < 0)
    current_cpu = 0;
  if (current_cpu > 1)
    current_cpu = 1;

  // CPU
  auto cpu_panel = std::make_shared<Horizontal>();
  cpu_panel->fixed_height = 1;
  cpu_panel->add(std::make_shared<Label>("CPU: "));

  auto cpu_txt = std::make_shared<Label>("0%");
  cpu_txt->fixed_width = 5;
  // @TODO: link to system monitor
  // @NOTE: Simulate
  cpu_txt->set_text(fmt::format("{}%", current_cpu));
  cpu_panel->add(cpu_txt);
  layout->add(cpu_panel);

  auto pb_cpu = std::make_shared<ProgressBar>();
  // @TODO: lint to system monitor
  // @NOTE: Simulate
  pb_cpu->value = current_cpu;
  if (current_cpu > 0.8f)
    pb_cpu->color = Theme::current().error;
  else if (current_cpu > 0.5f)
    pb_cpu->color = Theme::current().warning;
  else
    pb_cpu->color = Theme::current().success;

  layout->add(pb_cpu);
  layout->add(std::make_shared<VerticalSpacer>(1));

  // MEM
  auto mem_panel = std::make_shared<Horizontal>();
  mem_panel->fixed_height = 1;
  mem_panel->add(std::make_shared<Label>("RAM: "));

  auto mem_txt = std::make_shared<Label>("0%");
  mem_txt->fixed_width = 5;
  // @TODO: link to system monitor
  mem_panel->add(mem_txt);
  layout->add(mem_panel);

  auto pb_mem = std::make_shared<ProgressBar>();
  // @TODO: lint to system monitor
  layout->add(pb_mem);
  layout->add(std::make_shared<VerticalSpacer>(1));

  panel->add(layout);
  return panel;
}

} // namespace ui::panels
