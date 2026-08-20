#ifndef GEOCUBE_CORE_HIGH_SCORES_H
#define GEOCUBE_CORE_HIGH_SCORES_H

#include <cstddef>
#include <string>
#include <vector>

namespace geocube::core {

struct HighScoreEntry {
  std::string playerName;
  int score = 0;
  int level = 0;
};

bool insertHighScore(std::vector<HighScoreEntry>& scores,
                     HighScoreEntry entry, std::size_t limit = 10);

} // namespace geocube::core

#endif
