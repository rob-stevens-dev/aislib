/**
 * @file static_and_voyage_data.h
 * @brief Static and Voyage Data (Message Type 5)
 * 
 * This file defines the static and voyage data message (Type 5)
 * used by AIS class A transceivers.
 */

 #ifndef AISLIB_STATIC_AND_VOYAGE_DATA_H
 #define AISLIB_STATIC_AND_VOYAGE_DATA_H
 
 #include "aislib/ais_message.h"
 #include <string>
 #include <chrono>
 
 namespace aislib {
 
 /**
  * @class StaticAndVoyageData
  * @brief Class for AIS message type 5 (Static and Voyage Data)
  * 
  * Message type 5 provides static and voyage-related data for vessels
  * equipped with Class A AIS transponders.
  */
 class StaticAndVoyageData : public AISMessage {
 public:
     /**
      * @brief Ship type categories
      */
     enum class ShipType : uint8_t {
         NOT_AVAILABLE = 0,
         RESERVED_1 = 1,
         RESERVED_2 = 2,
         RESERVED_3 = 3,
         RESERVED_4 = 4,
         RESERVED_5 = 5,
         RESERVED_6 = 6,
         RESERVED_7 = 7,
         RESERVED_8 = 8,
         RESERVED_9 = 9,
         RESERVED_10 = 10,
         RESERVED_11 = 11,
         RESERVED_12 = 12,
         RESERVED_13 = 13,
         RESERVED_14 = 14,
         RESERVED_15 = 15,
         RESERVED_16 = 16,
         RESERVED_17 = 17,
         RESERVED_18 = 18,
         RESERVED_19 = 19,
         WING_IN_GROUND = 20,
         WING_IN_GROUND_HAZARDOUS_A = 21,
         WING_IN_GROUND_HAZARDOUS_B = 22,
         WING_IN_GROUND_HAZARDOUS_C = 23,
         WING_IN_GROUND_HAZARDOUS_D = 24,
         WING_IN_GROUND_RESERVED_1 = 25,
         WING_IN_GROUND_RESERVED_2 = 26,
         WING_IN_GROUND_RESERVED_3 = 27,
         WING_IN_GROUND_RESERVED_4 = 28,
         WING_IN_GROUND_RESERVED_5 = 29,
         FISHING = 30,
         TOWING = 31,
         TOWING_LARGE = 32,
         DREDGER = 33,
         DIVING = 34,
         MILITARY = 35,
         SAILING = 36,
         PLEASURE = 37,
         RESERVED_38 = 38,
         RESERVED_39 = 39,
         HIGH_SPEED_CRAFT = 40,
         HIGH_SPEED_CRAFT_HAZARDOUS_A = 41,
         HIGH_SPEED_CRAFT_HAZARDOUS_B = 42,
         HIGH_SPEED_CRAFT_HAZARDOUS_C = 43,
         HIGH_SPEED_CRAFT_HAZARDOUS_D = 44,
         HIGH_SPEED_CRAFT_RESERVED_1 = 45,
         HIGH_SPEED_CRAFT_RESERVED_2 = 46,
         HIGH_SPEED_CRAFT_RESERVED_3 = 47,
         HIGH_SPEED_CRAFT_RESERVED_4 = 48,
         HIGH_SPEED_CRAFT_RESERVED_5 = 49,
         PILOT = 50,
         SEARCH_AND_RESCUE = 51,
         TUG = 52,
         PORT_TENDER = 53,
         ANTI_POLLUTION = 54,
         LAW_ENFORCEMENT = 55,
         SPARE_LOCAL_1 = 56,
         SPARE_LOCAL_2 = 57,
         MEDICAL = 58,
         PASSENGER_SHIP = 60,
         PASSENGER_SHIP_HAZARDOUS_A = 61,
         PASSENGER_SHIP_HAZARDOUS_B = 62,
         PASSENGER_SHIP_HAZARDOUS_C = 63,
         PASSENGER_SHIP_HAZARDOUS_D = 64,
         PASSENGER_SHIP_RESERVED_1 = 65,
         PASSENGER_SHIP_RESERVED_2 = 66,
         PASSENGER_SHIP_RESERVED_3 = 67,
         PASSENGER_SHIP_RESERVED_4 = 68,
         PASSENGER_SHIP_RESERVED_5 = 69,
         CARGO = 70,
         CARGO_HAZARDOUS_A = 71,
         CARGO_HAZARDOUS_B = 72,
         CARGO_HAZARDOUS_C = 73,
         CARGO_HAZARDOUS_D = 74,
         CARGO_RESERVED_1 = 75,
         CARGO_RESERVED_2 = 76,
         CARGO_RESERVED_3 = 77,
         CARGO_RESERVED_4 = 78,
         CARGO_RESERVED_5 = 79,
         TANKER = 80,
         TANKER_HAZARDOUS_A = 81,
         TANKER_HAZARDOUS_B = 82,
         TANKER_HAZARDOUS_C = 83,
         TANKER_HAZARDOUS_D = 84,
         TANKER_RESERVED_1 = 85,
         TANKER_RESERVED_2 = 86,
         TANKER_RESERVED_3 = 87,
         TANKER_RESERVED_4 = 88,
         TANKER_RESERVED_5 = 89,
         OTHER = 90,
         OTHER_HAZARDOUS_A = 91,
         OTHER_HAZARDOUS_B = 92,
         OTHER_HAZARDOUS_C = 93,
         OTHER_HAZARDOUS_D = 94,
         OTHER_RESERVED_1 = 95,
         OTHER_RESERVED_2 = 96,
         OTHER_RESERVED_3 = 97,
         OTHER_RESERVED_4 = 98,
         OTHER_RESERVED_5 = 99
     };
 
     /**
      * @brief Constructor
      * @param mmsi MMSI number
      * @param repeat_indicator Repeat indicator
      */
     StaticAndVoyageData(uint32_t mmsi, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit StaticAndVoyageData(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~StaticAndVoyageData() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (5)
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
      * @brief Get the AIS version
      * @return AIS version (0-3)
      */
     uint8_t get_ais_version() const;
 
     /**
      * @brief Get the IMO number
      * @return IMO number
      */
     uint32_t get_imo_number() const;
 
     /**
      * @brief Get the call sign
      * @return Call sign
      */
     std::string get_call_sign() const;
 
     /**
      * @brief Get the vessel name
      * @return Vessel name
      */
     std::string get_vessel_name() const;
 
     /**
      * @brief Get the ship type
      * @return Ship type
      */
     ShipType get_ship_type() const;
 
     /**
      * @brief Get the dimension to bow
      * @return Dimension to bow in meters
      */
     uint16_t get_dimension_to_bow() const;
 
     /**
      * @brief Get the dimension to stern
      * @return Dimension to stern in meters
      */
     uint16_t get_dimension_to_stern() const;
 
     /**
      * @brief Get the dimension to port
      * @return Dimension to port in meters
      */
     uint8_t get_dimension_to_port() const;
 
     /**
      * @brief Get the dimension to starboard
      * @return Dimension to starboard in meters
      */
     uint8_t get_dimension_to_starboard() const;
 
     /**
      * @brief Get the type of electronic position fixing device
      * @return EPFD type
      */
     uint8_t get_epfd_type() const;
 
     /**
      * @brief Get the ETA month
      * @return ETA month (1-12, 0 = not available)
      */
     uint8_t get_eta_month() const;
 
     /**
      * @brief Get the ETA day
      * @return ETA day (1-31, 0 = not available)
      */
     uint8_t get_eta_day() const;
 
     /**
      * @brief Get the ETA hour
      * @return ETA hour (0-23, 24 = not available)
      */
     uint8_t get_eta_hour() const;
 
     /**
      * @brief Get the ETA minute
      * @return ETA minute (0-59, 60 = not available)
      */
     uint8_t get_eta_minute() const;
 
     /**
      * @brief Get the ETA as std::chrono::system_clock::time_point
      * @return ETA timestamp
      */
     std::chrono::system_clock::time_point get_eta() const;
 
     /**
      * @brief Get the maximum static draught
      * @return Maximum static draught in meters (0.1 meter resolution)
      */
     float get_draught() const;
 
     /**
      * @brief Get the destination
      * @return Destination
      */
     std::string get_destination() const;
 
     /**
      * @brief Get the DTE flag
      * @return Data terminal equipment (DTE) flag
      */
     bool get_dte_flag() const;
 
     /**
      * @brief Set the AIS version
      * @param version AIS version (0-3)
      */
     void set_ais_version(uint8_t version);
 
     /**
      * @brief Set the IMO number
      * @param imo IMO number
      */
     void set_imo_number(uint32_t imo);
 
     /**
      * @brief Set the call sign
      * @param call_sign Call sign
      */
     void set_call_sign(const std::string& call_sign);
 
     /**
      * @brief Set the vessel name
      * @param name Vessel name
      */
     void set_vessel_name(const std::string& name);
 
     /**
      * @brief Set the ship type
      * @param type Ship type
      */
     void set_ship_type(ShipType type);
 
     /**
      * @brief Set the ship dimensions
      * @param to_bow Dimension to bow in meters
      * @param to_stern Dimension to stern in meters
      * @param to_port Dimension to port in meters
      * @param to_starboard Dimension to starboard in meters
      */
     void set_ship_dimensions(uint16_t to_bow, uint16_t to_stern, uint8_t to_port, uint8_t to_starboard);
 
     /**
      * @brief Set the type of electronic position fixing device
      * @param type EPFD type
      */
     void set_epfd_type(uint8_t type);
 
     /**
      * @brief Set the ETA
      * @param eta ETA timestamp
      */
     void set_eta(const std::chrono::system_clock::time_point& eta);
 
     /**
      * @brief Set the ETA components
      * @param month ETA month (1-12, 0 = not available)
      * @param day ETA day (1-31, 0 = not available)
      * @param hour ETA hour (0-23, 24 = not available)
      * @param minute ETA minute (0-59, 60 = not available)
      */
     void set_eta_components(uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
 
     /**
      * @brief Set the maximum static draught
      * @param draught Maximum static draught in meters
      */
     void set_draught(float draught);
 
     /**
      * @brief Set the destination
      * @param destination Destination
      */
     void set_destination(const std::string& destination);
 
     /**
      * @brief Set the DTE flag
      * @param dte Data terminal equipment (DTE) flag
      */
     void set_dte_flag(bool dte);
 
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
     uint32_t mmsi_;
     uint8_t repeat_indicator_;
     uint8_t ais_version_;
     uint32_t imo_number_;
     std::string call_sign_;
     std::string vessel_name_;
     ShipType ship_type_;
     uint16_t dimension_to_bow_;
     uint16_t dimension_to_stern_;
     uint8_t dimension_to_port_;
     uint8_t dimension_to_starboard_;
     uint8_t epfd_type_;
     uint8_t eta_month_;
     uint8_t eta_day_;
     uint8_t eta_hour_;
     uint8_t eta_minute_;
     uint8_t draught_; // in 0.1 meter steps
     std::string destination_;
     bool dte_flag_;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_STATIC_AND_VOYAGE_DATA_H