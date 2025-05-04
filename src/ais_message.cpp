/**
 * @file ais_message.cpp
 * @brief Implementation of AISMessage base class
 */

 #include "aislib/ais_message.h"
 #include "aislib/message_factory.h" // Include MessageFactory
 #include "aislib/nmea_utils.h"
 #include <sstream>
 #include <iomanip>
 #include <stdexcept>
 
 namespace aislib {
 
 std::unique_ptr<AISMessage> AISMessage::from_bits(const BitVector& bits) {
     // Use the MessageFactory to create the appropriate message type
     return MessageFactory::instance().create_message(bits);
 }
 
 std::unique_ptr<AISMessage> AISMessage::from_nmea(const std::string& nmea) {
     // Validate the NMEA sentence
     if (!NMEAUtils::validate_checksum(nmea)) {
         throw std::invalid_argument("Invalid NMEA checksum");
     }
     
     // Parse the NMEA sentence
     std::vector<std::string> fields = NMEAUtils::parse_fields(nmea);
     
     // Check if it's an AIS message (!AIVDM or !AIVDO)
     if (fields.size() < 7 || (fields[0] != "!AIVDM" && fields[0] != "!AIVDO")) {
         throw std::invalid_argument("Not an AIS message");
     }
     
     // Extract the payload
     std::string payload = fields[5];
     
     // Convert to bits
     BitVector bits(payload);
     
     // Create the message from bits
     return from_bits(bits);
 }
 
 std::vector<std::string> AISMessage::to_nmea() const {
     std::vector<std::string> result;
     
     // Convert the message to bits
     BitVector bits;
     to_bits(bits);
     
     // Convert bits to payload
     std::string payload = bits.to_nmea_payload();
     
     // Calculate the number of fill bits
     uint8_t fill_bits = static_cast<uint8_t>((6 - (bits.size() % 6)) % 6);
     
     // Create NMEA sentence
     std::string sentence = NMEAUtils::create_aivdm_sentence(payload, 1, 1, "", 'A', fill_bits);
     result.push_back(sentence);
     
     return result;
 }
 
 std::string AISMessage::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << "\n";
     ss << "MMSI: " << get_mmsi() << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(get_repeat_indicator());
     
     return ss.str();
 }
 
 } // namespace aislib