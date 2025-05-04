/**
 * @file ais_message.h
 * @brief AIS Message Base Class
 * 
 * This file defines the abstract base class for all AIS message types.
 */

 #ifndef AISLIB_AIS_MESSAGE_H
 #define AISLIB_AIS_MESSAGE_H
 
 #include "bit_vector.h"
 #include <memory>
 #include <string>
 #include <vector>
 #include <cstdint>
 
 namespace aislib {
 
 /**
  * @class AISMessage
  * @brief Abstract base class for all AIS message types
  * 
  * AISMessage provides a common interface for all AIS message types
  * and implements shared functionality.
  */
 class AISMessage {
 public:
     /**
      * @brief Virtual destructor
      */
     virtual ~AISMessage() = default;
     
     /**
      * @brief Get the message type
      * @return Message type (1-27)
      */
     virtual uint8_t get_message_type() const = 0;
 
     /**
      * @brief Get the MMSI
      * @return MMSI number
      */
     virtual uint32_t get_mmsi() const = 0;
 
     /**
      * @brief Get the repeat indicator
      * @return Repeat indicator
      */
     virtual uint8_t get_repeat_indicator() const = 0;
     
     /**
      * @brief Convert the message to a bit vector
      * @param bits Bit vector to populate
      */
     virtual void to_bits(BitVector& bits) const = 0;
     
     /**
      * @brief Factory method to create a message from bits
      * @param bits Bit vector containing the message data
      * @return Smart pointer to the created message
      * @throws std::invalid_argument if the message type is invalid or not supported
      */
     static std::unique_ptr<AISMessage> from_bits(const BitVector& bits);
     
     /**
      * @brief Factory method to create a message from NMEA sentence
      * @param nmea NMEA sentence
      * @return Smart pointer to the created message
      * @throws std::invalid_argument if the NMEA sentence is invalid
      */
     static std::unique_ptr<AISMessage> from_nmea(const std::string& nmea);
     
     /**
      * @brief Convert the message to NMEA sentences
      * @return Vector of NMEA sentences
      */
     std::vector<std::string> to_nmea() const;
     
     /**
      * @brief Get a string representation of the message
      * @return String representation
      */
     virtual std::string to_string() const;
     
 protected:
     /**
      * @brief Protected constructor to prevent direct instantiation
      */
     AISMessage() = default;
     
     /**
      * @brief Copy constructor
      * @param other Message to copy
      */
     AISMessage(const AISMessage& other) = default;
     
     /**
      * @brief Move constructor
      * @param other Message to move
      */
     AISMessage(AISMessage&& other) = default;
     
     /**
      * @brief Copy assignment operator
      * @param other Message to copy
      * @return Reference to this message
      */
     AISMessage& operator=(const AISMessage& other) = default;
     
     /**
      * @brief Move assignment operator
      * @param other Message to move
      * @return Reference to this message
      */
     AISMessage& operator=(AISMessage&& other) = default;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_AIS_MESSAGE_H