/**
 * @file binary_addressed_message.h
 * @brief Binary Addressed Message (Type 6)
 * 
 * This file defines the Binary Addressed Message (Type 6) used
 * to send application-specific data to a specific destination.
 */

 #ifndef AISLIB_BINARY_ADDRESSED_MESSAGE_H
 #define AISLIB_BINARY_ADDRESSED_MESSAGE_H
 
 #include "binary_message.h"
 #include <cstdint>
 
 namespace aislib {
 
 /**
  * @class BinaryAddressedMessage
  * @brief Class for AIS message type 6 (Binary Addressed Message)
  * 
  * Message type 6 provides a mechanism for sending application-specific
  * data to a specific recipient via AIS.
  */
 class BinaryAddressedMessage : public BinaryMessage {
 public:
     /**
      * @brief Constructor
      * @param mmsi Source MMSI number
      * @param dest_mmsi Destination MMSI number
      * @param sequence_number Sequence number
      * @param repeat_indicator Repeat indicator
      */
     BinaryAddressedMessage(uint32_t mmsi, uint32_t dest_mmsi, uint8_t sequence_number, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit BinaryAddressedMessage(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~BinaryAddressedMessage() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (6)
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
      * @brief Get the destination MMSI
      * @return Destination MMSI number
      */
     uint32_t get_dest_mmsi() const;
 
     /**
      * @brief Get the sequence number
      * @return Sequence number
      */
     uint8_t get_sequence_number() const;
 
     /**
      * @brief Get the retransmit flag
      * @return Retransmit flag
      */
     bool get_retransmit_flag() const;
 
     /**
      * @brief Set the destination MMSI
      * @param dest_mmsi Destination MMSI number
      */
     void set_dest_mmsi(uint32_t dest_mmsi);
 
     /**
      * @brief Set the sequence number
      * @param sequence_number Sequence number
      */
     void set_sequence_number(uint8_t sequence_number);
 
     /**
      * @brief Set the retransmit flag
      * @param retransmit Retransmit flag
      */
     void set_retransmit_flag(bool retransmit);
 
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
 
 private:
     uint32_t dest_mmsi_;
     uint8_t sequence_number_;
     bool retransmit_flag_;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_BINARY_ADDRESSED_MESSAGE_H