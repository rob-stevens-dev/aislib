/**
 * @file nmea_utils.cpp
 * @brief Implementation of NMEA utility functions
 */

 #include "aislib/nmea_utils.h"
 #include <sstream>
 #include <iomanip>
 #include <stdexcept>
 
 namespace aislib {
 
    uint8_t NMEAUtils::calculate_checksum(const std::string& sentence) {
        // Checksum is XOR of all characters between '$' or '!' and '*'
        uint8_t checksum = 0;
        
        // Start after the first character if it's $ or !
        size_t start_pos = 0;
        if (!sentence.empty() && (sentence[0] == '$' || sentence[0] == '!')) {
            start_pos = 1;
        }
        
        // End before * if present
        size_t end_pos = sentence.find('*');
        if (end_pos == std::string::npos) {
            end_pos = sentence.length();
        }
        
        // XOR all characters
        for (size_t i = start_pos; i < end_pos; ++i) {
            checksum ^= static_cast<uint8_t>(sentence[i]);
        }
        
        return checksum;
    }
 
    bool NMEAUtils::validate_checksum(const std::string& sentence) {
        // Find the '*' character that precedes the checksum
        size_t asterisk_pos = sentence.find('*');
        if (asterisk_pos == std::string::npos || asterisk_pos + 3 > sentence.length()) {
            return false; // No checksum or not enough characters for checksum
        }
        
        // Extract the checksum from the sentence
        std::string checksum_str = sentence.substr(asterisk_pos + 1, 2);
        uint8_t expected_checksum;
        
        try {
            expected_checksum = static_cast<uint8_t>(std::stoi(checksum_str, nullptr, 16));
        } catch (const std::exception&) {
            return false; // Invalid checksum format
        }
        
        // Calculate the actual checksum from the part of the sentence before the asterisk
        std::string sentence_part = sentence.substr(0, asterisk_pos);
        uint8_t actual_checksum = calculate_checksum(sentence_part);
        
        return actual_checksum == expected_checksum;
    }
 
 std::vector<std::string> NMEAUtils::parse_fields(const std::string& sentence) {
     std::vector<std::string> fields;
     
     // Remove checksum part
     size_t asterisk_pos = sentence.find('*');
     std::string sentence_without_checksum;
     
     if (asterisk_pos != std::string::npos) {
         sentence_without_checksum = sentence.substr(0, asterisk_pos);
     } else {
         sentence_without_checksum = sentence;
     }
     
     // Split by commas
     std::stringstream ss(sentence_without_checksum);
     std::string field;
     
     while (std::getline(ss, field, ',')) {
         fields.push_back(field);
     }
     
     return fields;
 }
 
 std::string NMEAUtils::create_aivdm_sentence(
     const std::string& payload,
     uint8_t fragment_count,
     uint8_t fragment_number,
     const std::string& message_id,
     char channel,
     uint8_t fill_bits
 ) {
     return create_nmea_sentence("!AIVDM", payload, fragment_count, fragment_number, message_id, channel, fill_bits);
 }
 
 std::string NMEAUtils::create_aivdo_sentence(
     const std::string& payload,
     uint8_t fragment_count,
     uint8_t fragment_number,
     const std::string& message_id,
     char channel,
     uint8_t fill_bits
 ) {
     return create_nmea_sentence("!AIVDO", payload, fragment_count, fragment_number, message_id, channel, fill_bits);
 }
 
 std::string NMEAUtils::create_nmea_sentence(
    const std::string& talker_id,
    const std::string& payload,
    uint8_t fragment_count,
    uint8_t fragment_number,
    const std::string& message_id,
    char channel,
    uint8_t fill_bits
) {
    // Validate inputs
    if (fragment_count < 1 || fragment_number < 1 || fragment_number > fragment_count) {
        throw std::invalid_argument("Invalid fragment count or number");
    }
    
    if (channel != 'A' && channel != 'B') {
        throw std::invalid_argument("Invalid channel, must be 'A' or 'B'");
    }
    
    if (fill_bits > 5) {
        throw std::invalid_argument("Invalid fill bits, must be 0-5");
    }
    
    // Create sentence
    std::stringstream ss;
    
    // Use numeric characters for fragment counts and fill bits, not actual values
    ss << talker_id << ","
       << static_cast<int>(fragment_count) << ","
       << static_cast<int>(fragment_number) << ","
       << (message_id.empty() ? "" : message_id) << ","
       << channel << ","
       << payload << ","
       << static_cast<int>(fill_bits);
    
    // Get the sentence so far
    std::string sentence_base = ss.str();
    
    // Calculate checksum
    uint8_t checksum = calculate_checksum(sentence_base);
    
    // Append checksum in the correct format
    std::stringstream ss_final;
    ss_final << sentence_base << "*" 
             << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
             << static_cast<int>(checksum);
    
    return ss_final.str();
}
 
 } // namespace aislib