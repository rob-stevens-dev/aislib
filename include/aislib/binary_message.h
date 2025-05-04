/**
 * @file binary_message.h
 * @brief Binary Message Base Class
 * 
 * This file defines the base class for AIS binary messages.
 */

 #ifndef AISLIB_BINARY_MESSAGE_H
 #define AISLIB_BINARY_MESSAGE_H
 
 #include "ais_message.h"
 #include <cstdint>
 
 namespace aislib {
 
 /**
  * @class BinaryMessage
  * @brief Base class for AIS binary messages (Types 6, 8, 25, 26)
  * 
  * Binary messages provide a mechanism for transmitting application-specific
  * data via AIS.
  */
 class BinaryMessage : public AISMessage {
 public:
     /**
      * @brief Destructor
      */
     ~BinaryMessage() override = default;
     
     /**
      * @brief Get the Designated Area Code (DAC)
      * @return DAC value
      */
     uint16_t get_dac() const;
     
     /**
      * @brief Get the Function Identifier (FI)
      * @return FI value
      */
     uint16_t get_fi() const;
     
     /**
      * @brief Get the application ID (DAC + FI)
      * @return Application ID (DAC << 16 | FI)
      */
     uint32_t get_application_id() const;
     
     /**
      * @brief Get the binary data payload
      * @return Binary data as BitVector
      */
     const BitVector& get_data() const;
     
     /**
      * @brief Set the application ID (DAC and FI)
      * @param dac Designated Area Code
      * @param fi Function Identifier
      */
     void set_application_id(uint16_t dac, uint16_t fi);
     
     /**
      * @brief Set the binary data payload
      * @param data Binary data as BitVector
      */
     void set_data(const BitVector& data);
 
 protected:
     /**
      * @brief Constructor
      * @param mmsi Source MMSI number
      * @param repeat_indicator Repeat indicator
      */
     BinaryMessage(uint32_t mmsi, uint8_t repeat_indicator);
 
     /**
      * @brief Copy constructor
      * @param other BinaryMessage to copy from
      */
     BinaryMessage(const BinaryMessage& other) = default;
     
     /**
      * @brief Move constructor
      * @param other BinaryMessage to move from
      */
     BinaryMessage(BinaryMessage&& other) = default;
     
     /**
      * @brief Copy assignment operator
      * @param other BinaryMessage to copy from
      * @return Reference to this object
      */
     BinaryMessage& operator=(const BinaryMessage& other) = default;
     
     /**
      * @brief Move assignment operator
      * @param other BinaryMessage to move from
      * @return Reference to this object
      */
     BinaryMessage& operator=(BinaryMessage&& other) = default;
 
     // Protected member variables
     uint32_t mmsi_;
     uint8_t repeat_indicator_;
     uint16_t dac_;
     uint16_t fi_;
     BitVector data_;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_BINARY_MESSAGE_H