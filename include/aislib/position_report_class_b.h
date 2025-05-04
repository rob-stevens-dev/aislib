/**
 * @file position_report_class_b.h
 * @brief Position Report for Class B AIS transponders
 * 
 * This file defines message types 18 and 19 (Standard and Extended
 * Position Reports for Class B AIS transponders).
 */

 #ifndef AISLIB_POSITION_REPORT_CLASS_B_H
 #define AISLIB_POSITION_REPORT_CLASS_B_H
 
 #include "ais_message.h"
 #include <string>
 #include <cmath>
 
 namespace aislib {
 
 /**
  * @class StandardPositionReportClassB
  * @brief Class for AIS message type 18 (Standard Position Report for Class B Equipment)
  * 
  * Message type 18 provides standard position and navigational information
  * for vessels equipped with Class B AIS transponders.
  */
 class StandardPositionReportClassB : public AISMessage {
 public:
     /**
      * @brief Constructor
      * @param mmsi MMSI number
      * @param repeat_indicator Repeat indicator
      */
     StandardPositionReportClassB(uint32_t mmsi, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit StandardPositionReportClassB(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~StandardPositionReportClassB() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (18)
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
      * @brief Get the speed over ground
      * @return Speed over ground in knots (0.1 knot resolution)
      */
     float get_speed_over_ground() const;
 
     /**
      * @brief Get the position accuracy
      * @return Position accuracy (true = high, false = low/GNSS > 10m)
      */
     bool get_position_accuracy() const;
 
     /**
      * @brief Get the longitude
      * @return Longitude in degrees (-180 to 180)
      */
     double get_longitude() const;
 
     /**
      * @brief Get the latitude
      * @return Latitude in degrees (-90 to 90)
      */
     double get_latitude() const;
 
     /**
      * @brief Get the course over ground
      * @return Course over ground in degrees (0.1 degree resolution)
      */
     float get_course_over_ground() const;
 
     /**
      * @brief Get the true heading
      * @return True heading in degrees (0-359)
      */
     uint16_t get_true_heading() const;
 
     /**
      * @brief Get the timestamp
      * @return UTC second when the report was generated (0-59)
      */
     uint8_t get_timestamp() const;
 
     /**
      * @brief Get the CS flag
      * @return Class B CS flag (true = SOTDMA, false = ITDMA)
      */
     bool get_cs_flag() const;
 
     /**
      * @brief Get the display flag
      * @return Display flag (has display capability)
      */
     bool get_display_flag() const;
 
     /**
      * @brief Get the DSC flag
      * @return DSC flag (has DSC capability)
      */
     bool get_dsc_flag() const;
 
     /**
      * @brief Get the band flag
      * @return Band flag (true = all marine bands, false = single band)
      */
     bool get_band_flag() const;
 
     /**
      * @brief Get the message 22 flag
      * @return Message 22 flag (can handle frequency assignment)
      */
     bool get_message_22_flag() const;
 
     /**
      * @brief Get the assigned flag
      * @return Assigned flag (operating in assigned mode)
      */
     bool get_assigned_flag() const;
 
     /**
      * @brief Get the RAIM flag
      * @return RAIM flag (RAIM in use)
      */
     bool get_raim_flag() const;
 
     /**
      * @brief Get the radio status
      * @return SOTDMA or ITDMA radio status (depends on CS flag)
      */
     uint32_t get_radio_status() const;
 
     /**
      * @brief Set the speed over ground
      * @param sog Speed over ground in knots
      */
     void set_speed_over_ground(float sog);
 
     /**
      * @brief Set the position accuracy
      * @param accuracy Position accuracy (true = high, false = low/GNSS > 10m)
      */
     void set_position_accuracy(bool accuracy);
 
     /**
      * @brief Set the longitude
      * @param longitude Longitude in degrees (-180 to 180)
      */
     void set_longitude(double longitude);
 
     /**
      * @brief Set the latitude
      * @param latitude Latitude in degrees (-90 to 90)
      */
     void set_latitude(double latitude);
 
     /**
      * @brief Set the course over ground
      * @param cog Course over ground in degrees
      */
     void set_course_over_ground(float cog);
 
     /**
      * @brief Set the true heading
      * @param heading True heading in degrees (0-359)
      */
     void set_true_heading(uint16_t heading);
 
     /**
      * @brief Set the timestamp
      * @param timestamp UTC second when the report was generated (0-59)
      */
     void set_timestamp(uint8_t timestamp);
 
     /**
      * @brief Set the CS flag
      * @param cs_flag Class B CS flag (true = SOTDMA, false = ITDMA)
      */
     void set_cs_flag(bool cs_flag);
 
     /**
      * @brief Set the display flag
      * @param display_flag Display flag (has display capability)
      */
     void set_display_flag(bool display_flag);
 
     /**
      * @brief Set the DSC flag
      * @param dsc_flag DSC flag (has DSC capability)
      */
     void set_dsc_flag(bool dsc_flag);
 
     /**
      * @brief Set the band flag
      * @param band_flag Band flag (true = all marine bands, false = single band)
      */
     void set_band_flag(bool band_flag);
 
     /**
      * @brief Set the message 22 flag
      * @param message_22_flag Message 22 flag (can handle frequency assignment)
      */
     void set_message_22_flag(bool message_22_flag);
 
     /**
      * @brief Set the assigned flag
      * @param assigned_flag Assigned flag (operating in assigned mode)
      */
     void set_assigned_flag(bool assigned_flag);
 
     /**
      * @brief Set the RAIM flag
      * @param raim_flag RAIM flag (RAIM in use)
      */
     void set_raim_flag(bool raim_flag);
 
     /**
      * @brief Set the radio status
      * @param radio_status SOTDMA or ITDMA radio status (depends on CS flag)
      */
     void set_radio_status(uint32_t radio_status);
 
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
 
 protected:
     // Make these variables available to derived classes
     uint32_t mmsi_;
     uint8_t repeat_indicator_;
     uint16_t speed_over_ground_; // in 0.1 knot steps
     bool position_accuracy_;
     int32_t longitude_; // in 1/10000 minute steps
     int32_t latitude_;  // in 1/10000 minute steps
     uint16_t course_over_ground_; // in 0.1 degree steps
     uint16_t true_heading_;
     uint8_t timestamp_;
     bool cs_flag_;
     bool display_flag_;
     bool dsc_flag_;
     bool band_flag_;
     bool message_22_flag_;
     bool assigned_flag_;
     bool raim_flag_;
     uint32_t radio_status_;
 };
 
 /**
  * @class ExtendedPositionReportClassB
  * @brief Class for AIS message type 19 (Extended Position Report for Class B Equipment)
  * 
  * Message type 19 provides extended position and vessel information
  * for vessels equipped with Class B AIS transponders.
  */
 class ExtendedPositionReportClassB : public StandardPositionReportClassB {
 public:
     /**
      * @brief Constructor
      * @param mmsi MMSI number
      * @param repeat_indicator Repeat indicator
      */
     ExtendedPositionReportClassB(uint32_t mmsi, uint8_t repeat_indicator);
     
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the message data
      */
     explicit ExtendedPositionReportClassB(const BitVector& bits);
     
     /**
      * @brief Destructor
      */
     ~ExtendedPositionReportClassB() override = default;
     
     /**
      * @brief Get the message type
      * @return Message type (19)
      */
     uint8_t get_message_type() const override;
 
     /**
      * @brief Get the vessel name
      * @return Vessel name
      */
     std::string get_vessel_name() const;
 
     /**
      * @brief Get the ship type
      * @return Ship type (same values as in message type 5)
      */
     uint8_t get_ship_type() const;
 
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
      * @brief Set the vessel name
      * @param name Vessel name
      */
     void set_vessel_name(const std::string& name);
 
     /**
      * @brief Set the ship type
      * @param type Ship type (same values as in message type 5)
      */
     void set_ship_type(uint8_t type);
 
     /**
      * @brief Set the ship dimensions
      * @param to_bow Dimension to bow in meters
      * @param to_stern Dimension to stern in meters
      * @param to_port Dimension to port in meters
      * @param to_starboard Dimension to starboard in meters
      */
     void set_ship_dimensions(
         uint16_t to_bow, 
         uint16_t to_stern, 
         uint8_t to_port, 
         uint8_t to_starboard
     );
 
     /**
      * @brief Set the type of electronic position fixing device
      * @param type EPFD type
      */
     void set_epfd_type(uint8_t type);
 
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
     std::string vessel_name_;
     uint8_t ship_type_;
     uint16_t dimension_to_bow_;
     uint16_t dimension_to_stern_;
     uint8_t dimension_to_port_;
     uint8_t dimension_to_starboard_;
     uint8_t epfd_type_;
 };
 
 } // namespace aislib
 
 #endif // AISLIB_POSITION_REPORT_CLASS_B_H