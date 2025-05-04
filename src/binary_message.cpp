/**
 * @file binary_message.cpp
 * @brief Implementation of the BinaryMessage class
 */

 #include "aislib/binary_message.h"

 namespace aislib {
 
 BinaryMessage::BinaryMessage(uint32_t mmsi, uint8_t repeat_indicator)
     : mmsi_(mmsi),
       repeat_indicator_(repeat_indicator),
       dac_(0),
       fi_(0),
       data_() {
 }
 
 uint16_t BinaryMessage::get_dac() const {
     return dac_;
 }
 
 uint16_t BinaryMessage::get_fi() const {
     return fi_;
 }
 
 uint32_t BinaryMessage::get_application_id() const {
     return static_cast<uint32_t>(dac_) << 16 | fi_;
 }
 
 const BitVector& BinaryMessage::get_data() const {
     return data_;
 }
 
 void BinaryMessage::set_application_id(uint16_t dac, uint16_t fi) {
     dac_ = dac;
     fi_ = fi;
 }
 
 void BinaryMessage::set_data(const BitVector& data) {
     data_ = data;
 }
 
 } // namespace aislib