/**
 * @file ais_parser.cpp
 * @brief Implementation of AISParser class with enhanced multi-part message handling
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
     
     // Extract message ID, channel, payload, and fill bits
     std::string message_id = fields[3];
     char channel = fields[4][0];
     std::string payload = fields[5];
     uint8_t fill_bits = 0;
     
     try {
         fill_bits = static_cast<uint8_t>(std::stoi(fields[6]));
     } catch (const std::exception&) {
         set_error(ParseError::ErrorType::INVALID_FRAGMENT_INFO, "Invalid fill bits");
         return nullptr;
     }
     
     // Handle single-part messages directly
     if (fragment_count == 1) {
         try {
             // Convert payload to bits
             BitVector bits(payload);
             
             // Adjust for fill bits
             if (fill_bits > 0 && fill_bits <= 5) {
                 // Create a new BitVector excluding fill bits
                 BitVector adjusted_bits;
                 for (size_t i = 0; i < bits.size() - fill_bits; i++) {
                     adjusted_bits.append_bit(bits.get_bit(i));
                 }
                 return AISMessage::from_bits(adjusted_bits);
             }
             
             // Create message from bits
             return AISMessage::from_bits(bits);
         } catch (const std::exception& e) {
             set_error(ParseError::ErrorType::INVALID_PAYLOAD, e.what());
             return nullptr;
         }
     } else {
         // This is a multi-part message
         try {
             // Add fragment to MultipartMessageManager
             auto result = multipart_manager_.add_fragment(
                 fragment_number, fragment_count, message_id, channel, payload, fill_bits);
             
             // Check if all fragments are received
             if (result) {
                 // Message is complete, create message from combined bits
                 return AISMessage::from_bits(*result);
             }
             
             // Not all fragments received yet, return nullptr without setting an error
             return nullptr;
         } catch (const std::exception& e) {
             set_error(ParseError::ErrorType::OTHER, e.what());
             return nullptr;
         }
     }
 }
 
 std::unique_ptr<AISMessage> AISParser::add_fragment(const std::string& nmea_sentence) {
     // This is a convenience wrapper around parse() that makes the intent clearer
     return parse(nmea_sentence);
 }
 
 void AISParser::cleanup_expired_fragments() {
     // Delegate to the multipart manager
     multipart_manager_.cleanup_expired();
 }
 
 size_t AISParser::get_incomplete_message_count() const {
     return multipart_manager_.get_incomplete_count();
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