#include "core/high_scores.h"

#include <algorithm>
#include <utility>

namespace geocube::core {

bool insertHighScore(std::vector<HighScoreEntry>& scores,
                     HighScoreEntry entry, std::size_t limit)
{
  if (limit == 0)
    return false;

  std::stable_sort(scores.begin(), scores.end(),
                   [](const HighScoreEntry& left, const HighScoreEntry& right) {
                     return left.score > right.score;
                   });

  const auto insertionPoint = std::lower_bound(
      scores.begin(), scores.end(), entry.score,
      [](const HighScoreEntry& existing, int score) {
        return existing.score > score;
      });

  if (scores.size() >= limit && insertionPoint == scores.end())
    return false;

  scores.insert(insertionPoint, std::move(entry));
  if (scores.size() > limit)
    scores.pop_back();
  return true;
}

} // namespace geocube::core
