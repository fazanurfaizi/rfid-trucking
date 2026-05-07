#include "repo/CheckpointRepo.hpp"
#include "db/Database.hpp"
#include "models/Checkpoint.hpp"
#include "utils/Logger.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

CheckpointRepo::CheckpointRepo(std::shared_ptr<Database> &db,
                               std::shared_ptr<Logger> logger)
    : db_(db), logger_(logger) {
  this->initTable();
}

void CheckpointRepo::initTable() {
  std::string query = R"(
    CREATE TABLE IF NOT EXISTS checkpoints (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      checkpoint_code TEXT NOT NULL,
      rfid_code TEXT NOT NULL,
      timemillis INTEGER,
      is_sync BOOLEAN
    );
  )";
  this->db_->execute(query);
}

void CheckpointRepo::insert(const Checkpoint &cp) {
  const char *sql = "INSERT INTO checkpoints (checkpoint_code, rfid_code, "
                    "timemillis, is_sync) VALUES (?, ?, ?, ?)";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(this->db_->getHandle(), sql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    const std::string err_msg =
        std::string("Failed to prepare insert statement: ") +
        sqlite3_errmsg(this->db_->getHandle());
    this->logger_->error(err_msg);
    throw std::runtime_error(err_msg);
  }

  sqlite3_bind_text(stmt, 1, cp.checkpoint_code.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, cp.rfid_code.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int64(stmt, 3, cp.timemillis);
  sqlite3_bind_int(stmt, 4, cp.is_sync ? 1 : 0);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    const std::string err_msg =
        std::string("Failed to execute insert statement") +
        sqlite3_errmsg(this->db_->getHandle());

    sqlite3_finalize(stmt);
    this->logger_->error(err_msg);
    throw std::runtime_error(err_msg);
  }

  sqlite3_finalize(stmt);
}

std::vector<Checkpoint> CheckpointRepo::getAll() {
  std::vector<Checkpoint> results;
  const char *sql = "SELECT id, checkpoint_code, rfid_code, timemillis, "
                    "is_sync FROM checkpoints ORDER BY timemillis DESC";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(this->db_->getHandle(), sql, -1, &stmt, nullptr) ==
      SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Checkpoint cp;
      cp.id = sqlite3_column_int(stmt, 0);
      cp.checkpoint_code =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
      cp.rfid_code =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
      cp.timemillis = sqlite3_column_int64(stmt, 3);
      cp.is_sync = sqlite3_column_int(stmt, 4) != 0;
      results.push_back(cp);
    }
  }

  sqlite3_finalize(stmt);

  return results;
}

std::vector<Checkpoint> CheckpointRepo::getUnsynced() {
  std::vector<Checkpoint> results;
  const char *sql = "SELECT id, checkpoint_code, rfid_code, timemillis, "
                    "is_sync FROM checkpoints WHERE is_sync = 0";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(this->db_->getHandle(), sql, -1, &stmt, nullptr) ==
      SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Checkpoint cp;
      cp.id = sqlite3_column_int(stmt, 0);
      cp.checkpoint_code =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
      cp.rfid_code =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
      cp.timemillis = sqlite3_column_int64(stmt, 3);
      cp.is_sync = sqlite3_column_int(stmt, 4) != 0;
      results.push_back(cp);
    }
  }

  sqlite3_finalize(stmt);

  return results;
}

void CheckpointRepo::updateSyncStatus(int id, bool is_sync) {
  const char *sql = "UPDATE checkpoints SET is_sync = ? WHERE id = ?";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(this->db_->getHandle(), sql, -1, &stmt, nullptr) ==
      SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, is_sync ? 1 : 0);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
}

void CheckpointRepo::deleteOlderThan(long long timemillis) {
  const char *sql = "DELETE FROM checkpoints WHERE timemillis < ?";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(this->db_->getHandle(), sql, -1, &stmt, nullptr) ==
      SQLITE_OK) {
    sqlite3_bind_int64(stmt, 1, timemillis);
    sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
}
