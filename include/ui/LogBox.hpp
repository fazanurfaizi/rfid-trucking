#include "cpptui.hpp"
#include "ui/MessageStore.hpp"

class LogBox : public cpptui::Widget {
private:
  std::shared_ptr<MessageStore> store_;

public:
  LogBox(std::shared_ptr<MessageStore> store) : store_(store) {}

  void render(cpptui::Buffer &buffer) override {
    // Clear background
    for (int dy = 0; dy < height; ++dy) {
      for (int dx = 0; dx < width; ++dx) {
        cpptui::Cell cell;
        cell.content = " ";
        cell.bg_color = cpptui::Color{20, 20, 40};
        buffer.set(x + dx, y + dy, cell);
      }
    }

    // Fetch messages safely
    auto msgs = store_->getMessages();

    // Calculate how many messages fit on screen
    int start_idx = 0;
    if (msgs.size() > height) {
      start_idx = msgs.size() - height; // Keep scroll at bottom
    }

    // Draw text
    for (int i = 0; i < height && (start_idx + i) < msgs.size(); ++i) {
      std::string line = msgs[start_idx + i];

      // Truncate line if it's wider than the widget
      if (line.length() > width)
        line = line.substr(0, width);

      for (size_t c = 0; c < line.length(); ++c) {
        cpptui::Cell cell;
        cell.content = std::string(1, line[c]);
        cell.bg_color = cpptui::Color{20, 20, 40};
        cell.fg_color = cpptui::Color::White();
        buffer.set(x + c, y + i, cell);
      }
    }
  }
};
