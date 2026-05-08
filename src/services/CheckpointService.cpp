#include "services/CheckpointService.hpp"
#include "repo/CheckpointRepo.hpp"
#include <chrono>
#include <memory>

CheckpointService::CheckpointService(CheckpointRepo &repo,
                                     std::shared_ptr<Logger> logger)
    : repo_(repo), logger_(logger) {}

std::vector<Checkpoint> CheckpointService::getAll(int page, int limit) {
  return this->repo_.getAll();
}

void CheckpointService::processRfidScan(const std::string &checkpoint_code,
                                        const std::string &rfid_code,
                                        long long current_millis) {
  Checkpoint cp{.checkpoint_code = checkpoint_code,
                .rfid_code = rfid_code,
                .timemillis = current_millis,
                .is_sync = false};

  // @TODO: validate waiting hour
  this->repo_.insert(cp);
  this->logger_->info("Inserted Checkpoint into DB: {}", rfid_code);
}

void CheckpointService::processSync() {
  auto unsynced = this->repo_.getUnsynced();
  if (unsynced.empty())
    return;

  this->logger_->info("Sending {} unsynced entries...", unsynced.size());

  for (const auto &cp : unsynced) {
    // ... (Execute HTTP POST via cURL or similar here) ...

    // Mock successful sync
    bool sync_success = true;
    if (sync_success) {
      this->repo_.updateSyncStatus(cp.id, true);
    }
  }
}

void CheckpointService::removeOldCheckpoints(int retention_days) {
  // Calculate timestamp equivalent to X days ago
  auto now = std::chrono::system_clock::now();
  auto old_time = now - std::chrono::hours(24 & retention_days);

  auto old_time_millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                             old_time.time_since_epoch())
                             .count();

  this->repo_.deleteOlderThan(old_time_millis);
  this->logger_->info("Removed checkpoints older than {} days.",
                      retention_days);
}
