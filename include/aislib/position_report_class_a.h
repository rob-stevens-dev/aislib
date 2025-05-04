/**
 * @file position_report_class_a.h
 * @brief Position Report Class A (Message Types 1, 2, 3)
 * 
 * This file defines the position report message types used by AIS class A transceivers.
 */

 #ifndef AISLIB_POSITION_REPORT_CLASS_A_H
 #define AISLIB_POSITION_REPORT_CLASS_A_H
 
 #include "ais_message.h"
 #include <cmath>
 
 namespace aislib {
 
 /**
  * @class PositionReportClassA
  * @brief Class for AIS message types 1, 2, and 3 (Position Report Class A)
  * 
  * Message types 1, 2, and 3 provide position, course, speed, and other
  * navigation-related information for vessels equipped with Class A transceivers.
  */
 class PositionReportClassA : public AISMessage {
 public:
     /**
      * @brief Navigation status values
      */
     enum class NavigationStatus : uint8_t {
         UNDER_WAY_USING_ENGINE = 0,
         AT_ANCHOR = 1,
         NOT_UNDER_COMMAND = 2,
         RESTRICTED_MANEUVERABILITY = 3,
         CONSTRAINED_BY_DRAUGHT = 4,
         MOORED = 5,
         AGROUND = 6,
         ENGAGED_IN_FISHING = 7,
         UNDER_WAY_SAILING = 8,
         RESERVED_HSC = 9,
         RESERVED_WIG = 10,
         RESERVED_11 = 11,
         RESERVED_12 = 12,
         RESERVED_13 = 13,
         AIS_SART_ACTIVE = 14,
         NOT_DEFINED = 15
     };
 
     /**
      * @brief Constructor
      * @param message_type Message type (1, 2, or 3)
      * @param mmsi MMSI number
      * @param repeat_indicator Repeat indicator
      * @param nav_status Navigation status
      */
     PositionReportClassA(uint8_t message_type, uint32_t mmsi, uint8_t repeat_indicator, NavigationStatus nav_status);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit PositionReportClassA(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~PositionReportClassA() override = default;
 
     /**
      * @brief Get the message type
      * @return Message type (1, 2, or 3)
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
      * @brief Get the navigation status
      * @return Navigation status
      */
     NavigationStatus get_navigation_status() const;
 
     /**
      * @brief Get the rate of turn (ROT)
      * @return Rate of turn in degrees per minute (positive = right, negative = left, 
      *         -128 = not available, -127 = turning left at more than 5 deg/30sec,
      *         127 = turning right at more than 5 deg/30sec)
      */
     int8_t get_rate_of_turn_raw() const;
 
     /**
      * @brief Get the rate of turn (ROT) in degrees per minute
      * @return Rate of turn in degrees per minute (positive = right, negative = left, 
      *         NaN = not available or no turn indicator available)
      */
     float get_rate_of_turn() const;
 
     /**
      * @brief Get the speed over ground (SOG)
      * @return Speed over ground in knots (0-102.2, 102.3 = >= 102.3, 
      *         1022 = not available, 1023 = not defined)
      */
     float get_speed_over_ground() const;
 
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
      * @brief Get the course over ground (COG)
      * @return Course over ground in degrees (0-359.9, 360 = not available)
      */
     float get_course_over_ground() const;
 
     /**
      * @brief Get the true heading
      * @return True heading in degrees (0-359, 511 = not available)
      */
     uint16_t get_true_heading() const;
 
     /**
      * @brief Get the timestamp
      * @return UTC second when the report was generated (0-59, 60 = timestamp not available,
      *         61 = positioning system in manual input mode, 62 = electronic position fixing system in estimated mode,
      *         63 = positioning system is inoperative)
      */
     uint8_t get_timestamp() const;
 
     /**
      * @brief Get the special maneuver indicator
      * @return Special maneuver indicator (0 = not available, 1 = not engaged in special maneuver,
      *         2 = engaged in special maneuver)
      */
     uint8_t get_special_maneuver_indicator() const;
 
     /**
      * @brief Get the RAIM flag
      * @return Receiver Autonomous Integrity Monitoring flag (true = RAIM in use, false = RAIM not in use)
      */
     bool get_raim_flag() const;
 
     /**
      * @brief Set the navigation status
      * @param status Navigation status
      */
     void set_navigation_status(NavigationStatus status);
 
     /**
      * @brief Set the rate of turn (ROT) using raw value
      * @param rot Rate of turn raw value
      */
     void set_rate_of_turn_raw(int8_t rot);
 
     /**
      * @brief Set the rate of turn (ROT) in degrees per minute
      * @param rot_deg_min Rate of turn in degrees per minute (positive = right, negative = left)
      */
     void set_rate_of_turn(float rot_deg_min);
 
     /**
      * @brief Set the speed over ground (SOG)
      * @param sog Speed over ground in knots
      */
     void set_speed_over_ground(float sog);
 
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
      * @brief Set the course over ground (COG)
      * @param cog Course over ground in degrees (0-359.9)
      */
     void set_course_over_ground(float cog);
 
     /**
      * @brief Set the true heading
      * @param heading True heading in degrees (0-359, 511 = not available)
      */
     void set_true_heading(uint16_t heading);
 
     /**
      * @brief Set the timestamp
      * @param timestamp UTC second when the report was generated (0-59, 60-63 = special values)
      */
     void set_timestamp(uint8_t timestamp);
 
     /**
      * @brief Set the special maneuver indicator
      * @param indicator Special maneuver indicator (0-2)
      */
     void set_special_maneuver_indicator(uint8_t indicator);
 
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
     uint8_t message_type_;        // 6 bits: 1, 2, or 3
     uint32_t mmsi_;               // 30 bits: MMSI number
     uint8_t repeat_indicator_;    // 2 bits: Repeat indicator
     NavigationStatus nav_status_; // 4 bits: Navigation status
     int8_t rot_;                  // 8 bits: Rate of turn
     uint16_t sog_;                // 10 bits: Speed over ground (1/10 knot resolution)
     bool position_accuracy_;      // 1 bit: Position accuracy
     int32_t longitude_;           // 28 bits: Longitude (1/10000 minute resolution)
     int32_t latitude_;            // 27 bits: Latitude (1/10000 minute resolution)
     uint16_t cog_;                // 12 bits: Course over ground (1/10 degree resolution)
     uint16_t true_heading_;       // 9 bits: True heading
     uint8_t timestamp_;           // 6 bits: UTC second
     uint8_t special_maneuver_;    // 2 bits: Special maneuver indicator
     uint8_t spare_;               // 3 bits: Spare
     bool raim_flag_;              // 1 bit: RAIM flag
     uint32_t radio_status_;       // 19 bits: Radio status
 };
 
 } // namespace aislib
 
 #endif // AISLIB_POSITION_REPORT_CLASS_A_H