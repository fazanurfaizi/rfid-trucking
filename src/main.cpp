#include "config/AppConfig.hpp"
#include "core/AppContainer.hpp"
#include "core/MqttGateway.hpp"
#include "ui/Dashboard.hpp"

int main() {
  AppConfig config = loadConfig();

  AppContainer container(config);

  Dashboard dashboard(container, config);
  MqttGateway mqtt(container, config);

  mqtt.start([&dashboard]() { dashboard.triggerCheckpointUpdate(); });

  dashboard.run();

  mqtt.stop();

  return 0;
}
