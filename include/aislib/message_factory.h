/**
 * @file message_factory.h
 * @brief Message Factory
 * 
 * This file defines the MessageFactory class, which creates appropriate
 * message objects based on the message type.
 */

 #ifndef AISLIB_MESSAGE_FACTORY_H
 #define AISLIB_MESSAGE_FACTORY_H
 
 #include "ais_message.h"
 #include "bit_vector.h"
 #include <memory>
 #include <functional>
 #include <map>
 #include <stdexcept>
 
 namespace aislib {
 
 /**
  * @class MessageFactory
  * @brief Factory for creating AIS message objects
  */
 class MessageFactory {
 public:
     /**
      * @brief Get the singleton instance
      * @return Reference to the singleton instance
      */
     static MessageFactory& instance();
     
     /**
      * @brief Create a message from bits
      * @param bits Bit vector containing the message data
      * @return Smart pointer to the created message
      * @throws std::invalid_argument if the message type is invalid or not supported
      */
     std::unique_ptr<AISMessage> create_message(const BitVector& bits) const;
     
     /**
      * @brief Register a message type constructor
      * @param message_type Message type (1-27)
      * @param constructor Function to create a message from bits
      */
     void register_message_type(
         uint8_t message_type,
         std::function<std::unique_ptr<AISMessage>(const BitVector&)> constructor
     );
     
     /**
      * @brief Check if a message type is registered
      * @param message_type Message type
      * @return true if registered, false otherwise
      */
     bool is_message_type_registered(uint8_t message_type) const;
     
 private:
     /**
      * @brief Private constructor (singleton pattern)
      */
     MessageFactory();
     
     /**
      * @brief Constructor map
      */
     std::map<uint8_t, std::function<std::unique_ptr<AISMessage>(const BitVector&)>> constructors_;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_MESSAGE_FACTORY_H