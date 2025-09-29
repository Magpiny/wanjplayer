#ifndef __QUEUE_MANAGER_HPP
#define __QUEUE_MANAGER_HPP

#include <vector>
#include <random>
#include <chrono>

namespace utils {

// File queue management utilities
class QueueManager {
public:
    enum class RepeatMode {
        NONE,
        ONE,
        ALL
    };
    
    enum class ShuffleMode {
        OFF,
        ON
    };

private:
    mutable std::vector<size_t> shuffle_indices;
    mutable size_t current_shuffle_pos;
    RepeatMode repeat_mode;
    ShuffleMode shuffle_mode;
    mutable std::mt19937 rng;

public:
    QueueManager();
    
    // Queue manipulation
    void GenerateShuffleOrder(size_t queue_size) const;
    size_t GetNextIndex(size_t current_index, size_t queue_size) const;
    size_t GetPreviousIndex(size_t current_index, size_t queue_size) const;
    
    // Mode management
    void SetRepeatMode(RepeatMode mode) { repeat_mode = mode; }
    RepeatMode GetRepeatMode() const { return repeat_mode; }
    void SetShuffleMode(ShuffleMode mode);
    ShuffleMode GetShuffleMode() const { return shuffle_mode; }
    
    // Utility methods
    void Reset();
    bool ShouldRepeatCurrent() const { return repeat_mode == RepeatMode::ONE; }
    bool ShouldRepeatAll() const { return repeat_mode == RepeatMode::ALL; }
    bool IsShuffleEnabled() const { return shuffle_mode == ShuffleMode::ON; }
    
    // Advanced queue management
    void RegenerateShuffleIfNeeded(size_t queue_size) const;
    size_t GetShufflePosition() const { return current_shuffle_pos; }
    void SetShufflePosition(size_t pos) const { current_shuffle_pos = pos; }
    
    // Statistics
    size_t GetShuffleOrderSize() const { return shuffle_indices.size(); }
    bool HasValidShuffleOrder(size_t queue_size) const;
    
    // Queue validation
    bool IsValidIndex(size_t index, size_t queue_size) const;
    size_t ClampIndex(size_t index, size_t queue_size) const;
};

}

#endif // __QUEUE_MANAGER_HPP