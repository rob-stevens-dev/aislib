/**
 * @file bit_vector.h
 * @brief BitVector class for bit-level operations
 * 
 * This file defines the BitVector class, which provides functionality
 * for bit-level manipulations required for AIS message processing.
 */

 #ifndef AISLIB_BIT_VECTOR_H
 #define AISLIB_BIT_VECTOR_H
 
 #include <vector>
 #include <cstdint>
 #include <string>
 #include <stdexcept>
 
 namespace aislib {
 
 /**
  * @class BitVector
  * @brief Class for bit-level operations
  * 
  * BitVector provides functionality for packing and extracting bits,
  * which is essential for AIS message encoding and decoding.
  */
 class BitVector {
 public:
     /**
      * @brief Default constructor
      */
     BitVector();
     
     /**
      * @brief Constructor with initial capacity
      * @param capacity Initial capacity in bits
      */
     explicit BitVector(size_t capacity);
     
     /**
      * @brief Constructor from 6-bit encoded NMEA payload
      * @param payload Encoded NMEA payload string
      */
     explicit BitVector(const std::string& payload);
     
     /**
      * @brief Get the number of bits
      * @return Number of bits
      */
     size_t size() const;
     
     /**
      * @brief Get the capacity in bits
      * @return Capacity in bits
      */
     size_t capacity() const;
     
     /**
      * @brief Reserve capacity
      * @param capacity Capacity in bits
      */
     void reserve(size_t capacity);
     
     /**
      * @brief Clear the bit vector
      */
     void clear();
     
     /**
      * @brief Get a bit value
      * @param index Bit index
      * @return Bit value (0 or 1)
      * @throws std::out_of_range if index is out of range
      */
     bool get_bit(size_t index) const;
     
     /**
      * @brief Set a bit value
      * @param index Bit index
      * @param value Bit value (true = 1, false = 0)
      * @throws std::out_of_range if index is out of range
      */
     void set_bit(size_t index, bool value);
     
     /**
      * @brief Append a bit
      * @param bit Bit value (true = 1, false = 0)
      */
     void append_bit(bool bit);
     
     /**
      * @brief Get an unsigned integer value
      * @param start_index Start bit index
      * @param bit_count Number of bits to read (max 64)
      * @return Unsigned integer value
      * @throws std::out_of_range if indices are out of range
      * @throws std::invalid_argument if bit_count > 64
      */
     uint64_t get_uint(size_t start_index, size_t bit_count) const;
     
     /**
      * @brief Get a signed integer value
      * @param start_index Start bit index
      * @param bit_count Number of bits to read (max 64)
      * @return Signed integer value
      * @throws std::out_of_range if indices are out of range
      * @throws std::invalid_argument if bit_count > 64
      */
     int64_t get_int(size_t start_index, size_t bit_count) const;
     
     /**
      * @brief Append an unsigned integer value
      * @param value Unsigned integer value
      * @param bit_count Number of bits to write (max 64)
      * @throws std::invalid_argument if bit_count > 64
      */
     void append_uint(uint64_t value, size_t bit_count);
     
     /**
      * @brief Append a signed integer value
      * @param value Signed integer value
      * @param bit_count Number of bits to write (max 64)
      * @throws std::invalid_argument if bit_count > 64
      */
     void append_int(int64_t value, size_t bit_count);
     
     /**
      * @brief Get a string value (6-bit ASCII)
      * @param start_index Start bit index
      * @param bit_count Number of bits to read (must be multiple of 6)
      * @return Decoded string
      * @throws std::out_of_range if indices are out of range
      * @throws std::invalid_argument if bit_count is not multiple of 6
      */
     std::string get_string(size_t start_index, size_t bit_count) const;
     
     /**
      * @brief Append a string value (6-bit ASCII)
      * @param value String value
      * @param bit_count Number of bits to write (must be multiple of 6)
      * @throws std::invalid_argument if bit_count is not multiple of 6
      * @throws std::invalid_argument if bit_count is not enough for the string
      */
     void append_string(const std::string& value, size_t bit_count);
     
     /**
      * @brief Convert to 6-bit encoded NMEA payload
      * @return Encoded NMEA payload string
      */
     std::string to_nmea_payload() const;
     
     /**
      * @brief Get a hexadecimal string representation
      * @return Hexadecimal string
      */
     std::string to_hex() const;
     
     /**
      * @brief Get a binary string representation
      * @return Binary string (0s and 1s)
      */
     std::string to_binary() const;
 
 private:
     std::vector<uint8_t> data_; // Storage for bit data (8 bits per byte)
     size_t bit_count_;          // Number of bits currently in use
     
     // Convert 6-bit ASCII character to 8-bit ASCII
     static char decode_ascii(uint8_t value);
     
     // Convert 8-bit ASCII character to 6-bit ASCII
     static uint8_t encode_ascii(char c);
 };
 
 } // namespace aislib
 
 #endif // AISLIB_BIT_VECTOR_H