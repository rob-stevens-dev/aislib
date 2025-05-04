/**
 * @file binary_broadcast_message.h
 * @brief Binary Broadcast Message (Type 8)
 * 
 * This file defines the Binary Broadcast Message (Type 8) used
 * to broadcast application-specific data.
 */

 #ifndef AISLIB_BINARY_BROADCAST_MESSAGE_H
 #define AISLIB_BINARY_BROADCAST_MESSAGE_H
 
 #include "binary_message.h"
 #include <cstdint>
 
 namespace aislib {
 
 /**
  * @class BinaryBroadcastMessage
  * @brief Class for AIS message type 8 (Binary Broadcast Message)
  * 
  * Message type 8 provides a mechanism for broadcasting application-specific
  * data via AIS.
  */
 class BinaryBroadcastMessage : public BinaryMessage {
 public:
     /**
      * @brief Constructor
      * @param mmsi Source MMSI number
      * @param repeat_indicator Repeat indicator
      */
     BinaryBroadcastMessage(uint32_t mmsi, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit BinaryBroadcastMessage(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~BinaryBroadcastMessage() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (8)
      */
     uint8_t get_message_type() const override;
 
     /**
      * @brief Get the MMSI
      * @return MMSI number
      */
     uint32_t get_mmsi() const override;
 
     /**
      * @brief Get the repeat indicator
      * @return Repeat indicator
      */
     uint8_t get_repeat_indicator() const override;
 
     /**
      * @brief Convert the message to a bit vector
      * @param bits Bit vector to populate
      */
     void to_bits(BitVector& bits) const override;
 
     /**
      * @brief Get a string representation of the message
      * @return String representation
      */
     std::string to_string() const override;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_BINARY_BROADCAST_MESSAGE_H