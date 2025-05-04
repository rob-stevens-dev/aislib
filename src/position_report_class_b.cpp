/**
 * @file position_report_class_b.cpp
 * @brief Implementation of StandardPositionReportClassB and ExtendedPositionReportClassB classes
 */

 #include "aislib/position_report_class_b.h"
 #include <sstream>
 #include <iomanip>
 #include <cmath>
 
 namespace aislib {
 
 // Constants for encoding/decoding
 constexpr uint16_t SOG_NOT_AVAILABLE = 1023;
 constexpr int32_t LONGITUDE_NOT_AVAILABLE = 0x6791AC0;
 constexpr int32_t LATITUDE_NOT_AVAILABLE = 0x3412140;
 constexpr uint16_t COG_NOT_AVAILABLE = 3600;
 constexpr uint16_t HEADING_NOT_AVAILABLE = 511;
 
 // StandardPositionReportClassB implementation
 StandardPositionReportClassB::StandardPositionReportClassB(uint32_t mmsi, uint8_t repeat_indicator)
     : mmsi_(mmsi),
       repeat_indicator_(repeat_indicator),
       speed_over_ground_(SOG_NOT_AVAILABLE),
       position_accuracy_(false),
       longitude_(LONGITUDE_NOT_AVAILABLE),
       latitude_(LATITUDE_NOT_AVAILABLE),
       course_over_ground_(COG_NOT_AVAILABLE),
       true_heading_(HEADING_NOT_AVAILABLE),
       timestamp_(60), // not available
       cs_flag_(true),
       display_flag_(false),
       dsc_flag_(false),
       band_flag_(false),
       message_22_flag_(false),
       assigned_flag_(false),
       raim_flag_(false),
       radio_status_(0) {
 }
 
 StandardPositionReportClassB::StandardPositionReportClassB(const BitVector& bits)
     : mmsi_(0),
       repeat_indicator_(0),
       speed_over_ground_(SOG_NOT_AVAILABLE),
       position_accuracy_(false),
       longitude_(LONGITUDE_NOT_AVAILABLE),
       latitude_(LATITUDE_NOT_AVAILABLE),
       course_over_ground_(COG_NOT_AVAILABLE),
       true_heading_(HEADING_NOT_AVAILABLE),
       timestamp_(60), // not available
       cs_flag_(true),
       display_flag_(false),
       dsc_flag_(false),
       band_flag_(false),
       message_22_flag_(false),
       assigned_flag_(false),
       raim_flag_(false),
       radio_status_(0) {
 
     // Check if we have a valid message type
     if (bits.get_uint(0, 6) != 18) {
         throw std::invalid_argument("Invalid message type for Standard Position Report Class B");
     }
 
     // Parse the message fields
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     
     // Regional reserved (8 bits) - skipped
     
     speed_over_ground_ = static_cast<uint16_t>(bits.get_uint(46, 10));
     position_accuracy_ = bits.get_bit(56);
     longitude_ = static_cast<int32_t>(bits.get_int(57, 28));
     latitude_ = static_cast<int32_t>(bits.get_int(85, 27));
     course_over_ground_ = static_cast<uint16_t>(bits.get_uint(112, 12));
     true_heading_ = static_cast<uint16_t>(bits.get_uint(124, 9));
     timestamp_ = static_cast<uint8_t>(bits.get_uint(133, 6));
     
     // Regional reserved (2 bits) - skipped
     
     cs_flag_ = bits.get_bit(141);
     display_flag_ = bits.get_bit(142);
     dsc_flag_ = bits.get_bit(143);
     band_flag_ = bits.get_bit(144);
     message_22_flag_ = bits.get_bit(145);
     assigned_flag_ = bits.get_bit(146);
     raim_flag_ = bits.get_bit(147);
     radio_status_ = static_cast<uint32_t>(bits.get_uint(148, 20));
 }
 
 uint8_t StandardPositionReportClassB::get_message_type() const {
     return 18;
 }
 
 uint32_t StandardPositionReportClassB::get_mmsi() const {
     return mmsi_;
 }
 
 uint8_t StandardPositionReportClassB::get_repeat_indicator() const {
     return repeat_indicator_;
 }
 
 float StandardPositionReportClassB::get_speed_over_ground() const {
     if (speed_over_ground_ == SOG_NOT_AVAILABLE) {
         return -1.0f; // Not available
     }
     return speed_over_ground_ * 0.1f;
 }
 
 bool StandardPositionReportClassB::get_position_accuracy() const {
     return position_accuracy_;
 }
 
 double StandardPositionReportClassB::get_longitude() const {
     if (longitude_ == LONGITUDE_NOT_AVAILABLE) {
         return 181.0; // Not available
     }
     return longitude_ / 600000.0;
 }
 
 double StandardPositionReportClassB::get_latitude() const {
     if (latitude_ == LATITUDE_NOT_AVAILABLE) {
         return 91.0; // Not available
     }
     return latitude_ / 600000.0;
 }
 
 float StandardPositionReportClassB::get_course_over_ground() const {
     if (course_over_ground_ == COG_NOT_AVAILABLE) {
         return -1.0f; // Not available
     }
     return course_over_ground_ * 0.1f;
 }
 
 uint16_t StandardPositionReportClassB::get_true_heading() const {
     if (true_heading_ == HEADING_NOT_AVAILABLE) {
         return 511; // Not available
     }
     return true_heading_;
 }
 
 uint8_t StandardPositionReportClassB::get_timestamp() const {
     return timestamp_;
 }
 
 bool StandardPositionReportClassB::get_cs_flag() const {
     return cs_flag_;
 }
 
 bool StandardPositionReportClassB::get_display_flag() const {
     return display_flag_;
 }
 
 bool StandardPositionReportClassB::get_dsc_flag() const {
     return dsc_flag_;
 }
 
 bool StandardPositionReportClassB::get_band_flag() const {
     return band_flag_;
 }
 
 bool StandardPositionReportClassB::get_message_22_flag() const {
     return message_22_flag_;
 }
 
 bool StandardPositionReportClassB::get_assigned_flag() const {
     return assigned_flag_;
 }
 
 bool StandardPositionReportClassB::get_raim_flag() const {
     return raim_flag_;
 }
 
 uint32_t StandardPositionReportClassB::get_radio_status() const {
     return radio_status_;
 }
 
 void StandardPositionReportClassB::set_speed_over_ground(float sog) {
     if (sog < 0.0f) {
         speed_over_ground_ = SOG_NOT_AVAILABLE;
     } else if (sog > 102.2f) {
         speed_over_ground_ = 1022; // Max value (102.2 knots)
     } else {
         speed_over_ground_ = static_cast<uint16_t>(std::round(sog * 10.0f));
     }
 }
 
 void StandardPositionReportClassB::set_position_accuracy(bool accuracy) {
     position_accuracy_ = accuracy;
 }
 
 void StandardPositionReportClassB::set_longitude(double longitude) {
     if (longitude < -180.0 || longitude > 180.0) {
         longitude_ = LONGITUDE_NOT_AVAILABLE;
     } else {
         longitude_ = static_cast<int32_t>(std::round(longitude * 600000.0));
     }
 }
 
 void StandardPositionReportClassB::set_latitude(double latitude) {
     if (latitude < -90.0 || latitude > 90.0) {
         latitude_ = LATITUDE_NOT_AVAILABLE;
     } else {
         latitude_ = static_cast<int32_t>(std::round(latitude * 600000.0));
     }
 }
 
 void StandardPositionReportClassB::set_course_over_ground(float cog) {
     if (cog < 0.0f || cog >= 360.0f) {
         course_over_ground_ = COG_NOT_AVAILABLE;
     } else {
         course_over_ground_ = static_cast<uint16_t>(std::round(cog * 10.0f));
     }
 }
 
 void StandardPositionReportClassB::set_true_heading(uint16_t heading) {
     if (heading > 359 && heading != HEADING_NOT_AVAILABLE) {
         // Invalid heading value, set to not available
         true_heading_ = HEADING_NOT_AVAILABLE;
     } else {
         true_heading_ = heading;
     }
 }
 
 void StandardPositionReportClassB::set_timestamp(uint8_t timestamp) {
     if (timestamp > 59 && timestamp != 60 && timestamp != 61 && timestamp != 62 && timestamp != 63) {
         // Invalid timestamp value, set to not available
         timestamp_ = 60;
     } else {
         timestamp_ = timestamp;
     }
 }
 
 void StandardPositionReportClassB::set_cs_flag(bool cs_flag) {
     cs_flag_ = cs_flag;
 }
 
 void StandardPositionReportClassB::set_display_flag(bool display_flag) {
     display_flag_ = display_flag;
 }
 
 void StandardPositionReportClassB::set_dsc_flag(bool dsc_flag) {
     dsc_flag_ = dsc_flag;
 }
 
 void StandardPositionReportClassB::set_band_flag(bool band_flag) {
     band_flag_ = band_flag;
 }
 
 void StandardPositionReportClassB::set_message_22_flag(bool message_22_flag) {
     message_22_flag_ = message_22_flag;
 }
 
 void StandardPositionReportClassB::set_assigned_flag(bool assigned_flag) {
     assigned_flag_ = assigned_flag;
 }
 
 void StandardPositionReportClassB::set_raim_flag(bool raim_flag) {
     raim_flag_ = raim_flag;
 }
 
 void StandardPositionReportClassB::set_radio_status(uint32_t radio_status) {
     radio_status_ = radio_status;
 }
 
 void StandardPositionReportClassB::to_bits(BitVector& bits) const {
     // Message Type (6 bits)
     bits.append_uint(get_message_type(), 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(repeat_indicator_, 2);
     
     // MMSI (30 bits)
     bits.append_uint(mmsi_, 30);
     
     // Regional Reserved (8 bits)
     bits.append_uint(0, 8);
     
     // Speed Over Ground (10 bits)
     bits.append_uint(speed_over_ground_, 10);
     
     // Position Accuracy (1 bit)
     bits.append_bit(position_accuracy_);
     
     // Longitude (28 bits)
     bits.append_int(longitude_, 28);
     
     // Latitude (27 bits)
     bits.append_int(latitude_, 27);
     
     // Course Over Ground (12 bits)
     bits.append_uint(course_over_ground_, 12);
     
     // True Heading (9 bits)
     bits.append_uint(true_heading_, 9);
     
     // Time Stamp (6 bits)
     bits.append_uint(timestamp_, 6);
     
     // Regional Reserved (2 bits)
     bits.append_uint(0, 2);
     
     // CS flag (1 bit)
     bits.append_bit(cs_flag_);
     
     // Display flag (1 bit)
     bits.append_bit(display_flag_);
     
     // DSC flag (1 bit)
     bits.append_bit(dsc_flag_);
     
     // Band flag (1 bit)
     bits.append_bit(band_flag_);
     
     // Message 22 flag (1 bit)
     bits.append_bit(message_22_flag_);
     
     // Assigned flag (1 bit)
     bits.append_bit(assigned_flag_);
     
     // RAIM flag (1 bit)
     bits.append_bit(raim_flag_);
     
     // Radio status (20 bits)
     bits.append_uint(radio_status_, 20);
 }
 
 std::string StandardPositionReportClassB::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << " (Standard Position Report Class B)\n";
     ss << "MMSI: " << mmsi_ << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(repeat_indicator_) << "\n";
     
     float sog = get_speed_over_ground();
     if (sog < 0.0f) {
         ss << "Speed Over Ground: Not available\n";
     } else {
         ss << "Speed Over Ground: " << sog << " knots\n";
     }
     
     ss << "Position Accuracy: " << (position_accuracy_ ? "High" : "Low/GNSS > 10m") << "\n";
     
     double lon = get_longitude();
     if (lon > 180.0) {
         ss << "Longitude: Not available\n";
     } else {
         ss << "Longitude: " << lon << " degrees\n";
     }
     
     double lat = get_latitude();
     if (lat > 90.0) {
         ss << "Latitude: Not available\n";
     } else {
         ss << "Latitude: " << lat << " degrees\n";
     }
     
     float cog = get_course_over_ground();
     if (cog < 0.0f) {
         ss << "Course Over Ground: Not available\n";
     } else {
         ss << "Course Over Ground: " << cog << " degrees\n";
     }
     
     if (true_heading_ == HEADING_NOT_AVAILABLE) {
         ss << "True Heading: Not available\n";
     } else {
         ss << "True Heading: " << true_heading_ << " degrees\n";
     }
     
     if (timestamp_ == 60) {
         ss << "Timestamp: Not available\n";
     } else if (timestamp_ == 61) {
         ss << "Timestamp: Manual input mode\n";
     } else if (timestamp_ == 62) {
         ss << "Timestamp: Dead reckoning mode\n";
     } else if (timestamp_ == 63) {
         ss << "Timestamp: Positioning system inoperative\n";
     } else {
         ss << "Timestamp: " << static_cast<int>(timestamp_) << " seconds\n";
     }
     
     ss << "Class B CS Flag: " << (cs_flag_ ? "SOTDMA" : "ITDMA") << "\n";
     ss << "Display Flag: " << (display_flag_ ? "Yes" : "No") << "\n";
     ss << "DSC Flag: " << (dsc_flag_ ? "Yes" : "No") << "\n";
     ss << "Band Flag: " << (band_flag_ ? "All marine bands" : "Single band") << "\n";
     ss << "Message 22 Flag: " << (message_22_flag_ ? "Yes" : "No") << "\n";
     ss << "Assigned Flag: " << (assigned_flag_ ? "Yes" : "No") << "\n";
     ss << "RAIM Flag: " << (raim_flag_ ? "In use" : "Not in use") << "\n";
     ss << "Radio Status: 0x" << std::hex << std::setw(5) << std::setfill('0') << radio_status_;
     
     return ss.str();
 }
 
 // ExtendedPositionReportClassB implementation
 ExtendedPositionReportClassB::ExtendedPositionReportClassB(uint32_t mmsi, uint8_t repeat_indicator)
     : StandardPositionReportClassB(mmsi, repeat_indicator),
       vessel_name_(),
       ship_type_(0),
       dimension_to_bow_(0),
       dimension_to_stern_(0),
       dimension_to_port_(0),
       dimension_to_starboard_(0),
       epfd_type_(0) {
 }
 
 ExtendedPositionReportClassB::ExtendedPositionReportClassB(const BitVector& bits)
     : StandardPositionReportClassB(0, 0),
       vessel_name_(),
       ship_type_(0),
       dimension_to_bow_(0),
       dimension_to_stern_(0),
       dimension_to_port_(0),
       dimension_to_starboard_(0),
       epfd_type_(0) {
     
     // Check if we have a valid message type
     if (bits.get_uint(0, 6) != 19) {
         throw std::invalid_argument("Invalid message type for Extended Position Report Class B");
     }
 
     // Parse the standard fields first
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     
     // Regional reserved (8 bits) - skipped
     
     speed_over_ground_ = static_cast<uint16_t>(bits.get_uint(46, 10));
     position_accuracy_ = bits.get_bit(56);
     longitude_ = static_cast<int32_t>(bits.get_int(57, 28));
     latitude_ = static_cast<int32_t>(bits.get_int(85, 27));
     course_over_ground_ = static_cast<uint16_t>(bits.get_uint(112, 12));
     true_heading_ = static_cast<uint16_t>(bits.get_uint(124, 9));
     timestamp_ = static_cast<uint8_t>(bits.get_uint(133, 6));
     
     // Regional reserved (4 bits) - skipped
     
     // Parse the extended fields
     vessel_name_ = bits.get_string(143, 120);
     ship_type_ = static_cast<uint8_t>(bits.get_uint(263, 8));
     dimension_to_bow_ = static_cast<uint16_t>(bits.get_uint(271, 9));
     dimension_to_stern_ = static_cast<uint16_t>(bits.get_uint(280, 9));
     dimension_to_port_ = static_cast<uint8_t>(bits.get_uint(289, 6));
     dimension_to_starboard_ = static_cast<uint8_t>(bits.get_uint(295, 6));
     epfd_type_ = static_cast<uint8_t>(bits.get_uint(301, 4));
     raim_flag_ = bits.get_bit(305);
     
     // DTE flag (1 bit) - skipped
     
     // Assigned mode flag (1 bit)
     assigned_flag_ = bits.get_bit(307);
 }
 
 uint8_t ExtendedPositionReportClassB::get_message_type() const {
     return 19;
 }
 
 std::string ExtendedPositionReportClassB::get_vessel_name() const {
     return vessel_name_;
 }
 
 uint8_t ExtendedPositionReportClassB::get_ship_type() const {
     return ship_type_;
 }
 
 uint16_t ExtendedPositionReportClassB::get_dimension_to_bow() const {
     return dimension_to_bow_;
 }
 
 uint16_t ExtendedPositionReportClassB::get_dimension_to_stern() const {
     return dimension_to_stern_;
 }
 
 uint8_t ExtendedPositionReportClassB::get_dimension_to_port() const {
     return dimension_to_port_;
 }
 
 uint8_t ExtendedPositionReportClassB::get_dimension_to_starboard() const {
     return dimension_to_starboard_;
 }
 
 uint8_t ExtendedPositionReportClassB::get_epfd_type() const {
     return epfd_type_;
 }
 
 void ExtendedPositionReportClassB::set_vessel_name(const std::string& name) {
     vessel_name_ = name;
 }
 
 void ExtendedPositionReportClassB::set_ship_type(uint8_t type) {
     ship_type_ = type;
 }
 
 void ExtendedPositionReportClassB::set_ship_dimensions(
     uint16_t to_bow, 
     uint16_t to_stern, 
     uint8_t to_port, 
     uint8_t to_starboard
 ) {
     dimension_to_bow_ = to_bow;
     dimension_to_stern_ = to_stern;
     dimension_to_port_ = to_port;
     dimension_to_starboard_ = to_starboard;
 }
 
 void ExtendedPositionReportClassB::set_epfd_type(uint8_t type) {
     epfd_type_ = type;
 }
 
 void ExtendedPositionReportClassB::to_bits(BitVector& bits) const {
     // Message Type (6 bits)
     bits.append_uint(get_message_type(), 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(get_repeat_indicator(), 2);
     
     // MMSI (30 bits)
     bits.append_uint(get_mmsi(), 30);
     
     // Regional Reserved (8 bits)
     bits.append_uint(0, 8);
     
     // Speed Over Ground (10 bits)
     bits.append_uint(speed_over_ground_, 10);
     
     // Position Accuracy (1 bit)
     bits.append_bit(position_accuracy_);
     
     // Longitude (28 bits)
     bits.append_int(longitude_, 28);
     
     // Latitude (27 bits)
     bits.append_int(latitude_, 27);
     
     // Course Over Ground (12 bits)
     bits.append_uint(course_over_ground_, 12);
     
     // True Heading (9 bits)
     bits.append_uint(true_heading_, 9);
     
     // Time Stamp (6 bits)
     bits.append_uint(timestamp_, 6);
     
     // Regional Reserved (4 bits)
     bits.append_uint(0, 4);
     
     // Vessel Name (120 bits, 20 characters * 6 bits)
     bits.append_string(vessel_name_, 120);
     
     // Ship Type (8 bits)
     bits.append_uint(ship_type_, 8);
     
     // Dimension to Bow (9 bits)
     bits.append_uint(dimension_to_bow_, 9);
     
     // Dimension to Stern (9 bits)
     bits.append_uint(dimension_to_stern_, 9);
     
     // Dimension to Port (6 bits)
     bits.append_uint(dimension_to_port_, 6);
     
     // Dimension to Starboard (6 bits)
     bits.append_uint(dimension_to_starboard_, 6);
     
     // Type of EPFD (4 bits)
     bits.append_uint(epfd_type_, 4);
     
     // RAIM Flag (1 bit)
     bits.append_bit(raim_flag_);
     
     // DTE Flag (1 bit)
     bits.append_bit(false);
     
     // Assigned Mode Flag (1 bit)
     bits.append_bit(assigned_flag_);
     
     // Spare (4 bits)
     bits.append_uint(0, 4);
 }
 
 std::string ExtendedPositionReportClassB::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << " (Extended Position Report Class B)\n";
     ss << "MMSI: " << get_mmsi() << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(get_repeat_indicator()) << "\n";
     
     float sog = get_speed_over_ground();
     if (sog < 0.0f) {
         ss << "Speed Over Ground: Not available\n";
     } else {
         ss << "Speed Over Ground: " << sog << " knots\n";
     }
     
     ss << "Position Accuracy: " << (position_accuracy_ ? "High" : "Low/GNSS > 10m") << "\n";
     
     double lon = get_longitude();
     if (lon > 180.0) {
         ss << "Longitude: Not available\n";
     } else {
         ss << "Longitude: " << lon << " degrees\n";
     }
     
     double lat = get_latitude();
     if (lat > 90.0) {
         ss << "Latitude: Not available\n";
     } else {
         ss << "Latitude: " << lat << " degrees\n";
     }
     
     float cog = get_course_over_ground();
     if (cog < 0.0f) {
         ss << "Course Over Ground: Not available\n";
     } else {
         ss << "Course Over Ground: " << cog << " degrees\n";
     }
     
     if (true_heading_ == HEADING_NOT_AVAILABLE) {
         ss << "True Heading: Not available\n";
     } else {
         ss << "True Heading: " << true_heading_ << " degrees\n";
     }
     
     if (timestamp_ == 60) {
         ss << "Timestamp: Not available\n";
     } else if (timestamp_ == 61) {
         ss << "Timestamp: Manual input mode\n";
     } else if (timestamp_ == 62) {
         ss << "Timestamp: Dead reckoning mode\n";
     } else if (timestamp_ == 63) {
         ss << "Timestamp: Positioning system inoperative\n";
     } else {
         ss << "Timestamp: " << static_cast<int>(timestamp_) << " seconds\n";
     }
     
     ss << "Vessel Name: " << vessel_name_ << "\n";
     ss << "Ship Type: " << static_cast<int>(ship_type_) << "\n";
     
     ss << "Ship Dimensions: " << dimension_to_bow_ << "m (bow) x " 
        << dimension_to_stern_ << "m (stern) x " 
        << static_cast<int>(dimension_to_port_) << "m (port) x " 
        << static_cast<int>(dimension_to_starboard_) << "m (starboard)\n";
     
     ss << "EPFD Type: " << static_cast<int>(epfd_type_) << "\n";
     ss << "RAIM Flag: " << (raim_flag_ ? "In use" : "Not in use") << "\n";
     ss << "Assigned Flag: " << (assigned_flag_ ? "Yes" : "No");
     
     return ss.str();
 }
 
 } // namespace aislib