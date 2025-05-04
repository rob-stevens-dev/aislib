/**
 * @file base_station_report.h
 * @brief Base Station Report (Message Type 4)
 * 
 * This file defines the base station report message (Type 4)
 * used by AIS base stations.
 */

 #ifndef AISLIB_BASE_STATION_REPORT_H
 #define AISLIB_BASE_STATION_REPORT_H
 
 #include "ais_message.h"
 #include <chrono>
 #include <ctime>
 #include <string>
 
 namespace aislib {
 
 /**
  * @class BaseStationReport
  * @brief Class for AIS message type 4 (Base Station Report)
  * 
  * Message type 4 provides UTC date and time, position, and GNSS status
  * information from shore stations and AIS aids to navigation.
  */
 class BaseStationReport : public AISMessage {
 public:
     /**
      * @brief Constructor
      * @param mmsi MMSI number
      * @param repeat_indicator Repeat indicator
      */
     BaseStationReport(uint32_t mmsi, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit BaseStationReport(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~BaseStationReport() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (4)
      */
     uint8_t get_message_type() const override;
     
     /**
      * @brief Get the MMSI
      * @return MMSI number
      */
     uint32_t get_mmsi() const override;
     
     /**
      * @brief Get the repeat indicator
      * @return Repeat indicator
      */
     uint8_t get_repeat_indicator() const override;
     
     /**
      * @brief Get the UTC year
      * @return UTC year (1-9999, 0 = not available)
      */
     uint16_t get_utc_year() const;
     
     /**
      * @brief Get the UTC month
      * @return UTC month (1-12, 0 = not available)
      */
     uint8_t get_utc_month() const;
     
     /**
      * @brief Get the UTC day
      * @return UTC day (1-31, 0 = not available)
      */
     uint8_t get_utc_day() const;
     
     /**
      * @brief Get the UTC hour
      * @return UTC hour (0-23, 24 = not available)
      */
     uint8_t get_utc_hour() const;
     
     /**
      * @brief Get the UTC minute
      * @return UTC minute (0-59, 60 = not available)
      */
     uint8_t get_utc_minute() const;
     
     /**
      * @brief Get the UTC second
      * @return UTC second (0-59, 60 = not available)
      */
     uint8_t get_utc_second() const;
     
     /**
      * @brief Get the UTC timestamp
      * @return UTC timestamp as system_clock::time_point
      */
     std::chrono::system_clock::time_point get_utc_timestamp() const;
     
     /**
      * @brief Get the position accuracy flag
      * @return Position accuracy (true = high accuracy (< 10m), false = low accuracy (>= 10m))
      */
     bool get_position_accuracy() const;
     
     /**
      * @brief Get the longitude
      * @return Longitude in degrees (-180 to 180, 181 = not available)
      */
     double get_longitude() const;
     
     /**
      * @brief Get the latitude
      * @return Latitude in degrees (-90 to 90, 91 = not available)
      */
     double get_latitude() const;
     
     /**
      * @brief Get the type of electronic position fixing device
      * @return EPFD type (0-8, 15 = not available)
      */
     uint8_t get_epfd_type() const;
     
     /**
      * @brief Get the RAIM flag
      * @return Receiver Autonomous Integrity Monitoring flag (true = RAIM in use, false = RAIM not in use)
      */
     bool get_raim_flag() const;
     
     /**
      * @brief Set the UTC time components
      * @param year UTC year (1-9999, 0 = not available)
      * @param month UTC month (1-12, 0 = not available)
      * @param day UTC day (1-31, 0 = not available)
      * @param hour UTC hour (0-23, 24 = not available)
      * @param minute UTC minute (0-59, 60 = not available)
      * @param second UTC second (0-59, 60 = not available)
      */
     void set_utc_time(uint16_t year, uint8_t month, uint8_t day,
                      uint8_t hour, uint8_t minute, uint8_t second);
     
     /**
      * @brief Set the UTC timestamp
      * @param timestamp UTC timestamp
      */
     void set_utc_timestamp(const std::chrono::system_clock::time_point& timestamp);
     
     /**
      * @brief Set the position accuracy flag
      * @param accuracy Position accuracy (true = high accuracy, false = low accuracy)
      */
     void set_position_accuracy(bool accuracy);
     
     /**
      * @brief Set the longitude
      * @param longitude Longitude in degrees (-180 to 180, 181 = not available)
      */
     void set_longitude(double longitude);
     
     /**
      * @brief Set the latitude
      * @param latitude Latitude in degrees (-90 to 90, 91 = not available)
      */
     void set_latitude(double latitude);
     
     /**
      * @brief Set the type of electronic position fixing device
      * @param type EPFD type (0-8, 15 = not available)
      */
     void set_epfd_type(uint8_t type);
     
     /**
      * @brief Set the RAIM flag
      * @param raim Receiver Autonomous Integrity Monitoring flag (true = RAIM in use, false = RAIM not in use)
      */
     void set_raim_flag(bool raim);
     
     /**
      * @brief Convert the message to a bit vector
      * @param bits Bit vector to populate
      */
     void to_bits(BitVector& bits) const override;
     
     /**
      * @brief Get a string representation of the message
      * @return String representation
      */
     std::string to_string() const override;
     
 private:
     uint32_t mmsi_;               // 30 bits: MMSI number
     uint8_t repeat_indicator_;    // 2 bits: Repeat indicator
     uint16_t utc_year_;           // 14 bits: UTC year (1-9999, 0 = not available)
     uint8_t utc_month_;           // 4 bits: UTC month (1-12, 0 = not available)
     uint8_t utc_day_;             // 5 bits: UTC day (1-31, 0 = not available)
     uint8_t utc_hour_;            // 5 bits: UTC hour (0-23, 24 = not available)
     uint8_t utc_minute_;          // 6 bits: UTC minute (0-59, 60 = not available)
     uint8_t utc_second_;          // 6 bits: UTC second (0-59, 60 = not available)
     bool position_accuracy_;      // 1 bit: Position accuracy
     int32_t longitude_;           // 28 bits: Longitude (1/10000 minute resolution)
     int32_t latitude_;            // 27 bits: Latitude (1/10000 minute resolution)
     uint8_t epfd_type_;           // 4 bits: Type of EPFD
     uint8_t spare_;               // 10 bits: Spare
     bool raim_flag_;              // 1 bit: RAIM flag
     uint32_t radio_status_;       // 19 bits: Radio status
 };
 
 } // namespace aislib
 
 #endif // AISLIB_BASE_STATION_REPORT_H