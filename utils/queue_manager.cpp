#include "queue_manager.hpp"
#include <algorithm>

namespace utils {

QueueManager::QueueManager()
    : current_shuffle_pos(0)
    , repeat_mode(RepeatMode::NONE)
    , shuffle_mode(ShuffleMode::OFF)
    , rng(std::chrono::steady_clock::now().time_since_epoch().count())
{
}

void QueueManager::GenerateShuffleOrder(size_t queue_size) const
{
    if (queue_size == 0) {
        shuffle_indices.clear();
        return;
    }
    
    shuffle_indices.clear();
    shuffle_indices.reserve(queue_size);
    
    for (size_t i = 0; i < queue_size; ++i) {
        shuffle_indices.push_back(i);
    }
    
    std::shuffle(shuffle_indices.begin(), shuffle_indices.end(), rng);
    current_shuffle_pos = 0;
}

size_t QueueManager::GetNextIndex(size_t current_index, size_t queue_size) const
{
    if (queue_size == 0) {
        return 0;
    }
    
    if (repeat_mode == RepeatMode::ONE) {
        return current_index;
    }
    
    if (shuffle_mode == ShuffleMode::ON) {
        if (shuffle_indices.empty() || shuffle_indices.size() != queue_size) {
            GenerateShuffleOrder(queue_size);
        }
        
        // Find current position in shuffle order
        auto it = std::find(shuffle_indices.begin(), shuffle_indices.end(), current_index);
        if (it != shuffle_indices.end()) {
            current_shuffle_pos = std::distance(shuffle_indices.begin(), it);
        }
        
        current_shuffle_pos++;
        if (current_shuffle_pos >= shuffle_indices.size()) {
            if (repeat_mode == RepeatMode::ALL) {
                current_shuffle_pos = 0;
                return shuffle_indices[current_shuffle_pos];
            } else {
                return current_index; // Stay at current if no repeat
            }
        }
        
        return shuffle_indices[current_shuffle_pos];
    } else {
        // Normal sequential mode
        size_t next_index = current_index + 1;
        if (next_index >= queue_size) {
            if (repeat_mode == RepeatMode::ALL) {
                return 0;
            } else {
                return current_index; // Stay at last track
            }
        }
        return next_index;
    }
}

size_t QueueManager::GetPreviousIndex(size_t current_index, size_t queue_size) const
{
    if (queue_size == 0) {
        return 0;
    }
    
    if (repeat_mode == RepeatMode::ONE) {
        return current_index;
    }
    
    if (shuffle_mode == ShuffleMode::ON) {
        if (shuffle_indices.empty() || shuffle_indices.size() != queue_size) {
            GenerateShuffleOrder(queue_size);
        }
        
        // Find current position in shuffle order
        auto it = std::find(shuffle_indices.begin(), shuffle_indices.end(), current_index);
        if (it != shuffle_indices.end()) {
            current_shuffle_pos = std::distance(shuffle_indices.begin(), it);
        }
        
        if (current_shuffle_pos == 0) {
            if (repeat_mode == RepeatMode::ALL) {
                current_shuffle_pos = shuffle_indices.size() - 1;
                return shuffle_indices[current_shuffle_pos];
            } else {
                return current_index; // Stay at current if no repeat
            }
        }
        
        current_shuffle_pos--;
        return shuffle_indices[current_shuffle_pos];
    } else {
        // Normal sequential mode
        if (current_index == 0) {
            if (repeat_mode == RepeatMode::ALL) {
                return queue_size - 1;
            } else {
                return 0; // Stay at first track
            }
        }
        return current_index - 1;
    }
}

void QueueManager::SetShuffleMode(ShuffleMode mode)
{
    shuffle_mode = mode;
    if (mode == ShuffleMode::OFF) {
        shuffle_indices.clear();
        current_shuffle_pos = 0;
    }
}

void QueueManager::Reset()
{
    shuffle_indices.clear();
    current_shuffle_pos = 0;
    repeat_mode = RepeatMode::NONE;
    shuffle_mode = ShuffleMode::OFF;
}

void QueueManager::RegenerateShuffleIfNeeded(size_t queue_size) const
{
    if (shuffle_mode == ShuffleMode::ON && 
        (shuffle_indices.empty() || shuffle_indices.size() != queue_size)) {
        GenerateShuffleOrder(queue_size);
    }
}

bool QueueManager::HasValidShuffleOrder(size_t queue_size) const
{
    return shuffle_indices.size() == queue_size && 
           current_shuffle_pos < shuffle_indices.size();
}

bool QueueManager::IsValidIndex(size_t index, size_t queue_size) const
{
    return index < queue_size;
}

size_t QueueManager::ClampIndex(size_t index, size_t queue_size) const
{
    if (queue_size == 0) return 0;
    return std::min(index, queue_size - 1);
}

}