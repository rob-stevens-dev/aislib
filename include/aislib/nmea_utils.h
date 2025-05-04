/**
 * @file nmea_utils.h
 * @brief NMEA sentence utilities
 * 
 * This file defines utility functions for working with NMEA sentences.
 */

 #ifndef AISLIB_NMEA_UTILS_H
 #define AISLIB_NMEA_UTILS_H
 
 #include <string>
 #include <vector>
 #include <cstdint>
 
 namespace aislib {
 
 /**
  * @class NMEAUtils
  * @brief Utility functions for NMEA sentence processing
  */
 class NMEAUtils {
 public:
     /**
      * @brief Calculate NMEA checksum
      * @param sentence NMEA sentence (without checksum)
      * @return Calculated checksum
      */
     static uint8_t calculate_checksum(const std::string& sentence);
     
     /**
      * @brief Validate NMEA checksum
      * @param sentence Complete NMEA sentence (with checksum)
      * @return true if checksum is valid, false otherwise
      */
     static bool validate_checksum(const std::string& sentence);
     
     /**
      * @brief Parse NMEA fields
      * @param sentence NMEA sentence
      * @return Vector of NMEA fields
      */
     static std::vector<std::string> parse_fields(const std::string& sentence);
     
     /**
      * @brief Create an AIVDM sentence
      * @param payload AIS message payload
      * @param fragment_count Total number of fragments
      * @param fragment_number Fragment number (1-based)
      * @param message_id Message ID for multi-sentence messages
      * @param channel AIS channel (A or B)
      * @param fill_bits Number of fill bits
      * @return AIVDM sentence
      */
     static std::string create_aivdm_sentence(
         const std::string& payload,
         uint8_t fragment_count,
         uint8_t fragment_number,
         const std::string& message_id,
         char channel,
         uint8_t fill_bits
     );
     
     /**
      * @brief Create an AIVDO sentence
      * @param payload AIS message payload
      * @param fragment_count Total number of fragments
      * @param fragment_number Fragment number (1-based)
      * @param message_id Message ID for multi-sentence messages
      * @param channel AIS channel (A or B)
      * @param fill_bits Number of fill bits
      * @return AIVDO sentence
      */
     static std::string create_aivdo_sentence(
         const std::string& payload,
         uint8_t fragment_count,
         uint8_t fragment_number,
         const std::string& message_id,
         char channel,
         uint8_t fill_bits
     );
     
 private:
     /**
      * @brief Helper function to create AIVDM/AIVDO sentences
      * @param talker_id Talker ID (AIVDM or AIVDO)
      * @param payload AIS message payload
      * @param fragment_count Total number of fragments
      * @param fragment_number Fragment number (1-based)
      * @param message_id Message ID for multi-sentence messages
      * @param channel AIS channel (A or B)
      * @param fill_bits Number of fill bits
      * @return NMEA sentence
      */
     static std::string create_nmea_sentence(
         const std::string& talker_id,
         const std::string& payload,
         uint8_t fragment_count,
         uint8_t fragment_number,
         const std::string& message_id,
         char channel,
         uint8_t fill_bits
     );
 };
 
 } // namespace aislib
 
 #endif // AISLIB_NMEA_UTILS_H