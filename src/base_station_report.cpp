/**
 * @file base_station_report.cpp
 * @brief Implementation of BaseStationReport class
 */

 #include "aislib/base_station_report.h"
 #include <sstream>
 #include <iomanip>
 #include <cmath>
 
 namespace aislib {
 
 BaseStationReport::BaseStationReport(
     uint32_t mmsi,
     uint8_t repeat_indicator
 ) : mmsi_(mmsi),
     repeat_indicator_(repeat_indicator),
     utc_year_(0),
     utc_month_(0),
     utc_day_(0),
     utc_hour_(24),
     utc_minute_(60),
     utc_second_(60),
     position_accuracy_(false),
     longitude_(0x6791AC0),  // 181 degrees = not available
     latitude_(0x3412140),   // 91 degrees = not available
     epfd_type_(15),         // 15 = not available
     spare_(0),
     raim_flag_(false),
     radio_status_(0) {
 }
 
 BaseStationReport::BaseStationReport(const BitVector& bits) {
     // Check if we have enough bits for a Base Station Report message
     if (bits.size() < 168) {
         throw std::invalid_argument("Bit vector too small for a Base Station Report message");
     }
 
     // Extract message type
     uint8_t message_type = static_cast<uint8_t>(bits.get_uint(0, 6));
     if (message_type != 4) {
         throw std::invalid_argument("Invalid message type for Base Station Report");
     }
 
     // Extract fields
     repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
     mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
     utc_year_ = static_cast<uint16_t>(bits.get_uint(38, 14));
     utc_month_ = static_cast<uint8_t>(bits.get_uint(52, 4));
     utc_day_ = static_cast<uint8_t>(bits.get_uint(56, 5));
     utc_hour_ = static_cast<uint8_t>(bits.get_uint(61, 5));
     utc_minute_ = static_cast<uint8_t>(bits.get_uint(66, 6));
     utc_second_ = static_cast<uint8_t>(bits.get_uint(72, 6));
     position_accuracy_ = bits.get_bit(78);
     longitude_ = static_cast<int32_t>(bits.get_int(79, 28));
     latitude_ = static_cast<int32_t>(bits.get_int(107, 27));
     epfd_type_ = static_cast<uint8_t>(bits.get_uint(134, 4));
     spare_ = static_cast<uint8_t>(bits.get_uint(138, 10));
     raim_flag_ = bits.get_bit(148);
     radio_status_ = static_cast<uint32_t>(bits.get_uint(149, 19));
 }
 
 uint8_t BaseStationReport::get_message_type() const {
     return 4;
 }
 
 uint32_t BaseStationReport::get_mmsi() const {
     return mmsi_;
 }
 
 uint8_t BaseStationReport::get_repeat_indicator() const {
     return repeat_indicator_;
 }
 
 uint16_t BaseStationReport::get_utc_year() const {
     return utc_year_;
 }
 
 uint8_t BaseStationReport::get_utc_month() const {
     return utc_month_;
 }
 
 uint8_t BaseStationReport::get_utc_day() const {
     return utc_day_;
 }
 
 uint8_t BaseStationReport::get_utc_hour() const {
     return utc_hour_;
 }
 
 uint8_t BaseStationReport::get_utc_minute() const {
     return utc_minute_;
 }
 
 uint8_t BaseStationReport::get_utc_second() const {
     return utc_second_;
 }
 
 std::chrono::system_clock::time_point BaseStationReport::get_utc_timestamp() const {
     // If any component is unavailable, return epoch
     if (utc_year_ == 0 || utc_month_ == 0 || utc_day_ == 0 ||
         utc_hour_ == 24 || utc_minute_ == 60 || utc_second_ == 60) {
         return std::chrono::system_clock::time_point();
     }
     
     // Set up the time components
     std::tm timeinfo = {};
     timeinfo.tm_year = utc_year_ - 1900; // Years since 1900
     timeinfo.tm_mon = utc_month_ - 1;    // Months since January (0-11)
     timeinfo.tm_mday = utc_day_;         // Day of the month (1-31)
     timeinfo.tm_hour = utc_hour_;        // Hours (0-23)
     timeinfo.tm_min = utc_minute_;       // Minutes (0-59)
     timeinfo.tm_sec = utc_second_;       // Seconds (0-59)
     
     // Convert to time_t
     std::time_t time = std::mktime(&timeinfo);
     
     // Convert to system_clock::time_point
     return std::chrono::system_clock::from_time_t(time);
 }
 
 bool BaseStationReport::get_position_accuracy() const {
     return position_accuracy_;
 }
 
 double BaseStationReport::get_longitude() const {
     const int32_t longitude_not_available = 0x6791AC0; // 181 degrees in 1/10000 minute
     if (longitude_ == longitude_not_available) {
         return 181.0; // Not available
     }
     
     // Convert from 1/10000 minute to degrees
     // 1/10000 minute = 1/600000 degree
     return longitude_ / 600000.0;
 }
 
 double BaseStationReport::get_latitude() const {
     const int32_t latitude_not_available = 0x3412140; // 91 degrees in 1/10000 minute
     if (latitude_ == latitude_not_available) {
         return 91.0; // Not available
     }
     
     // Convert from 1/10000 minute to degrees
     // 1/10000 minute = 1/600000 degree
     return latitude_ / 600000.0;
 }
 
 uint8_t BaseStationReport::get_epfd_type() const {
     return epfd_type_;
 }
 
 bool BaseStationReport::get_raim_flag() const {
     return raim_flag_;
 }
 
 void BaseStationReport::set_utc_time(
     uint16_t year, uint8_t month, uint8_t day,
     uint8_t hour, uint8_t minute, uint8_t second
 ) {
     // Validate and set UTC year
     if (year > 9999) {
         utc_year_ = 0; // Not available
     } else {
         utc_year_ = year;
     }
     
     // Validate and set UTC month
     if (month > 12) {
         utc_month_ = 0; // Not available
     } else {
         utc_month_ = month;
     }
     
     // Validate and set UTC day
     if (day > 31) {
         utc_day_ = 0; // Not available
     } else {
         utc_day_ = day;
     }
     
     // Validate and set UTC hour
     if (hour > 23) {
         utc_hour_ = 24; // Not available
     } else {
         utc_hour_ = hour;
     }
     
     // Validate and set UTC minute
     if (minute > 59) {
         utc_minute_ = 60; // Not available
     } else {
         utc_minute_ = minute;
     }
     
     // Validate and set UTC second
     if (second > 59) {
         utc_second_ = 60; // Not available
     } else {
         utc_second_ = second;
     }
 }
 
 void BaseStationReport::set_utc_timestamp(const std::chrono::system_clock::time_point& timestamp) {
     // Convert to time_t
     std::time_t time = std::chrono::system_clock::to_time_t(timestamp);
     
     // Convert to tm struct
     std::tm* timeinfo = std::gmtime(&time);
     
     if (timeinfo) {
         // Set UTC components
         utc_year_ = timeinfo->tm_year + 1900;  // Years since 1900
         utc_month_ = timeinfo->tm_mon + 1;     // Months since January (0-11)
         utc_day_ = timeinfo->tm_mday;          // Day of the month (1-31)
         utc_hour_ = timeinfo->tm_hour;         // Hours (0-23)
         utc_minute_ = timeinfo->tm_min;        // Minutes (0-59)
         utc_second_ = timeinfo->tm_sec;        // Seconds (0-59)
     } else {
         // Invalid timestamp
         utc_year_ = 0;
         utc_month_ = 0;
         utc_day_ = 0;
         utc_hour_ = 24;
         utc_minute_ = 60;
         utc_second_ = 60;
     }
 }
 
 void BaseStationReport::set_position_accuracy(bool accuracy) {
     position_accuracy_ = accuracy;
 }
 
 void BaseStationReport::set_longitude(double longitude) {
     if (longitude > 180.0 || longitude < -180.0) {
         longitude_ = 0x6791AC0; // 181 degrees = not available
         return;
     }
     
     // Convert from degrees to 1/10000 minute
     // 1 degree = 60 minutes, 1 minute = 10000 units
     longitude_ = static_cast<int32_t>(std::round(longitude * 600000.0));
 }
 
 void BaseStationReport::set_latitude(double latitude) {
     if (latitude > 90.0 || latitude < -90.0) {
         latitude_ = 0x3412140; // 91 degrees = not available
         return;
     }
     
     // Convert from degrees to 1/10000 minute
     // 1 degree = 60 minutes, 1 minute = 10000 units
     latitude_ = static_cast<int32_t>(std::round(latitude * 600000.0));
 }
 
 void BaseStationReport::set_epfd_type(uint8_t type) {
     if (type > 8 && type != 15) {
         epfd_type_ = 15; // Not available
     } else {
         epfd_type_ = type;
     }
 }
 
 void BaseStationReport::set_raim_flag(bool raim) {
     raim_flag_ = raim;
 }
 
 void BaseStationReport::to_bits(BitVector& bits) const {
     // Clear the bit vector
     bits.clear();
     
     // Message Type (6 bits) = 4
     bits.append_uint(4, 6);
     
     // Repeat Indicator (2 bits)
     bits.append_uint(repeat_indicator_, 2);
     
     // MMSI (30 bits)
     bits.append_uint(mmsi_, 30);
     
     // UTC Year (14 bits)
     bits.append_uint(utc_year_, 14);
     
     // UTC Month (4 bits)
     bits.append_uint(utc_month_, 4);
     
     // UTC Day (5 bits)
     bits.append_uint(utc_day_, 5);
     
     // UTC Hour (5 bits)
     bits.append_uint(utc_hour_, 5);
     
     // UTC Minute (6 bits)
     bits.append_uint(utc_minute_, 6);
     
     // UTC Second (6 bits)
     bits.append_uint(utc_second_, 6);
     
     // Position Accuracy (1 bit)
     bits.append_bit(position_accuracy_);
     
     // Longitude (28 bits)
     bits.append_int(longitude_, 28);
     
     // Latitude (27 bits)
     bits.append_int(latitude_, 27);
     
     // EPFD Type (4 bits)
     bits.append_uint(epfd_type_, 4);
     
     // Spare (10 bits)
     bits.append_uint(spare_, 10);
     
     // RAIM Flag (1 bit)
     bits.append_bit(raim_flag_);
     
     // Radio Status (19 bits)
     bits.append_uint(radio_status_, 19);
 }
 
 std::string BaseStationReport::to_string() const {
     std::stringstream ss;
     
     ss << "AIS Message Type: 4 (Base Station Report)\n";
     ss << "MMSI: " << mmsi_ << "\n";
     ss << "Repeat Indicator: " << static_cast<int>(repeat_indicator_) << "\n";
     
     // UTC Time
     if (utc_year_ == 0 || utc_month_ == 0 || utc_day_ == 0) {
         ss << "UTC Date: Not available\n";
     } else {
         ss << "UTC Date: " << utc_year_ << "-"
            << std::setw(2) << std::setfill('0') << static_cast<int>(utc_month_) << "-"
            << std::setw(2) << std::setfill('0') << static_cast<int>(utc_day_) << "\n";
     }
     
     if (utc_hour_ == 24 || utc_minute_ == 60 || utc_second_ == 60) {
         ss << "UTC Time: Not available\n";
     } else {
         ss << "UTC Time: "
            << std::setw(2) << std::setfill('0') << static_cast<int>(utc_hour_) << ":"
            << std::setw(2) << std::setfill('0') << static_cast<int>(utc_minute_) << ":"
            << std::setw(2) << std::setfill('0') << static_cast<int>(utc_second_) << "\n";
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
     
     // EPFD Type
     ss << "EPFD Type: ";
     switch (epfd_type_) {
         case 0:
             ss << "Undefined\n";
             break;
         case 1:
             ss << "GPS\n";
             break;
         case 2:
             ss << "GLONASS\n";
             break;
         case 3:
             ss << "Combined GPS/GLONASS\n";
             break;
         case 4:
             ss << "Loran-C\n";
             break;
         case 5:
             ss << "Chayka\n";
             break;
         case 6:
             ss << "Integrated Navigation System\n";
             break;
         case 7:
             ss << "Surveyed\n";
             break;
         case 8:
             ss << "Galileo\n";
             break;
         case 15:
             ss << "Not available\n";
             break;
         default:
             ss << "Unknown (" << static_cast<int>(epfd_type_) << ")\n";
             break;
     }
     
     ss << "RAIM Flag: " << (raim_flag_ ? "In use" : "Not in use") << "\n";
     ss << "Radio Status: 0x" << std::hex << std::setw(5) << std::setfill('0') << radio_status_;
     
     return ss.str();
 }
 
 } // namespace aislib