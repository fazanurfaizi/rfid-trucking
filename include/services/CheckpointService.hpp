#pragma once

#include "repo/CheckpointRepo.hpp"
#include <memory>
#include <string>

class CheckpointService {
public:
  explicit CheckpointService(CheckpointRepo &repo,
                             std::shared_ptr<Logger> logger);

  std::vector<Checkpoint> getAll(int page, int limit = 20);
  void processRfidScan(const std::string &checkpoint_code,
                       const std::string &rfid_code, long long current_millis);
  void processSync();
  void removeOldCheckpoints(int retention_days);

private:
  CheckpointRepo &repo_;
  std::shared_ptr<Logger> logger_;
};
