/**
 * @file static_data.cpp
 * @brief Implementation of StaticAndVoyageData class
 */

 #include "aislib/static_data.h"
 #include <sstream>
 #include <iomanip>
 #include <ctime>
 
 namespace aislib {
 
 StaticAndVoyageData::StaticAndVoyageData(uint32_t mmsi, uint8_t repeat_indicator)
     : mmsi_(mmsi),
       repeat_indicator_(repeat_indicator),
       ais_version_(0),
       imo_number_(0),
       call_sign_(),
       vessel_name_(),
       ship_type_(ShipType::NOT_AVAILABLE),
       dimension_to_bow_(0),
       dimension_to_stern_(0),
       dimension_to_port_(0),
       dimension_to_starboard_(0),
       epfd_type_(0),
       eta_month_(0),
       eta_day_(0),
       eta_hour_(24),
       eta_minute_(60),
       draught_(0),
       destination_(),
       dte_flag_(false) {
 }
 
 StaticAndVoyageData::StaticAndVoyageData(const BitVector& bits)
     : mmsi_(0),
       repeat_indicator_(0),
       ais_version_(0),
       imo_number_(0),
       call_sign_(),
       vessel_name_(),
       ship_type_(ShipType::NOT_AVAILABLE),
       dimension_to_bow_(0),
       dimension_to_stern_(0),
       dimension_to_port_(0),
       dimension_to_starboard_(0),
       epfd_type_(0),
       eta_month_(0),
       eta_day_(0),
       eta_hour_(24),
       eta_minute_(60),
       draught_(0),
       destination_(),
       dte_flag_(false) {
 
     // Check if we have a valid message type
     if (bits.get_uint(0, 6) != 5) {
         throw std::invalid_argument("Invalid message type for Static and Voyage Data");
     }
 
     // Parse the message fields
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     ais_version_ = static_cast<uint8_t>(bits.get_uint(38, 2));
     imo_number_ = static_cast<uint32_t>(bits.get_uint(40, 30));
     call_sign_ = bits.get_string(70, 42);  // 7 six-bit characters
     vessel_name_ = bits.get_string(112, 120);  // 20 six-bit characters
     ship_type_ = static_cast<ShipType>(bits.get_uint(232, 8));
     dimension_to_bow_ = static_cast<uint16_t>(bits.get_uint(240, 9));
     dimension_to_stern_ = static_cast<uint16_t>(bits.get_uint(249, 9));
     dimension_to_port_ = static_cast<uint8_t>(bits.get_uint(258, 6));
     dimension_to_starboard_ = static_cast<uint8_t>(bits.get_uint(264, 6));
     epfd_type_ = static_cast<uint8_t>(bits.get_uint(270, 4));
     eta_month_ = static_cast<uint8_t>(bits.get_uint(274, 4));
     eta_day_ = static_cast<uint8_t>(bits.get_uint(278, 5));
     eta_hour_ = static_cast<uint8_t>(bits.get_uint(283, 5));
     eta_minute_ = static_cast<uint8_t>(bits.get_uint(288, 6));
     draught_ = static_cast<uint8_t>(bits.get_uint(294, 8));
     destination_ = bits.get_string(302, 120);  // 20 six-bit characters
     dte_flag_ = bits.get_bit(422);
     
     // Spare (1 bit) - skipped
 }
 
 uint8_t StaticAndVoyageData::get_message_type() const {
     return 5;
 }
 
 uint32_t StaticAndVoyageData::get_mmsi() const {
     return mmsi_;
 }
 
 uint8_t StaticAndVoyageData::get_repeat_indicator() const {
     return repeat_indicator_;
 }
 
 uint8_t StaticAndVoyageData::get_ais_version() const {
     return ais_version_;
 }
 
 uint32_t StaticAndVoyageData::get_imo_number() const {
     return imo_number_;
 }
 
 std::string StaticAndVoyageData::get_call_sign() const {
     return call_sign_;
 }
 
 std::string StaticAndVoyageData::get_vessel_name() const {
     return vessel_name_;
 }
 
 StaticAndVoyageData::ShipType StaticAndVoyageData::get_ship_type() const {
     return ship_type_;
 }
 
 uint16_t StaticAndVoyageData::get_dimension_to_bow() const {
     return dimension_to_bow_;
 }
 
 uint16_t StaticAndVoyageData::get_dimension_to_stern() const {
     return dimension_to_stern_;
 }
 
 uint8_t StaticAndVoyageData::get_dimension_to_port() const {
     return dimension_to_port_;
 }
 
 uint8_t StaticAndVoyageData::get_dimension_to_starboard() const {
     return dimension_to_starboard_;
 }
 
 uint8_t StaticAndVoyageData::get_epfd_type() const {
     return epfd_type_;
 }
 
 uint8_t StaticAndVoyageData::get_eta_month() const {
     return eta_month_;
 }
 
 uint8_t StaticAndVoyageData::get_eta_day() const {
     return eta_day_;
 }
 
 uint8_t StaticAndVoyageData::get_eta_hour() const {
     return eta_hour_;
 }
 
 uint8_t StaticAndVoyageData::get_eta_minute() const {
     return eta_minute_;
 }
 
 std::chrono::system_clock::time_point StaticAndVoyageData::get_eta() const {
     // Check if ETA is available
     if (eta_month_ == 0 || eta_day_ == 0 || eta_hour_ >= 24 || eta_minute_ >= 60) {
         // Return a default time point
         return std::chrono::system_clock::time_point();
     }
 
     // Get the current time to determine the year
     std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
     std::tm* now_tm = std::gmtime(&now);
     int year = now_tm->tm_year + 1900;
 
     // If the ETA month is earlier than the current month, assume it's next year
     if (eta_month_ < static_cast<uint8_t>(now_tm->tm_mon + 1)) {
         year++;
     }
 
     // Create a tm structure for the ETA
     std::tm eta_tm = {};
     eta_tm.tm_year = year - 1900;
     eta_tm.tm_mon = eta_month_ - 1;
     eta_tm.tm_mday = eta_day_;
     eta_tm.tm_hour = eta_hour_;
     eta_tm.tm_min = eta_minute_;
     eta_tm.tm_sec = 0;
 
     // Convert to time_t and then to system_clock::time_point
     std::time_t eta_time_t = std::mktime(&eta_tm);
     return std::chrono::system_clock::from_time_t(eta_time_t);
 }
 
 float StaticAndVoyageData::get_draught() const {
     return draught_ * 0.1f;
 }
 
 std::string StaticAndVoyageData::get_destination() const {
     return destination_;
 }
 
 bool StaticAndVoyageData::get_dte_flag() const {
     return dte_flag_;
 }
 
 void StaticAndVoyageData::set_ais_version(uint8_t version) {
     if (version > 3) {
         ais_version_ = 0; // Default to 0 if invalid
     } else {
         ais_version_ = version;
     }
 }
 
 void StaticAndVoyageData::set_imo_number(uint32_t imo) {
     imo_number_ = imo;
 }
 
 void StaticAndVoyageData::set_call_sign(const std::string& call_sign) {
     call_sign_ = call_sign;
 }
 
 void StaticAndVoyageData::set_vessel_name(const std::string& name) {
     vessel_name_ = name;
 }
 
 void StaticAndVoyageData::set_ship_type(ShipType type) {
     ship_type_ = type;
 }
 
 void StaticAndVoyageData::set_ship_dimensions(uint16_t to_bow, uint16_t to_stern, uint8_t to_port, uint8_t to_starboard) {
     dimension_to_bow_ = to_bow;
     dimension_to_stern_ = to_stern;
     dimension_to_port_ = to_port;
     dimension_to_starboard_ = to_starboard;
 }
 
 void StaticAndVoyageData::set_epfd_type(uint8_t type) {
     if (type > 15) {
         epfd_type_ = 0; // Default to 0 if invalid
     } else {
         epfd_type_ = type;
     }
 }
 
 void StaticAndVoyageData::set_eta(const std::chrono::system_clock::time_point& eta) {
     // Convert time_point to tm
     std::time_t eta_time_t = std::chrono::system_clock::to_time_t(eta);
     std::tm* eta_tm = std::gmtime(&eta_time_t);
 
     // Set ETA components
     set_eta_components(
         static_cast<uint8_t>(eta_tm->tm_mon + 1),
         static_cast<uint8_t>(eta_tm->tm_mday),
         static_cast<uint8_t>(eta_tm->tm_hour),
         static_cast<uint8_t>(eta_tm->tm_min)
     );
 }
 
 void StaticAndVoyageData::set_eta_components(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
     // Validate and set month
     if (month > 12) {
         eta_month_ = 0; // Not available
     } else {
         eta_month_ = month;
     }
 
     // Validate and set day
     if (day > 31) {
         eta_day_ = 0; // Not available
     } else {
         eta_day_ = day;
     }
 
     // Validate and set hour
     if (hour > 23 && hour != 24) {
         eta_hour_ = 24; // Not available
     } else {
         eta_hour_ = hour;
     }
 
     // Validate and set minute
     if (minute > 59 && minute != 60) {
         eta_minute_ = 60; // Not available
     } else {
         eta_minute_ = minute;
     }
 }
 
 void StaticAndVoyageData::set_draught(float draught) {
     if (draught < 0.0f) {
         draught_ = 0;
     } else if (draught > 25.5f) {
         draught_ = 255;
     } else {
         draught_ = static_cast<uint8_t>(std::round(draught * 10.0f));
     }
 }
 
 void StaticAndVoyageData::set_destination(const std::string& destination) {
     destination_ = destination;
 }
 
 void StaticAndVoyageData::set_dte_flag(bool dte) {
     dte_flag_ = dte;
 }
 
 void StaticAndVoyageData::to_bits(BitVector& bits) const {
     // Message Type (6 bits)
     bits.append_uint(get_message_type(), 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(repeat_indicator_, 2);
     
     // MMSI (30 bits)
     bits.append_uint(mmsi_, 30);
     
     // AIS Version (2 bits)
     bits.append_uint(ais_version_, 2);
     
     // IMO Number (30 bits)
     bits.append_uint(imo_number_, 30);
     
     // Call Sign (42 bits, 7 characters * 6 bits)
     bits.append_string(call_sign_, 42);
     
     // Vessel Name (120 bits, 20 characters * 6 bits)
     bits.append_string(vessel_name_, 120);
     
     // Ship Type (8 bits)
     bits.append_uint(static_cast<uint8_t>(ship_type_), 8);
     
     // Dimension to Bow (9 bits)
     bits.append_uint(dimension_to_bow_, 9);
     
     // Dimension to Stern (9 bits)
     bits.append_uint(dimension_to_stern_, 9);
     
     // Dimension to Port (6 bits)
     bits.append_uint(dimension_to_port_, 6);
     
     // Dimension to Starboard (6 bits)
     bits.append_uint(dimension_to_starboard_, 6);
     
     // EPFD Type (4 bits)
     bits.append_uint(epfd_type_, 4);
     
     // ETA Month (4 bits)
     bits.append_uint(eta_month_, 4);
     
     // ETA Day (5 bits)
     bits.append_uint(eta_day_, 5);
     
     // ETA Hour (5 bits)
     bits.append_uint(eta_hour_, 5);
     
     // ETA Minute (6 bits)
     bits.append_uint(eta_minute_, 6);
     
     // Draught (8 bits)
     bits.append_uint(draught_, 8);
     
     // Destination (120 bits, 20 characters * 6 bits)
     bits.append_string(destination_, 120);
     
     // DTE Flag (1 bit)
     bits.append_bit(dte_flag_);
     
     // Spare (1 bit)
     bits.append_bit(false);
 }
 
 std::string StaticAndVoyageData::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << " (Static and Voyage Data)\n";
     ss << "MMSI: " << mmsi_ << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(repeat_indicator_) << "\n";
     ss << "AIS Version: " << static_cast<int>(ais_version_) << "\n";
     ss << "IMO Number: " << imo_number_ << "\n";
     ss << "Call Sign: " << call_sign_ << "\n";
     ss << "Vessel Name: " << vessel_name_ << "\n";
     ss << "Ship Type: " << static_cast<int>(ship_type_) << "\n";
     
     ss << "Ship Dimensions: " << dimension_to_bow_ << "m (bow) x " 
        << dimension_to_stern_ << "m (stern) x " 
        << static_cast<int>(dimension_to_port_) << "m (port) x " 
        << static_cast<int>(dimension_to_starboard_) << "m (starboard)\n";
     
     ss << "EPFD Type: " << static_cast<int>(epfd_type_) << "\n";
     
     if (eta_month_ == 0 || eta_day_ == 0 || eta_hour_ >= 24 || eta_minute_ >= 60) {
        ss << "ETA: Not available\n";
    } else {
        ss << "ETA: Month " << static_cast<int>(eta_month_) 
           << ", Day " << static_cast<int>(eta_day_)
           << ", " << static_cast<int>(eta_hour_) << ":"
           << std::setw(2) << std::setfill('0') << static_cast<int>(eta_minute_) << " UTC\n";
    }
    
    ss << "Draught: " << get_draught() << " meters\n";
    ss << "Destination: " << destination_ << "\n";
    ss << "DTE: " << (dte_flag_ ? "Ready" : "Not ready");
    
    return ss.str();
}

} // namespace aislib