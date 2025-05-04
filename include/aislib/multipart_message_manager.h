/**
 * @file multipart_message_manager.h
 * @brief Multipart Message Manager
 * 
 * This file defines the MultipartMessageManager class, which handles
 * reassembly of multipart AIS messages.
 */

 #ifndef AISLIB_MULTIPART_MESSAGE_MANAGER_H
 #define AISLIB_MULTIPART_MESSAGE_MANAGER_H
 
 #include "bit_vector.h"
 #include <map>
 #include <string>
 #include <vector>
 #include <chrono>
 #include <optional>
 #include <memory>
 
 namespace aislib {
 
 /**
  * @class MultipartMessageManager
  * @brief Handles reassembly of multipart AIS messages
  */
 class MultipartMessageManager {
 public:
     /**
      * @brief Constructor
      * @param timeout Timeout for incomplete messages
      * @param max_messages Maximum number of incomplete messages to track
      */
     explicit MultipartMessageManager(
         std::chrono::seconds timeout = std::chrono::seconds(60),
         size_t max_messages = 100
     );
     
     /**
      * @brief Add a message fragment
      * @param fragment_number Fragment number (1-based)
      * @param fragment_count Total number of fragments
      * @param message_id Message ID
      * @param channel AIS channel (A or B)
      * @param payload Fragment payload
      * @param fill_bits Number of fill bits
      * @return Combined payload if all fragments are received, empty optional otherwise
      */
     std::optional<BitVector> add_fragment(
         uint8_t fragment_number,
         uint8_t fragment_count,
         const std::string& message_id,
         char channel,
         const std::string& payload,
         uint8_t fill_bits
     );
     
     /**
      * @brief Remove expired message fragments
      */
     void cleanup_expired();
     
     /**
      * @brief Clear all incomplete messages
      */
     void clear();
     
     /**
      * @brief Get the number of incomplete messages
      * @return Number of incomplete messages
      */
     size_t get_incomplete_count() const;
     
     /**
      * @brief Set the timeout for incomplete messages
      * @param timeout Timeout
      */
     void set_timeout(std::chrono::seconds timeout);
     
     /**
      * @brief Set the maximum number of incomplete messages to track
      * @param max_messages Maximum number of messages
      */
     void set_max_messages(size_t max_messages);
     
 private:
     // Multipart message key
     struct MessageKey {
         std::string message_id;
         char channel;
         
         bool operator<(const MessageKey& other) const;
     };
     
     // Fragment information
     struct Fragment {
         std::string payload;
         uint8_t fill_bits;
         bool received;
     };
     
     // Multipart message information
     struct MessageInfo {
         std::vector<Fragment> fragments;
         std::chrono::steady_clock::time_point last_update;
         uint8_t received_count;
     };
     
     // Map of message keys to message information
     std::map<MessageKey, MessageInfo> messages_;
     
     // Timeout for incomplete messages
     std::chrono::seconds timeout_;
     
     // Maximum number of incomplete messages to track
     size_t max_messages_;
     
     // Combine fragments into a single bit vector
     BitVector combine_fragments(const std::vector<Fragment>& fragments);
 };
 
 } // namespace aislib
 
 #endif // AISLIB_MULTIPART_MESSAGE_MANAGER_H