/**
 * @file position_report_class_a.cpp
 * @brief Implementation of PositionReportClassA class
 */

 #include "aislib/position_report_class_a.h"
 #include <sstream>
 #include <iomanip>
 #include <cmath>
 
 namespace aislib {
 
 PositionReportClassA::PositionReportClassA(
     uint8_t message_type,
     uint32_t mmsi,
     uint8_t repeat_indicator,
     NavigationStatus nav_status
 ) : message_type_(message_type),
     mmsi_(mmsi),
     repeat_indicator_(repeat_indicator),
     nav_status_(nav_status),
     rot_(-128),
     sog_(1023),
     position_accuracy_(false),
     longitude_(0x6791AC0),  // 181 degrees = not available
     latitude_(0x3412140),   // 91 degrees = not available
     cog_(3600),             // 360 degrees = not available
     true_heading_(511),     // 511 = not available
     timestamp_(60),         // 60 = not available
     special_maneuver_(0),   // 0 = not available
     spare_(0),
     raim_flag_(false),
     radio_status_(0) {
     // Validate message type
     if (message_type_ != 1 && message_type_ != 2 && message_type_ != 3) {
         throw std::invalid_argument("Invalid message type for Position Report Class A");
     }
 }
 
 PositionReportClassA::PositionReportClassA(const BitVector& bits) {
     // Check if we have enough bits for a Position Report Class A message
     if (bits.size() < 168) {
         throw std::invalid_argument("Bit vector too small for a Position Report Class A message");
     }
 
     // Extract message type
     message_type_ = static_cast<uint8_t>(bits.get_uint(0, 6));
     if (message_type_ != 1 && message_type_ != 2 && message_type_ != 3) {
         throw std::invalid_argument("Invalid message type for Position Report Class A");
     }
 
     // Extract fields
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     nav_status_ = static_cast<NavigationStatus>(bits.get_uint(38, 4));
     rot_ = static_cast<int8_t>(bits.get_int(42, 8));
     sog_ = static_cast<uint16_t>(bits.get_uint(50, 10));
     position_accuracy_ = bits.get_bit(60);
     longitude_ = static_cast<int32_t>(bits.get_int(61, 28));
     latitude_ = static_cast<int32_t>(bits.get_int(89, 27));
     cog_ = static_cast<uint16_t>(bits.get_uint(116, 12));
     true_heading_ = static_cast<uint16_t>(bits.get_uint(128, 9));
     timestamp_ = static_cast<uint8_t>(bits.get_uint(137, 6));
     special_maneuver_ = static_cast<uint8_t>(bits.get_uint(143, 2));
     spare_ = static_cast<uint8_t>(bits.get_uint(145, 3));
     raim_flag_ = bits.get_bit(148);
     radio_status_ = static_cast<uint32_t>(bits.get_uint(149, 19));
 }
 
 uint8_t PositionReportClassA::get_message_type() const {
     return message_type_;
 }
 
 uint32_t PositionReportClassA::get_mmsi() const {
     return mmsi_;
 }
 
 uint8_t PositionReportClassA::get_repeat_indicator() const {
     return repeat_indicator_;
 }
 
 PositionReportClassA::NavigationStatus PositionReportClassA::get_navigation_status() const {
     return nav_status_;
 }
 
 int8_t PositionReportClassA::get_rate_of_turn_raw() const {
     return rot_;
 }
 
 float PositionReportClassA::get_rate_of_turn() const {
     if (rot_ == -128) {
         return std::numeric_limits<float>::quiet_NaN(); // Not available
     }
     
     if (rot_ == 127 || rot_ == -127) {
         return std::numeric_limits<float>::infinity() * (rot_ > 0 ? 1.0f : -1.0f); // Turning right/left at > 5 deg/30s
     }
     
     // Convert ROT indicator to actual degrees per minute
     // ROT = 4.733 * SQRT(ROTind) degrees/min
     // Where ROTind is the raw value sent in the message
     if (rot_ == 0) {
         return 0.0f;
     }
     
     float rot_deg_min = 4.733f * std::sqrt(std::abs(rot_)) * (rot_ > 0 ? 1.0f : -1.0f);
     return rot_deg_min;
 }
 
 float PositionReportClassA::get_speed_over_ground() const {
     if (sog_ == 1023) {
         return std::numeric_limits<float>::quiet_NaN(); // Not available
     }
     
     if (sog_ == 1022) {
         return 102.2f; // >= 102.2 knots
     }
     
     return sog_ / 10.0f;
 }
 
 bool PositionReportClassA::get_position_accuracy() const {
     return position_accuracy_;
 }
 
 double PositionReportClassA::get_longitude() const {
     const int32_t longitude_not_available = 0x6791AC0; // 181 degrees in 1/10000 minute
     if (longitude_ == longitude_not_available) {
         return 181.0; // Not available
     }
     
     // Convert from 1/10000 minute to degrees
     // 1/10000 minute = 1/600000 degree
     return longitude_ / 600000.0;
 }
 
 double PositionReportClassA::get_latitude() const {
     const int32_t latitude_not_available = 0x3412140; // 91 degrees in 1/10000 minute
     if (latitude_ == latitude_not_available) {
         return 91.0; // Not available
     }
     
     // Convert from 1/10000 minute to degrees
     // 1/10000 minute = 1/600000 degree
     return latitude_ / 600000.0;
 }
 
 float PositionReportClassA::get_course_over_ground() const {
     if (cog_ == 3600) {
         return std::numeric_limits<float>::quiet_NaN(); // Not available
     }
     
     return cog_ / 10.0f;
 }
 
 uint16_t PositionReportClassA::get_true_heading() const {
     return true_heading_;
 }
 
 uint8_t PositionReportClassA::get_timestamp() const {
     return timestamp_;
 }
 
 uint8_t PositionReportClassA::get_special_maneuver_indicator() const {
     return special_maneuver_;
 }
 
 bool PositionReportClassA::get_raim_flag() const {
     return raim_flag_;
 }
 
 void PositionReportClassA::set_navigation_status(NavigationStatus status) {
     nav_status_ = status;
 }
 
 void PositionReportClassA::set_rate_of_turn_raw(int8_t rot) {
     rot_ = rot;
 }
 
 void PositionReportClassA::set_rate_of_turn(float rot_deg_min) {
     if (std::isnan(rot_deg_min)) {
         rot_ = -128; // Not available
         return;
     }
     
     if (rot_deg_min > 708.0f) {
         rot_ = 127; // Turning right at more than 5 deg/30s
         return;
     }
     
     if (rot_deg_min < -708.0f) {
         rot_ = -127; // Turning left at more than 5 deg/30s
         return;
     }
     
     if (rot_deg_min == 0.0f) {
         rot_ = 0;
         return;
     }
     
     // Convert degrees per minute to ROT indicator
     // ROTind = (ROT / 4.733)^2 * sign(ROT)
     float rot_indicator = std::pow(std::abs(rot_deg_min) / 4.733f, 2.0f);
     
     // Apply sign and round to nearest integer
     rot_ = static_cast<int8_t>(std::round(rot_indicator) * (rot_deg_min > 0 ? 1 : -1));
     
     // Clamp to valid range
     if (rot_ > 126) rot_ = 126;
     if (rot_ < -126) rot_ = -126;
 }
 
 void PositionReportClassA::set_speed_over_ground(float sog) {
     if (std::isnan(sog)) {
         sog_ = 1023; // Not available
         return;
     }
     
     if (sog >= 102.2f) {
         sog_ = 1022; // >= 102.2 knots
         return;
     }
     
     if (sog < 0.0f) {
         sog_ = 0; // Minimum is 0
         return;
     }
     
     // Convert knots to 1/10 knot resolution
     sog_ = static_cast<uint16_t>(std::round(sog * 10.0f));
     
     // Clamp to valid range
     if (sog_ > 1022) sog_ = 1022;
 }
 
 void PositionReportClassA::set_position_accuracy(bool accuracy) {
     position_accuracy_ = accuracy;
 }
 
 void PositionReportClassA::set_longitude(double longitude) {
     if (longitude > 180.0 || longitude < -180.0) {
         longitude_ = 0x6791AC0; // 181 degrees = not available
         return;
     }
     
     // Convert from degrees to 1/10000 minute
     // 1 degree = 60 minutes, 1 minute = 10000 units
     longitude_ = static_cast<int32_t>(std::round(longitude * 600000.0));
 }
 
 void PositionReportClassA::set_latitude(double latitude) {
     if (latitude > 90.0 || latitude < -90.0) {
         latitude_ = 0x3412140; // 91 degrees = not available
         return;
     }
     
     // Convert from degrees to 1/10000 minute
     // 1 degree = 60 minutes, 1 minute = 10000 units
     latitude_ = static_cast<int32_t>(std::round(latitude * 600000.0));
 }
 
 void PositionReportClassA::set_course_over_ground(float cog) {
     if (std::isnan(cog)) {
         cog_ = 3600; // Not available
         return;
     }
     
     if (cog < 0.0f || cog >= 360.0f) {
         // Normalize to 0-359.9 range
         cog = std::fmod(cog + 360.0f, 360.0f);
         
         // Special case for 360.0
         if (std::abs(cog - 360.0f) < 0.05f) {
             cog = 0.0f;
         }
     }
     
     // Convert degrees to 1/10 degree resolution
     cog_ = static_cast<uint16_t>(std::round(cog * 10.0f));
     
     // Clamp to valid range
     if (cog_ >= 3600) cog_ = 3599;
 }
 
 void PositionReportClassA::set_true_heading(uint16_t heading) {
     if (heading >= 360 && heading != 511) {
         heading = 511; // Not available
     }
     
     true_heading_ = heading;
 }
 
 void PositionReportClassA::set_timestamp(uint8_t timestamp) {
     if (timestamp > 63) {
         timestamp = 60; // Default to 'not available'
     }
     
     timestamp_ = timestamp;
 }
 
 void PositionReportClassA::set_special_maneuver_indicator(uint8_t indicator) {
     if (indicator > 2) {
         indicator = 0; // Default to 'not available'
     }
     
     special_maneuver_ = indicator;
 }
 
 void PositionReportClassA::set_raim_flag(bool raim) {
     raim_flag_ = raim;
 }
 
 void PositionReportClassA::to_bits(BitVector& bits) const {
     // Clear the bit vector
     bits.clear();
     
     // Message Type (6 bits)
     bits.append_uint(message_type_, 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(repeat_indicator_, 2);
     
     // MMSI (30 bits)
     bits.append_uint(mmsi_, 30);
     
     // Navigation Status (4 bits)
     bits.append_uint(static_cast<uint8_t>(nav_status_), 4);
     
     // Rate of Turn (8 bits)
     bits.append_int(rot_, 8);
     
     // Speed Over Ground (10 bits)
     bits.append_uint(sog_, 10);
     
     // Position Accuracy (1 bit)
     bits.append_bit(position_accuracy_);
     
     // Longitude (28 bits)
     bits.append_int(longitude_, 28);
     
     // Latitude (27 bits)
     bits.append_int(latitude_, 27);
     
     // Course Over Ground (12 bits)
     bits.append_uint(cog_, 12);
     
     // True Heading (9 bits)
     bits.append_uint(true_heading_, 9);
     
     // Timestamp (6 bits)
     bits.append_uint(timestamp_, 6);
     
     // Special Maneuver Indicator (2 bits)
     bits.append_uint(special_maneuver_, 2);
     
     // Spare (3 bits)
     bits.append_uint(spare_, 3);
     
     // RAIM Flag (1 bit)
     bits.append_bit(raim_flag_);
     
     // Radio Status (19 bits)
     bits.append_uint(radio_status_, 19);
 }
 
 std::string PositionReportClassA::to_string() const {
     std::stringstream ss;
     
     ss << AISMessage::to_string() << "\n";
     ss << "Navigation Status: " << static_cast<int>(static_cast<uint8_t>(nav_status_)) << "\n";
     
     float rot = get_rate_of_turn();
     if (std::isnan(rot)) {
         ss << "Rate of Turn: Not available\n";
     } else if (std::isinf(rot)) {
         ss << "Rate of Turn: " << (rot > 0 ? "Turning right at more than 5 deg/30s" : "Turning left at more than 5 deg/30s") << "\n";
     } else {
         ss << "Rate of Turn: " << rot << " deg/min\n";
     }
     
     float sog = get_speed_over_ground();
     if (std::isnan(sog)) {
         ss << "Speed Over Ground: Not available\n";
     } else {
         ss << "Speed Over Ground: " << sog << " knots\n";
     }
     
     ss << "Position Accuracy: " << (position_accuracy_ ? "High" : "Low") << "\n";
     
     double lon = get_longitude();
     if (lon > 180.0) {
         ss << "Longitude: Not available\n";
     } else {
         ss << "Longitude: " << std::fixed << std::setprecision(6) << lon << " degrees\n";
     }
     
     double lat = get_latitude();
     if (lat > 90.0) {
         ss << "Latitude: Not available\n";
     } else {
         ss << "Latitude: " << std::fixed << std::setprecision(6) << lat << " degrees\n";
     }
     
     float cog = get_course_over_ground();
     if (std::isnan(cog)) {
         ss << "Course Over Ground: Not available\n";
     } else {
         ss << "Course Over Ground: " << std::fixed << std::setprecision(1) << cog << " degrees\n";
     }
     
     if (true_heading_ == 511) {
         ss << "True Heading: Not available\n";
     } else {
         ss << "True Heading: " << true_heading_ << " degrees\n";
     }
     
     switch (timestamp_) {
         case 60:
             ss << "Timestamp: Not available\n";
             break;
         case 61:
             ss << "Timestamp: Positioning system in manual input mode\n";
             break;
         case 62:
             ss << "Timestamp: Electronic position fixing system in estimated mode\n";
             break;
         case 63:
             ss << "Timestamp: Positioning system is inoperative\n";
             break;
         default:
             ss << "Timestamp: " << static_cast<int>(timestamp_) << " seconds\n";
             break;
     }
     
     switch (special_maneuver_) {
         case 0:
             ss << "Special Maneuver Indicator: Not available\n";
             break;
         case 1:
             ss << "Special Maneuver Indicator: Not engaged in special maneuver\n";
             break;
         case 2:
             ss << "Special Maneuver Indicator: Engaged in special maneuver\n";
             break;
     }
     
     ss << "RAIM Flag: " << (raim_flag_ ? "In use" : "Not in use") << "\n";
     ss << "Radio Status: 0x" << std::hex << std::setw(5) << std::setfill('0') << radio_status_;
     
     return ss.str();
 }
 
 } // namespace aislib