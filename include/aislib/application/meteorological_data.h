/**
 * @file meteorological_data.h
 * @brief Meteorological and Hydrological Data Message (DAC=1, FI=31)
 * 
 * This file defines the Meteorological and Hydrological Data message format
 * (DAC=1, FI=31) used to provide weather and water condition information
 * for a specific location.
 * 
 * Meteorological and Hydrological Data messages can include various parameters
 * such as wind speed and direction, air temperature, humidity, air pressure,
 * visibility, water level, current speed and direction, wave height and period,
 * sea temperature, precipitation, salinity, and ice conditions.
 * 
 * These messages are typically broadcast using Binary Broadcast Messages (Type 8),
 * but can also be sent as Binary Addressed Messages (Type 6) when directed
 * to a specific recipient.
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

 #ifndef AISLIB_APPLICATION_METEOROLOGICAL_DATA_H
 #define AISLIB_APPLICATION_METEOROLOGICAL_DATA_H
 
 #include "aislib/binary_message.h"
 #include "aislib/binary_addressed_message.h"
 #include "aislib/binary_broadcast_message.h"
 #include "aislib/binary_application_ids.h"
 #include <cstdint>
 #include <string>
 #include <chrono>
 #include <cstring>
 #include <algorithm>
 
 namespace aislib {
 namespace application {
 
 /**
  * @class MeteorologicalData
  * @brief Class for Meteorological and Hydrological data messages (DAC=1, FI=31)
  * 
  * Meteorological and Hydrological data messages provide weather and water
  * condition information for a specific location.
  */
 class MeteorologicalData {
 public:
     /**
      * @brief Constructor
      * @param lat Latitude in 1/1000 minutes
      * @param lon Longitude in 1/1000 minutes
      * @param timestamp Timestamp of the observation
      */
     MeteorologicalData(
         int32_t lat,
         int32_t lon,
         const std::chrono::system_clock::time_point& timestamp
     );
 
     /**
      * @brief Constructor from binary data
      * @param bits BitVector containing the binary data part of the AIS message
      */
     explicit MeteorologicalData(const BitVector& bits);
 
     /**
      * @brief Get the latitude
      * @return Latitude in degrees
      */
     double get_latitude() const;
 
     /**
      * @brief Get the longitude
      * @return Longitude in degrees
      */
     double get_longitude() const;
 
     /**
      * @brief Get the timestamp of the observation
      * @return Timestamp
      */
     std::chrono::system_clock::time_point get_timestamp() const;
 
     /**
      * @brief Get the wind speed
      * @return Wind speed in knots (0.1 knot resolution), -1 if not available
      */
     float get_wind_speed() const;
 
     /**
      * @brief Get the wind gust
      * @return Wind gust in knots (0.1 knot resolution), -1 if not available
      */
     float get_wind_gust() const;
 
     /**
      * @brief Get the wind direction
      * @return Wind direction in degrees (0-359), -1 if not available
      */
     int16_t get_wind_direction() const;
 
     /**
      * @brief Get the air temperature
      * @return Air temperature in degrees Celsius (0.1 degree resolution), -1024 if not available
      */
     float get_air_temperature() const;
 
     /**
      * @brief Get the relative humidity
      * @return Relative humidity in percent (0-100), -1 if not available
      */
     int8_t get_relative_humidity() const;
 
     /**
      * @brief Get the dew point
      * @return Dew point in degrees Celsius (0.1 degree resolution), -1024 if not available
      */
     float get_dew_point() const;
 
     /**
      * @brief Get the air pressure
      * @return Air pressure in hPa, -1 if not available
      */
     int16_t get_air_pressure() const;
 
     /**
      * @brief Get the air pressure tendency
      * @return Air pressure tendency (0=steady, 1=decreasing, 2=increasing), -1 if not available
      */
     int8_t get_air_pressure_tendency() const;
 
     /**
      * @brief Get the horizontal visibility
      * @return Horizontal visibility in nautical miles (0.1 NM resolution), -1 if not available
      */
     float get_horizontal_visibility() const;
 
     /**
      * @brief Get the water level
      * @return Water level in meters (0.01 meter resolution), -327.68 if not available
      */
     float get_water_level() const;
 
     /**
      * @brief Get the water level trend
      * @return Water level trend (0=steady, 1=decreasing, 2=increasing), -1 if not available
      */
     int8_t get_water_level_trend() const;
 
     /**
      * @brief Get the surface current speed
      * @return Surface current speed in knots (0.1 knot resolution), -1 if not available
      */
     float get_surface_current_speed() const;
 
     /**
      * @brief Get the surface current direction
      * @return Surface current direction in degrees (0-359), -1 if not available
      */
     int16_t get_surface_current_direction() const;
 
     /**
      * @brief Get the wave height
      * @return Wave height in meters (0.1 meter resolution), -1 if not available
      */
     float get_wave_height() const;
 
     /**
      * @brief Get the wave period
      * @return Wave period in seconds, -1 if not available
      */
     int8_t get_wave_period() const;
 
     /**
      * @brief Get the wave direction
      * @return Wave direction in degrees (0-359), -1 if not available
      */
     int16_t get_wave_direction() const;
 
     /**
      * @brief Get the swell height
      * @return Swell height in meters (0.1 meter resolution), -1 if not available
      */
     float get_swell_height() const;
 
     /**
      * @brief Get the swell period
      * @return Swell period in seconds, -1 if not available
      */
     int8_t get_swell_period() const;
 
     /**
      * @brief Get the swell direction
      * @return Swell direction in degrees (0-359), -1 if not available
      */
     int16_t get_swell_direction() const;
 
     /**
      * @brief Get the sea temperature
      * @return Sea temperature in degrees Celsius (0.1 degree resolution), -1024 if not available
      */
     float get_sea_temperature() const;
 
     /**
      * @brief Get the precipitation type
      * @return Precipitation type (0=reserved, 1=rain, 2=thunderstorm, 3=freezing rain,
      *         4=mixed/ice, 5=snow, 6=reserved, 7=not available)
      */
     int8_t get_precipitation_type() const;
 
     /**
      * @brief Get the salinity
      * @return Salinity in parts per thousand (0.1 PPT resolution), -1 if not available
      */
     float get_salinity() const;
 
     /**
      * @brief Get the ice
      * @return Ice (0=no, 1=yes, 2=reserved, 3=not available)
      */
     int8_t get_ice() const;
 
     /**
      * @brief Set the latitude
      * @param lat Latitude in degrees
      */
     void set_latitude(double lat);
 
     /**
      * @brief Set the longitude
      * @param lon Longitude in degrees
      */
     void set_longitude(double lon);
 
     /**
      * @brief Set the timestamp of the observation
      * @param timestamp Timestamp
      */
     void set_timestamp(const std::chrono::system_clock::time_point& timestamp);
 
     /**
      * @brief Set the wind speed
      * @param speed Wind speed in knots
      */
     void set_wind_speed(float speed);
 
     /**
      * @brief Set the wind gust
      * @param gust Wind gust in knots
      */
     void set_wind_gust(float gust);
 
     /**
      * @brief Set the wind direction
      * @param direction Wind direction in degrees (0-359)
      */
     void set_wind_direction(int16_t direction);
 
     /**
      * @brief Set the air temperature
      * @param temperature Air temperature in degrees Celsius
      */
     void set_air_temperature(float temperature);
 
     /**
      * @brief Set the relative humidity
      * @param humidity Relative humidity in percent (0-100)
      */
     void set_relative_humidity(int8_t humidity);
 
     /**
      * @brief Set the dew point
      * @param dew_point Dew point in degrees Celsius
      */
     void set_dew_point(float dew_point);
 
     /**
      * @brief Set the air pressure
      * @param pressure Air pressure in hPa
      */
     void set_air_pressure(int16_t pressure);
 
     /**
      * @brief Set the air pressure tendency
      * @param tendency Air pressure tendency (0=steady, 1=decreasing, 2=increasing)
      */
     void set_air_pressure_tendency(int8_t tendency);
 
     /**
      * @brief Set the horizontal visibility
      * @param visibility Horizontal visibility in nautical miles
      */
     void set_horizontal_visibility(float visibility);
 
     /**
      * @brief Set the water level
      * @param level Water level in meters
      */
     void set_water_level(float level);
 
     /**
      * @brief Set the water level trend
      * @param trend Water level trend (0=steady, 1=decreasing, 2=increasing)
      */
     void set_water_level_trend(int8_t trend);
 
     /**
      * @brief Set the surface current speed
      * @param speed Surface current speed in knots
      */
     void set_surface_current_speed(float speed);
 
     /**
      * @brief Set the surface current direction
      * @param direction Surface current direction in degrees (0-359)
      */
     void set_surface_current_direction(int16_t direction);
 
     /**
      * @brief Set the wave height
      * @param height Wave height in meters
      */
     void set_wave_height(float height);
 
     /**
      * @brief Set the wave period
      * @param period Wave period in seconds
      */
     void set_wave_period(int8_t period);
 
     /**
      * @brief Set the wave direction
      * @param direction Wave direction in degrees (0-359)
      */
     void set_wave_direction(int16_t direction);
 
     /**
      * @brief Set the swell height
      * @param height Swell height in meters
      */
     void set_swell_height(float height);
 
     /**
      * @brief Set the swell period
      * @param period Swell period in seconds
      */
     void set_swell_period(int8_t period);
 
     /**
      * @brief Set the swell direction
      * @param direction Swell direction in degrees (0-359)
      */
     void set_swell_direction(int16_t direction);
 
     /**
      * @brief Set the sea temperature
      * @param temperature Sea temperature in degrees Celsius
      */
     void set_sea_temperature(float temperature);
 
     /**
      * @brief Set the precipitation type
      * @param type Precipitation type (0=reserved, 1=rain, 2=thunderstorm, 3=freezing rain,
      *        4=mixed/ice, 5=snow, 6=reserved)
      */
     void set_precipitation_type(int8_t type);
 
     /**
      * @brief Set the salinity
      * @param salinity Salinity in parts per thousand
      */
     void set_salinity(float salinity);
 
     /**
      * @brief Set the ice
      * @param ice Ice (0=no, 1=yes, 2=reserved)
      */
     void set_ice(int8_t ice);
 
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
      * @brief Create a binary addressed message containing this meteorological data
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
      * @brief Create a binary broadcast message containing this meteorological data
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
     // Required fields
     int32_t latitude_;              // In 1/1000 minutes
     int32_t longitude_;             // In 1/1000 minutes
     uint8_t day_;                   // Day (1-31)
     uint8_t hour_;                  // Hour (0-23)
     uint8_t minute_;                // Minute (0-59)
     
     // Optional fields with their availability flags
     bool wind_speed_available_;
     uint16_t wind_speed_;           // In 0.1 knots
     
     bool wind_gust_available_;
     uint16_t wind_gust_;            // In 0.1 knots
     
     bool wind_direction_available_;
     uint16_t wind_direction_;       // In degrees (0-359)
     
     bool air_temperature_available_;
     int16_t air_temperature_;       // In 0.1 degrees Celsius
     
     bool relative_humidity_available_;
     uint8_t relative_humidity_;     // In percent (0-100)
     
     bool dew_point_available_;
     int16_t dew_point_;             // In 0.1 degrees Celsius
     
     bool air_pressure_available_;
     uint16_t air_pressure_;         // In hPa
     
     bool air_pressure_tendency_available_;
     uint8_t air_pressure_tendency_;  // 0=steady, 1=decreasing, 2=increasing
     
     bool visibility_available_;
     uint8_t horizontal_visibility_; // In 0.1 nautical miles
     
     bool water_level_available_;
     int16_t water_level_;           // In 0.01 meters
     
     bool water_level_trend_available_;
     uint8_t water_level_trend_;     // 0=steady, 1=decreasing, 2=increasing
     
     bool surface_current_available_;
     uint16_t surface_current_speed_;     // In 0.1 knots
     uint16_t surface_current_direction_; // In degrees (0-359)
     
     bool wave_available_;
     uint8_t wave_height_;           // In 0.1 meters
     uint8_t wave_period_;           // In seconds
     uint16_t wave_direction_;       // In degrees (0-359)
     
     bool swell_available_;
     uint8_t swell_height_;          // In 0.1 meters
     uint8_t swell_period_;          // In seconds
     uint16_t swell_direction_;      // In degrees (0-359)
     
     bool sea_temperature_available_;
     int16_t sea_temperature_;       // In 0.1 degrees Celsius
     
     bool precipitation_available_;
     uint8_t precipitation_type_;    // 0=reserved, 1=rain, 2=thunderstorm, 3=freezing rain,
                                     // 4=mixed/ice, 5=snow, 6=reserved, 7=not available
     
     bool salinity_available_;
     uint16_t salinity_;             // In 0.1 parts per thousand
     
     bool ice_available_;
     uint8_t ice_;                   // 0=no, 1=yes, 2=reserved, 3=not available
 };
 
 } // namespace application
 } // namespace aislib
 
 #endif // AISLIB_APPLICATION_METEOROLOGICAL_DATA_H