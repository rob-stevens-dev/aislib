/**
 * @file ais_parser.cpp
 * @brief Implementation of AISParser class
 */

 #include "aislib/ais_parser.h"
 #include "aislib/nmea_utils.h"
 #include <stdexcept>
 
 namespace aislib {
 
 AISParser::AISParser(const ParserConfig& config)
     : multipart_manager_(config.message_timeout, config.max_incomplete_messages) {
     clear_error();
 }
 
 std::unique_ptr<AISMessage> AISParser::parse(const std::string& nmea_sentence) {
     // Clear previous error
     clear_error();
     
     // Validate checksum
     if (!NMEAUtils::validate_checksum(nmea_sentence)) {
         set_error(ParseError::ErrorType::INVALID_CHECKSUM, "Invalid NMEA checksum");
         return nullptr;
     }
     
     // Parse fields
     std::vector<std::string> fields = NMEAUtils::parse_fields(nmea_sentence);
     
     // Check if it's an AIS message (!AIVDM or !AIVDO)
     if (fields.size() < 7 || (fields[0] != "!AIVDM" && fields[0] != "!AIVDO")) {
         set_error(ParseError::ErrorType::INVALID_SENTENCE_FORMAT, "Not an AIS message");
         return nullptr;
     }
     
     // Extract fragment information
     uint8_t fragment_count = 0;
     uint8_t fragment_number = 0;
     
     try {
         fragment_count = static_cast<uint8_t>(std::stoi(fields[1]));
         fragment_number = static_cast<uint8_t>(std::stoi(fields[2]));
     } catch (const std::exception&) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fragment information");
         return nullptr;
     }
     
     // Check fragment numbers
     if (fragment_count < 1 || fragment_number < 1 || fragment_number > fragment_count) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fragment count or number");
         return nullptr;
     }
     
     // Extract message ID
     std::string message_id = fields[3];
     
     // Extract channel
     char channel = fields[4][0];
     if (channel != 'A' && channel != 'B') {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid channel");
         return nullptr;
     }
     
     // Extract payload
     std::string payload = fields[5];
     
     // Extract fill bits
     uint8_t fill_bits = 0;
     try {
         fill_bits = static_cast<uint8_t>(std::stoi(fields[6]));
     } catch (const std::exception&) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fill bits");
         return nullptr;
     }
     
     // Check if single fragment
     if (fragment_count == 1) {
         try {
             // Convert payload to bits
             BitVector bits(payload);
             
             // Adjust for fill bits
             if (fill_bits > 0 && fill_bits <= 5) {
                 bits = BitVector(payload.substr(0, payload.length() - 1)); // Remove last character
                 
                 // Add the bits from the last character, excluding fill bits
                 uint8_t last_char_value = 0;
                 char last_char = payload.back();
                 
                 if (last_char >= '0' && last_char <= 'W') {
                     last_char_value = static_cast<uint8_t>(last_char - '0');
                 } else if (last_char >= '`' && last_char <= 'w') {
                     last_char_value = static_cast<uint8_t>(last_char - '`' + 40);
                 }
                 
                 // Add the valid bits from the last character
                 for (int i = 5; i >= static_cast<int>(fill_bits); --i) {
                     bits.append_bit((last_char_value & (1 << i)) != 0);
                 }
             }
             
             // Create message from bits
             return AISMessage::from_bits(bits);
         } catch (const std::exception& e) {
             set_error(ParseError::ErrorType::INVALID_PAYLOAD, e.what());
             return nullptr;
         }
     } else {
         // Multipart message
         return add_fragment(nmea_sentence);
     }
 }
 
 std::unique_ptr<AISMessage> AISParser::add_fragment(const std::string& nmea_sentence) {
     // Clear previous error
     clear_error();
     
     // Validate checksum
     if (!NMEAUtils::validate_checksum(nmea_sentence)) {
         set_error(ParseError::ErrorType::INVALID_CHECKSUM, "Invalid NMEA checksum");
         return nullptr;
     }
     
     // Parse fields
     std::vector<std::string> fields = NMEAUtils::parse_fields(nmea_sentence);
     
     // Check if it's an AIS message (!AIVDM or !AIVDO)
     if (fields.size() < 7 || (fields[0] != "!AIVDM" && fields[0] != "!AIVDO")) {
         set_error(ParseError::ErrorType::INVALID_SENTENCE_FORMAT, "Not an AIS message");
         return nullptr;
     }
     
     // Extract fragment information
     uint8_t fragment_count = 0;
     uint8_t fragment_number = 0;
     
     try {
         fragment_count = static_cast<uint8_t>(std::stoi(fields[1]));
         fragment_number = static_cast<uint8_t>(std::stoi(fields[2]));
     } catch (const std::exception&) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fragment information");
         return nullptr;
     }
     
     // Extract message ID
     std::string message_id = fields[3];
     
     // Extract channel
     char channel = fields[4][0];
     
     // Extract payload
     std::string payload = fields[5];
     
     // Extract fill bits
     uint8_t fill_bits = 0;
     try {
         fill_bits = static_cast<uint8_t>(std::stoi(fields[6]));
     } catch (const std::exception&) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fill bits");
         return nullptr;
     }
     
     try {
         // Add fragment to multipart manager
         auto combined = multipart_manager_.add_fragment(
             fragment_number, fragment_count, message_id, channel, payload, fill_bits);
         
         // Check if all fragments are received
         if (combined) {
             // Create message from combined bits
             return AISMessage::from_bits(*combined);
         }
         
         // Not all fragments received yet
         return nullptr;
     } catch (const std::exception& e) {
         set_error(ParseError::ErrorType::OTHER, e.what());
         return nullptr;
     }
 }
 
 AISParser::ParseError AISParser::get_last_error() const {
     return last_error_;
 }
 
 void AISParser::set_message_timeout(std::chrono::seconds timeout) {
     multipart_manager_.set_timeout(timeout);
 }
 
 void AISParser::set_max_incomplete_messages(size_t max_messages) {
     multipart_manager_.set_max_messages(max_messages);
 }
 
 void AISParser::clear_incomplete_messages() {
     multipart_manager_.clear();
 }
 
 void AISParser::set_error(ParseError::ErrorType type, const std::string& message) {
     last_error_.type = type;
     last_error_.message = message;
 }
 
 void AISParser::clear_error() {
     last_error_.type = ParseError::ErrorType::NONE;
     last_error_.message.clear();
 }
 
 std::optional<std::unique_ptr<AISMessage>> parse_ais(const std::string& nmea_sentence) {
     AISParser parser;
     auto message = parser.parse(nmea_sentence);
     
     if (message) {
         return message;
     }
     
     return std::nullopt;
 }
 
 } // namespace aislib