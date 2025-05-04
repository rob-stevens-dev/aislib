/**
 * @file multipart_message_manager.cpp
 * @brief Implementation of MultipartMessageManager class
 */

 #include "aislib/multipart_message_manager.h"
 #include <stdexcept>
 #include <algorithm>
 
 namespace aislib {
 
 bool MultipartMessageManager::MessageKey::operator<(const MessageKey& other) const {
     if (message_id != other.message_id) {
         return message_id < other.message_id;
     }
     return channel < other.channel;
 }
 
 MultipartMessageManager::MultipartMessageManager(
     std::chrono::seconds timeout,
     size_t max_messages
 ) : timeout_(timeout), max_messages_(max_messages) {
 }
 
 std::optional<BitVector> MultipartMessageManager::add_fragment(
    uint8_t fragment_number,
    uint8_t fragment_count,
    const std::string& message_id,
    char channel,
    const std::string& payload,
    uint8_t fill_bits
) {
    // Validate inputs
    if (fragment_number < 1 || fragment_number > fragment_count) {
        throw std::invalid_argument("Invalid fragment number");
    }
    
    if (channel != 'A' && channel != 'B') {
        throw std::invalid_argument("Invalid channel, must be 'A' or 'B'");
    }
    
    if (fill_bits > 5) {
        throw std::invalid_argument("Invalid fill bits, must be 0-5");
    }
    
    // Use a sequential ID if message_id is empty
    std::string effective_message_id = message_id;
    if (effective_message_id.empty()) {
        effective_message_id = "seq" + std::to_string(fragment_count);
    }
    
    // Create the message key
    MessageKey key{effective_message_id, channel};
    
    // Check if we already have this message
    auto it = messages_.find(key);
    if (it == messages_.end()) {
        // Create a new message info
        MessageInfo info;
        info.fragments.resize(fragment_count);
        info.last_update = std::chrono::steady_clock::now();
        info.received_count = 0;
        
        // Initialize all fragments as not received
        for (auto& fragment : info.fragments) {
            fragment.received = false;
        }
        
        // Insert the new message info
        it = messages_.insert({key, std::move(info)}).first;
        
        // Clean up expired messages if we have too many
        if (messages_.size() > max_messages_) {
            // Find the oldest message
            auto oldest_it = messages_.begin();
            auto oldest_time = oldest_it->second.last_update;
            
            for (auto message_it = messages_.begin(); message_it != messages_.end(); ++message_it) {
                if (message_it->second.last_update < oldest_time) {
                    oldest_it = message_it;
                    oldest_time = message_it->second.last_update;
                }
            }
            
            // Remove the oldest message
            if (oldest_it != messages_.end()) {
                messages_.erase(oldest_it);
            }
        }
    }
    
    // Update the fragment
    MessageInfo& info = it->second;
    Fragment& fragment = info.fragments[fragment_number - 1];
    
    // Check if this fragment was already received
    if (!fragment.received) {
        fragment.payload = payload;
        fragment.fill_bits = fill_bits;
        fragment.received = true;
        
        // Update the received count and last update time
        ++info.received_count;
        info.last_update = std::chrono::steady_clock::now();
    }
    
    // Check if all fragments are received
    if (info.received_count == info.fragments.size()) {
        // Combine all fragments
        BitVector combined = combine_fragments(info.fragments);
        
        // Remove the message from the map
        messages_.erase(it);
        
        // Return the combined bit vector
        return combined;
    }
    
    // Not all fragments received yet
    return std::nullopt;
}
 
 void MultipartMessageManager::cleanup_expired() {
     auto now = std::chrono::steady_clock::now();
     
     for (auto it = messages_.begin(); it != messages_.end();) {
         auto elapsed = now - it->second.last_update;
         
         if (elapsed > timeout_) {
             it = messages_.erase(it);
         } else {
             ++it;
         }
     }
 }
 
 void MultipartMessageManager::clear() {
     messages_.clear();
 }
 
 size_t MultipartMessageManager::get_incomplete_count() const {
     return messages_.size();
 }
 
 void MultipartMessageManager::set_timeout(std::chrono::seconds timeout) {
     timeout_ = timeout;
 }
 
 void MultipartMessageManager::set_max_messages(size_t max_messages) {
    max_messages_ = max_messages;
    
    // Clean up if we have too many incomplete messages
    if (messages_.size() > max_messages_) {
        // Sort messages by last update time
        std::vector<std::pair<std::chrono::steady_clock::time_point, std::map<MessageKey, MessageInfo>::iterator>> time_sorted;
        
        for (auto it = messages_.begin(); it != messages_.end(); ++it) {
            time_sorted.push_back({it->second.last_update, it});
        }
        
        // Sort by time (oldest first)
        std::sort(time_sorted.begin(), time_sorted.end(), 
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            });
        
        // Keep only the newest max_messages_ messages
        size_t to_remove = messages_.size() - max_messages_;
        for (size_t i = 0; i < to_remove; ++i) {
            messages_.erase(time_sorted[i].second);
        }
    }
}
 
 BitVector MultipartMessageManager::combine_fragments(const std::vector<Fragment>& fragments) {
     BitVector combined;
     
     // Calculate the total size needed (excluding fill bits in the last fragment)
     size_t total_bits = 0;
     for (size_t i = 0; i < fragments.size(); ++i) {
         const Fragment& fragment = fragments[i];
         
         // Convert payload to bits
         BitVector fragment_bits(fragment.payload);
         
         // Add all bits except fill bits in the last fragment
         if (i == fragments.size() - 1) {
             total_bits += fragment_bits.size() - fragment.fill_bits;
         } else {
             total_bits += fragment_bits.size();
         }
     }
     
     // Reserve capacity for the combined bit vector
     combined.reserve(total_bits);
     
     // Combine all fragments
     for (size_t i = 0; i < fragments.size(); ++i) {
         const Fragment& fragment = fragments[i];
         
         // Convert payload to bits
         BitVector fragment_bits(fragment.payload);
         
         // Add all bits except fill bits in the last fragment
         if (i == fragments.size() - 1) {
             size_t bits_to_copy = fragment_bits.size() - fragment.fill_bits;
             
             for (size_t j = 0; j < bits_to_copy; ++j) {
                 combined.append_bit(fragment_bits.get_bit(j));
             }
         } else {
             for (size_t j = 0; j < fragment_bits.size(); ++j) {
                 combined.append_bit(fragment_bits.get_bit(j));
             }
         }
     }
     
     return combined;
 }
 
 } // namespace aislib