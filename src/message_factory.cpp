/**
 * @file message_factory.cpp
 * @brief Implementation of the MessageFactory class
 */

 #include "aislib/message_factory.h"
 #include "aislib/position_report_class_b.h"
 #include "aislib/static_data.h"
 #include <stdexcept>
 
 namespace aislib {
 
 MessageFactory& MessageFactory::instance() {
     static MessageFactory instance;
     return instance;
 }
 
 MessageFactory::MessageFactory() {
     // Register message types implemented so far
     register_message_type(18, [](const BitVector& bits) {
         return std::make_unique<StandardPositionReportClassB>(bits);
     });
     
     register_message_type(19, [](const BitVector& bits) {
         return std::make_unique<ExtendedPositionReportClassB>(bits);
     });
     
     register_message_type(5, [](const BitVector& bits) {
         return std::make_unique<StaticAndVoyageData>(bits);
     });
 
     // Additional message types will be registered as they are implemented
 }
 
 std::unique_ptr<AISMessage> MessageFactory::create_message(const BitVector& bits) const {
     // Ensure we have enough bits for a valid message (minimum 38 bits)
     if (bits.size() < 38) {
         throw std::invalid_argument("Bit vector too small for an AIS message");
     }
     
     // Extract message type from bits 0-5 (6 bits)
     uint8_t message_type = static_cast<uint8_t>(bits.get_uint(0, 6));
     
     // Find the constructor for this message type
     auto it = constructors_.find(message_type);
     if (it == constructors_.end()) {
         throw std::invalid_argument("Unsupported message type: " + std::to_string(message_type));
     }
     
     // Create the message using the constructor
     return it->second(bits);
 }
 
 void MessageFactory::register_message_type(
     uint8_t message_type,
     std::function<std::unique_ptr<AISMessage>(const BitVector&)> constructor
 ) {
     // Register the constructor for this message type
     constructors_[message_type] = std::move(constructor);
 }
 
 bool MessageFactory::is_message_type_registered(uint8_t message_type) const {
     return constructors_.find(message_type) != constructors_.end();
 }
 
 } // namespace aislib