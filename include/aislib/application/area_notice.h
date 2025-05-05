/**
 * @file area_notice.h
 * @brief Area Notice Message (DAC=1, FI=22)
 * 
 * This file defines the Area Notice message format (DAC=1, FI=22) used to
 * provide information about specific geographic areas, such as warnings,
 * restrictions, or other notices.
 * 
 * Area Notices consist of a notice type, validity period, and one or more
 * subareas that define the geographical extent of the notice. Subareas
 * can be defined using various shapes, including circles, rectangles,
 * sectors, polylines, polygons, and text.
 * 
 * Area Notices are typically broadcast using Binary Broadcast Messages (Type 8),
 * but can also be sent as Binary Addressed Messages (Type 6) when directed
 * to a specific recipient.
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

 #ifndef AISLIB_APPLICATION_AREA_NOTICE_H
 #define AISLIB_APPLICATION_AREA_NOTICE_H
 
 #include "aislib/binary_message.h"
 #include "aislib/binary_addressed_message.h"
 #include "aislib/binary_broadcast_message.h"
 #include "aislib/binary_application_ids.h"
 #include <vector>
 #include <chrono>
 #include <string>
 #include <cstring>
 #include <algorithm>
 
 namespace aislib {
 namespace application {
 
 /**
  * @class AreaNotice
  * @brief Class for Area Notice messages (DAC=1, FI=22)
  * 
  * Area Notices provide information about specific geographic areas,
  * such as warnings, restrictions, etc.
  */
 class AreaNotice {
 public:
     /**
      * @brief Area notice types
      */
     enum class NoticeType : uint8_t {
         CAUTION = 0,                   ///< Caution area: Marine mammals
         CAUTION_MARINE_MAMMALS = 1,    ///< Caution area: Marine mammals
         CAUTION_SEA_BIRDS = 2,         ///< Caution area: Sea birds
         CAUTION_FISH = 3,              ///< Caution area: Fish
         CAUTION_DIVING = 4,            ///< Caution area: Diving operations
         CAUTION_HIGH_SPEED_AREA = 5,   ///< Caution area: High speed area
         WARNING_STORM = 6,             ///< Warning: Storm
         CAUTION_FISHING_GEAR = 7,      ///< Caution: Fishing gear
         CAUTION_TOW = 8,               ///< Caution: Tow
         WARNING_ICE = 9,               ///< Warning: Ice
         CAUTION_TIDE = 10,             ///< Caution: Tide
         CAUTION_CURRENT = 11,          ///< Caution: Current
         CAUTION_OBSTRUCTION = 12,      ///< Caution: Obstruction
         CAUTION_CONSTRUCTION = 13,     ///< Caution: Construction
         CAUTION_ROCK = 14,             ///< Caution: Rock
         CAUTION_WATERWAY_BLOCKED = 15, ///< Caution: Waterway blocked
         CAUTION_ICING = 16,            ///< Caution: Icing
         CAUTION_WATER_LEVEL = 17,      ///< Caution: Water level
         EXERCISE_WARNING = 18,         ///< Exercise warning
         SPECIAL_PROTECTION = 19,       ///< Caution: Special protection area
         SECURITY_ZONE = 20,            ///< Caution: Security zone
         NO_ANCHORING = 21,             ///< Caution: No anchoring
         DEEP_DRAUGHT = 22,             ///< Caution: Deep draught vessel
         HABITATION = 23,               ///< Information: Habitation
         FISHING = 24,                  ///< Information: Fishing
         ENVIRONMENTAL = 25,            ///< Caution: Environmental
         DISCOLOURED_WATER = 26,        ///< Caution: Discoloured water
         DANGEROUS_CARGO = 27,          ///< Caution: Dangerous cargo
         MARITIME_PILOT = 28,           ///< Caution: Maritime pilot
         FERRY_CROSSING = 29,           ///< Caution: Ferry crossing
         BRIDGE_CLEARANCE = 30,         ///< Warning: Bridge clearance
         SUBMERGED_CABLE = 31,          ///< Warning: Submerged cable
         STRONG_CURRENT = 32,           ///< Warning: Strong current
         SUBMERGED_OBJECT = 33,         ///< Warning: Submerged object
         RESTRICTED_AREA = 34,          ///< Caution: Restricted area
         DANGER_AREA = 35,              ///< Warning: Danger area
         MILITARY_EXERCISES = 36,       ///< Warning: Military exercises
         UNDERWATER_OPERATIONS = 37,    ///< Warning: Underwater operations
         SEAPLANE_OPERATIONS = 38,      ///< Warning: Seaplane operations
         RECREATIONAL_CRAFT = 39,       ///< Warning: Recreational craft
         HIGH_SPEED_CRAFT = 40,         ///< Warning: High speed craft
         VESSEL_TRAFFIC = 41,           ///< Warning: Vessel traffic
         SALVAGE_OPERATIONS = 42,       ///< Warning: Salvage operations
         DREDGING_OPERATIONS = 43,      ///< Warning: Dredging operations
         SURVEY_OPERATIONS = 44,        ///< Warning: Survey operations
         POLLUTION_RESPONSE = 45        ///< Warning: Pollution response
     };
 
     /**
      * @brief Area shape types
      */
     enum class AreaShape : uint8_t {
         CIRCLE = 0,         ///< Circle
         RECTANGLE = 1,      ///< Rectangle
         SECTOR = 2,         ///< Sector
         POLYLINE = 3,       ///< Polyline
         POLYGON = 4,        ///< Polygon
         TEXT = 5,           ///< Text
         RESERVED_6 = 6,     ///< Reserved
         RESERVED_7 = 7      ///< Reserved
     };
 
     /**
      * @struct SubArea
      * @brief Structure for representing a subarea within an area notice
      */
     struct SubArea {
         AreaShape shape_type;           ///< Shape type
         int32_t longitude;              ///< Longitude (in 1/10000 minutes)
         int32_t latitude;               ///< Latitude (in 1/10000 minutes)
         
         // Union for shape-specific parameters
         union {
             struct {                    ///< For circle
                 uint16_t radius;        ///< Radius in meters (0-32767)
                 uint16_t spare;         ///< Spare
             } circle;
             
             struct {                    ///< For rectangle
                 uint16_t e_dimension;   ///< E dimension in meters (0-32767)
                 uint16_t n_dimension;   ///< N dimension in meters (0-32767)
                 uint16_t orientation;   ///< Orientation in degrees (0-359)
                 uint16_t spare;         ///< Spare
             } rectangle;
             
             struct {                    ///< For sector
                 uint16_t radius;        ///< Radius in meters (0-32767)
                 uint16_t left_bound;    ///< Left bound in degrees (0-359)
                 uint16_t right_bound;   ///< Right bound in degrees (0-359)
                 uint16_t spare;         ///< Spare
             } sector;
             
             struct {                    ///< For polyline/polygon
                 int32_t angles[4];      ///< Angles (longitude, latitude pairs)
                 uint8_t spare;          ///< Spare
             } points;
             
             struct {                    ///< For text
                 char text[15];          ///< Text (14 characters + null terminator)
                 uint8_t spare;          ///< Spare
             } text;
         } params;
 
         /**
          * @brief Constructor for circle
          * @param lon Longitude (in degrees)
          * @param lat Latitude (in degrees)
          * @param radius Radius in meters
          */
         static SubArea Circle(double lon, double lat, uint16_t radius);
 
         /**
          * @brief Constructor for rectangle
          * @param lon Longitude (in degrees)
          * @param lat Latitude (in degrees)
          * @param e_dim E dimension in meters
          * @param n_dim N dimension in meters
          * @param orient Orientation in degrees
          */
         static SubArea Rectangle(double lon, double lat, uint16_t e_dim, uint16_t n_dim, uint16_t orient);
 
         /**
          * @brief Constructor for sector
          * @param lon Longitude (in degrees)
          * @param lat Latitude (in degrees)
          * @param radius Radius in meters
          * @param left_bound Left bound in degrees
          * @param right_bound Right bound in degrees
          */
         static SubArea Sector(double lon, double lat, uint16_t radius, uint16_t left_bound, uint16_t right_bound);
 
         /**
          * @brief Constructor for text
          * @param lon Longitude (in degrees)
          * @param lat Latitude (in degrees)
          * @param text Text message
          */
         static SubArea Text(double lon, double lat, const std::string& text);
     };
 
     /**
      * @brief Constructor
      * @param message_type Message version (1 for current version)
      * @param notice_type Type of notice
      * @param start_time Start time of the notice validity
      * @param duration Duration in minutes (0 = unlimited)
      * @param subareas List of subareas that make up the notice
      */
     AreaNotice(
         uint8_t message_version,
         NoticeType notice_type,
         const std::chrono::system_clock::time_point& start_time,
         uint16_t duration,
         const std::vector<SubArea>& subareas
     );
 
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the binary data part of the AIS message
      */
     explicit AreaNotice(const BitVector& bits);
 
     /**
      * @brief Get the message version
      * @return Message version
      */
     uint8_t get_message_version() const;
 
     /**
      * @brief Get the notice type
      * @return Notice type
      */
     NoticeType get_notice_type() const;
 
     /**
      * @brief Get the start time of the notice validity
      * @return Start time
      */
     std::chrono::system_clock::time_point get_start_time() const;
 
     /**
      * @brief Get the duration in minutes
      * @return Duration (0 = unlimited)
      */
     uint16_t get_duration() const;
 
     /**
      * @brief Get the subareas that make up the notice
      * @return List of subareas
      */
     const std::vector<SubArea>& get_subareas() const;
 
     /**
      * @brief Set the message version
      * @param version Message version
      */
     void set_message_version(uint8_t version);
 
     /**
      * @brief Set the notice type
      * @param type Notice type
      */
     void set_notice_type(NoticeType type);
 
     /**
      * @brief Set the start time of the notice validity
      * @param time Start time
      */
     void set_start_time(const std::chrono::system_clock::time_point& time);
 
     /**
      * @brief Set the duration in minutes
      * @param duration Duration (0 = unlimited)
      */
     void set_duration(uint16_t duration);
 
     /**
      * @brief Set the subareas that make up the notice
      * @param subareas List of subareas
      */
     void set_subareas(const std::vector<SubArea>& subareas);
 
     /**
      * @brief Convert to a bit vector
      * @param bits Bit vector to populate
      */
     void to_bits(BitVector& bits) const;
 
     /**
      * @brief Create a bit vector for use in a binary message
      * @return Bit vector containing the binary data
      */
     BitVector to_bit_vector() const;
 
     /**
      * @brief Create a binary addressed message containing this area notice
      * @param source_mmsi Source MMSI number
      * @param dest_mmsi Destination MMSI number
      * @param sequence_number Sequence number
      * @param repeat_indicator Repeat indicator
      * @return Binary addressed message
      */
     BinaryAddressedMessage to_addressed_message(
         uint32_t source_mmsi,
         uint32_t dest_mmsi,
         uint8_t sequence_number,
         uint8_t repeat_indicator
     ) const;
 
     /**
      * @brief Create a binary broadcast message containing this area notice
      * @param source_mmsi Source MMSI number
      * @param repeat_indicator Repeat indicator
      * @return Binary broadcast message
      */
     BinaryBroadcastMessage to_broadcast_message(
         uint32_t source_mmsi,
         uint8_t repeat_indicator
     ) const;
 
     /**
      * @brief Get a string representation
      * @return String representation
      */
     std::string to_string() const;
 
 private:
     uint8_t message_version_;
     NoticeType notice_type_;
     uint32_t start_month_;
     uint32_t start_day_;
     uint32_t start_hour_;
     uint32_t start_minute_;
     uint16_t duration_;
     std::vector<SubArea> subareas_;
 };
 
 } // namespace application
 } // namespace aislib
 
 #endif // AISLIB_APPLICATION_AREA_NOTICE_H