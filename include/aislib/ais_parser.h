/**
 * @file ais_parser.h
 * @brief AIS Parser with enhanced multi-part message handling
 * 
 * This file defines the AISParser class, which handles parsing of
 * NMEA sentences containing AIS messages, including proper handling
 * of multi-part messages.
 */

 #ifndef AISLIB_AIS_PARSER_H
 #define AISLIB_AIS_PARSER_H
 
 #include "ais_message.h"
 #include "multipart_message_manager.h"
 #include <string>
 #include <optional>
 #include <memory>
 #include <chrono>
 
 namespace aislib {
 
 /**
  * @class AISParser
  * @brief Parser for AIS messages with enhanced multi-part message support
  */
 class AISParser {
 public:
     /**
      * @struct ParseError
      * @brief Structure for parse error information
      */
     struct ParseError {
         enum class ErrorType {
             NONE,
             INVALID_CHECKSUM,
             INVALID_SENTENCE_FORMAT,
             INVALID_FRAGMENT_INFO,
             UNSUPPORTED_MESSAGE_TYPE,
             INVALID_PAYLOAD,
             OTHER
         };
         
         ErrorType type;
         std::string message;
     };
     
     /**
      * @struct ParserConfig
      * @brief Configuration for AIS parser
      */
     struct ParserConfig {
         std::chrono::seconds message_timeout;
         size_t max_incomplete_messages;
         
         // Default constructor
         ParserConfig() : 
             message_timeout(std::chrono::seconds(60)), 
             max_incomplete_messages(100) {}
     };
     
     /**
      * @brief Constructor
      * @param config Parser configuration
      */
     explicit AISParser(const ParserConfig& config = ParserConfig());
     
     /**
      * @brief Parse a complete NMEA sentence
      * @param nmea_sentence NMEA sentence
      * @return Smart pointer to AIS message if successful, nullptr otherwise
      * 
      * This method will automatically handle single-part messages and multi-part
      * message fragments. For multi-part messages, it returns a message only when
      * all fragments have been received.
      */
     std::unique_ptr<AISMessage> parse(const std::string& nmea_sentence);
     
     /**
      * @brief Add a fragment of a multipart message
      * @param nmea_sentence NMEA sentence containing the fragment
      * @return Smart pointer to AIS message if all fragments are received, nullptr otherwise
      * 
      * This is a convenience wrapper around parse() that makes the intent clearer.
      */
     std::unique_ptr<AISMessage> add_fragment(const std::string& nmea_sentence);
     
     /**
      * @brief Clean up expired message fragments
      * 
      * This method should be called periodically to remove fragments that have exceeded
      * the configured timeout.
      */
     void cleanup_expired_fragments();
     
     /**
      * @brief Get the number of incomplete multi-part messages
      * @return Count of incomplete messages
      */
     size_t get_incomplete_message_count() const;
     
     /**
      * @brief Get the last parse error
      * @return Last parse error
      */
     ParseError get_last_error() const;
     
     /**
      * @brief Set the message timeout
      * @param timeout Timeout for incomplete messages
      */
     void set_message_timeout(std::chrono::seconds timeout);
     
     /**
      * @brief Set the maximum number of incomplete messages
      * @param max_messages Maximum number of incomplete messages to track
      */
     void set_max_incomplete_messages(size_t max_messages);
     
     /**
      * @brief Clear all incomplete messages
      */
     void clear_incomplete_messages();
     
 private:
     // Multipart message manager
     MultipartMessageManager multipart_manager_;
     
     // Last parse error
     ParseError last_error_;
     
     // Set the last error
     void set_error(ParseError::ErrorType type, const std::string& message);
     
     // Clear the last error
     void clear_error();
 };
 
 /**
  * @brief Simplified API for parsing AIS messages
  * @param nmea_sentence NMEA sentence
  * @return Optional smart pointer to AIS message
  * 
  * This function provides a simple one-shot parsing of AIS messages.
  * Note that it does not support multi-part messages that span multiple calls.
  */
 std::optional<std::unique_ptr<AISMessage>> parse_ais(const std::string& nmea_sentence);
 
 } // namespace aislib
 
 #endif // AISLIB_AIS_PARSER_H