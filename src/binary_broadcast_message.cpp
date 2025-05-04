/**
 * @file binary_broadcast_message.cpp
 * @brief Implementation of the BinaryBroadcastMessage class
 */

 #include "aislib/binary_broadcast_message.h"
 #include <sstream>
 #include <iomanip>
 
 namespace aislib {
 
 BinaryBroadcastMessage::BinaryBroadcastMessage(uint32_t mmsi, uint8_t repeat_indicator)
     : BinaryMessage(mmsi, repeat_indicator) {
 }
 
 BinaryBroadcastMessage::BinaryBroadcastMessage(const BitVector& bits)
     : BinaryMessage(0, 0) {
     
     // Check message type
     if (bits.get_uint(0, 6) != 8) {
         throw std::invalid_argument("Invalid message type for Binary Broadcast Message");
     }
     
     // Parse the message fields
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     
     // Spare bit (2 bits) - skipped
     
     // Parse application ID
     dac_ = static_cast<uint16_t>(bits.get_uint(40, 10));
     fi_ = static_cast<uint16_t>(bits.get_uint(50, 6));
     
     // Parse application data
     // The rest of the bits are application data
     size_t data_start = 56;
     size_t data_length = bits.size() - data_start;
     
     if (data_length > 0) {
         data_ = BitVector();
         for (size_t i = 0; i < data_length; ++i) {
             data_.append_bit(bits.get_bit(data_start + i));
         }
     }
 }
 
 uint8_t BinaryBroadcastMessage::get_message_type() const {
     return 8;
 }
 
 uint32_t BinaryBroadcastMessage::get_mmsi() const {
     return mmsi_;
 }
 
 uint8_t BinaryBroadcastMessage::get_repeat_indicator() const {
     return repeat_indicator_;
 }
 
 void BinaryBroadcastMessage::to_bits(BitVector& bits) const {
     // Message Type (6 bits)
     bits.append_uint(get_message_type(), 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(repeat_indicator_, 2);
     
     // MMSI (30 bits)
     bits.append_uint(mmsi_, 30);
     
     // Spare (2 bits)
     bits.append_uint(0, 2);
     
     // DAC (10 bits)
     bits.append_uint(dac_, 10);
     
     // FI (6 bits)
     bits.append_uint(fi_, 6);
     
     // Application Data (variable length)
     if (data_.size() > 0) {
         for (size_t i = 0; i < data_.size(); ++i) {
             bits.append_bit(data_.get_bit(i));
         }
     }
 }
 
 std::string BinaryBroadcastMessage::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << " (Binary Broadcast Message)\n";
     ss << "MMSI: " << mmsi_ << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(repeat_indicator_) << "\n";
     ss << "Application ID: DAC=" << dac_ << ", FI=" << fi_ << "\n";
     ss << "Data Length: " << data_.size() << " bits\n";
     
     if (data_.size() > 0) {
         ss << "Data (hex): " << data_.to_hex();
     }
     
     return ss.str();
 }
 
 } // namespace aislib