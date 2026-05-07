#pragma once

#include "db/Database.hpp"
#include "models/Checkpoint.hpp"
#include <memory>
#include <vector>

class CheckpointRepo {
public:
  explicit CheckpointRepo(std::shared_ptr<Database> &db,
                          std::shared_ptr<Logger> logger);

  void initTable();
  void insert(const Checkpoint &checkpoint);
  std::vector<Checkpoint> getAll();
  std::vector<Checkpoint> getUnsynced();
  void updateSyncStatus(int id, bool is_sync);
  void deleteOlderThan(long long timemillis);

private:
  std::shared_ptr<Database> db_;
  std::shared_ptr<Logger> logger_;
};
