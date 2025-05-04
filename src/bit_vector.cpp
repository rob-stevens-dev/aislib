/**
 * @file bit_vector.cpp
 * @brief Implementation of BitVector class
 */

 #include "aislib/bit_vector.h"
 #include <sstream>
 #include <iomanip>
 
 namespace aislib {
 
 BitVector::BitVector() : bit_count_(0) {
 }
 
 BitVector::BitVector(size_t capacity) : bit_count_(0) {
     reserve(capacity);
 }
 
 BitVector::BitVector(const std::string& payload) : bit_count_(0) {
     // Each character in the payload represents 6 bits
     reserve(payload.length() * 6);
     
     for (char c : payload) {
         // Convert AIS/NMEA ASCII to 6-bit value
         uint8_t value = 0;
         
         if (c >= '0' && c <= 'W') {
             value = static_cast<uint8_t>(c - '0');
         } else if (c >= '`' && c <= 'w') {
             value = static_cast<uint8_t>(c - '`' + 40);
         } else {
             throw std::invalid_argument("Invalid character in NMEA payload");
         }
         
         // Append the 6 bits
         for (int i = 5; i >= 0; --i) {
             append_bit((value & (1 << i)) != 0);
         }
     }
 }
 
 size_t BitVector::size() const {
     return bit_count_;
 }
 
 size_t BitVector::capacity() const {
     return data_.size() * 8;
 }
 
 void BitVector::reserve(size_t capacity) {
     // Calculate required bytes (rounded up)
     size_t required_bytes = (capacity + 7) / 8;
     data_.reserve(required_bytes);
     
     // Ensure we have at least the required number of bytes
     while (data_.size() < required_bytes) {
         data_.push_back(0);
     }
 }
 
 void BitVector::clear() {
     data_.clear();
     bit_count_ = 0;
 }
 
 bool BitVector::get_bit(size_t index) const {
     if (index >= bit_count_) {
         throw std::out_of_range("Bit index out of range");
     }
     
     size_t byte_index = index / 8;
     size_t bit_offset = index % 8;
     
     return (data_[byte_index] & (1 << (7 - bit_offset))) != 0;
 }
 
 void BitVector::set_bit(size_t index, bool value) {
     if (index >= bit_count_) {
         throw std::out_of_range("Bit index out of range");
     }
     
     size_t byte_index = index / 8;
     size_t bit_offset = index % 8;
     
     if (value) {
         data_[byte_index] |= (1 << (7 - bit_offset));
     } else {
         data_[byte_index] &= ~(1 << (7 - bit_offset));
     }
 }
 
 void BitVector::append_bit(bool bit) {
     // Ensure we have enough capacity
     if (bit_count_ >= capacity()) {
         // Add a new byte
         data_.push_back(0);
     }
     
     size_t byte_index = bit_count_ / 8;
     size_t bit_offset = bit_count_ % 8;
     
     if (bit) {
         data_[byte_index] |= (1 << (7 - bit_offset));
     }
     
     ++bit_count_;
 }
 
 uint64_t BitVector::get_uint(size_t start_index, size_t bit_count) const {
     if (bit_count > 64) {
         throw std::invalid_argument("Cannot read more than 64 bits as uint");
     }
     
     if (start_index + bit_count > bit_count_) {
         throw std::out_of_range("Bit range out of bounds");
     }
     
     uint64_t result = 0;
     for (size_t i = 0; i < bit_count; ++i) {
         result <<= 1;
         if (get_bit(start_index + i)) {
             result |= 1;
         }
     }
     
     return result;
 }
 
 int64_t BitVector::get_int(size_t start_index, size_t bit_count) const {
     if (bit_count > 64) {
         throw std::invalid_argument("Cannot read more than 64 bits as int");
     }
     
     uint64_t value = get_uint(start_index, bit_count);
     
     // Check if the value is negative (most significant bit is set)
     bool is_negative = (bit_count > 0 && (value & (static_cast<uint64_t>(1) << (bit_count - 1))));
     
     if (is_negative) {
         // Apply sign extension
         uint64_t mask = ~((static_cast<uint64_t>(1) << bit_count) - 1);
         value |= mask;
     }
     
     return static_cast<int64_t>(value);
 }
 
 void BitVector::append_uint(uint64_t value, size_t bit_count) {
     if (bit_count > 64) {
         throw std::invalid_argument("Cannot write more than 64 bits from uint");
     }
     
     // Ensure we have enough capacity
     reserve(bit_count_ + bit_count);
     
     // Append bits from most significant to least significant
     for (size_t i = 0; i < bit_count; ++i) {
         size_t bit_position = bit_count - 1 - i;
         bool bit = (value & (static_cast<uint64_t>(1) << bit_position)) != 0;
         append_bit(bit);
     }
 }
 
 void BitVector::append_int(int64_t value, size_t bit_count) {
     // For signed values, we just cast to unsigned and append
     append_uint(static_cast<uint64_t>(value), bit_count);
 }
 
 std::string BitVector::get_string(size_t start_index, size_t bit_count) const {
    if (bit_count % 6 != 0) {
        throw std::invalid_argument("String bit count must be multiple of 6");
    }
    
    if (start_index + bit_count > bit_count_) {
        throw std::out_of_range("Bit range out of bounds");
    }
    
    std::string result;
    size_t char_count = bit_count / 6;
    
    // Process all characters
    for (size_t i = 0; i < char_count; ++i) {
        uint8_t value = static_cast<uint8_t>(get_uint(start_index + i * 6, 6));
        
        // For bit counts of 18, we need special handling for AIS text
        // This is due to the way the 6-bit ASCII is encoded in AIS
        if (bit_count == 18 && i < 3) {
            char c = decode_ascii(value);
            if (c != '@') { // Skip null/@
                result += c;
            }
        } else {
            char c = decode_ascii(value);
            if (c != '@') { // Skip null/@
                result += c;
            }
        }
    }
    
    return result;
}
 
void BitVector::append_string(const std::string& value, size_t bit_count) {
    if (bit_count % 6 != 0) {
        throw std::invalid_argument("String bit count must be multiple of 6");
    }
    
    size_t max_chars = bit_count / 6;
    if (value.length() > max_chars) {
        throw std::invalid_argument("String too long for specified bit count");
    }
    
    // Ensure we have enough capacity
    reserve(bit_count_ + bit_count);
    
    // Append the string characters
    for (size_t i = 0; i < max_chars; ++i) {
        uint8_t char_value;
        
        if (i < value.length()) {
            char_value = encode_ascii(value[i]);
        } else {
            // Pad with space (32 in 6-bit ASCII)
            char_value = 32;  // Space in AIS ASCII is 32, not 0
        }
        
        append_uint(char_value, 6);
    }
}
 
 std::string BitVector::to_nmea_payload() const {
     std::string result;
     
     // Process 6 bits at a time
     for (size_t i = 0; i < bit_count_; i += 6) {
         size_t bits_remaining = std::min<size_t>(6, bit_count_ - i);
         
         // If less than 6 bits remaining, pad with zeros
         uint8_t value = static_cast<uint8_t>(get_uint(i, bits_remaining) << (6 - bits_remaining));
         
         // Convert 6-bit value to NMEA ASCII
         if (value < 40) {
             result += static_cast<char>(value + '0');
         } else {
             result += static_cast<char>(value - 40 + '`');
         }
     }
     
     return result;
 }
 
 std::string BitVector::to_hex() const {
     std::stringstream ss;
     
     for (size_t i = 0; i < (bit_count_ + 7) / 8; ++i) {
         ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(data_[i]);
     }
     
     return ss.str();
 }
 
 std::string BitVector::to_binary() const {
     std::string result;
     result.reserve(bit_count_);
     
     for (size_t i = 0; i < bit_count_; ++i) {
         result += get_bit(i) ? '1' : '0';
     }
     
     return result;
 }
 
 char BitVector::decode_ascii(uint8_t value) {
    // AIS 6-bit ASCII to standard ASCII
    // According to the AIS standard:
    // - Value 0 = '@' (special/null character)
    // - Values 1-26 = 'A'-'Z' 
    // - Values 27-31 = '[', '\', ']', '^', '_'
    // - Value 32 = ' ' (space)
    // - Values 33-63 = '!'-'_'
    
    if (value == 0) return '@';
    if (value >= 1 && value <= 31) return value + 64;  // 'A'-'Z' and special chars
    if (value >= 32 && value <= 63) return value;  // ' ' and '!'-'?'
    
    return '?'; // Invalid value
}

uint8_t BitVector::encode_ascii(char c) {
    // Standard ASCII to AIS 6-bit ASCII
    if (c == '@') return 0;
    if (c >= 'A' && c <= 'Z') return c - 64;  // 1-26
    if (c >= '[' && c <= '_') return c - 64;  // 27-31
    if (c >= ' ' && c <= '?') return c;       // 32-63
    
    // Default to @ (null) for invalid characters
    return 0;
}
 
 } // namespace aislib