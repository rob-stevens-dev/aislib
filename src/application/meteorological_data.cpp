/**
 * @file meteorological_data.cpp
 * @brief Implementation of MeteorologicalData class
 */

 #include "aislib/application/meteorological_data.h"
 #include <sstream>
 #include <iomanip>
 #include <cmath>
 
 namespace aislib {
 namespace application {
 
 MeteorologicalData::MeteorologicalData(
     int32_t lat,
     int32_t lon,
     const std::chrono::system_clock::time_point& timestamp
 ) : latitude_(lat),
     longitude_(lon),
     day_(0),
     hour_(0),
     minute_(0),
     wind_speed_available_(false),
     wind_speed_(0),
     wind_gust_available_(false),
     wind_gust_(0),
     wind_direction_available_(false),
     wind_direction_(0),
     air_temperature_available_(false),
     air_temperature_(0),
     relative_humidity_available_(false),
     relative_humidity_(0),
     dew_point_available_(false),
     dew_point_(0),
     air_pressure_available_(false),
     air_pressure_(0),
     air_pressure_tendency_available_(false),
     air_pressure_tendency_(0),
     visibility_available_(false),
     horizontal_visibility_(0),
     water_level_available_(false),
     water_level_(0),
     water_level_trend_available_(false),
     water_level_trend_(0),
     surface_current_available_(false),
     surface_current_speed_(0),
     surface_current_direction_(0),
     wave_available_(false),
     wave_height_(0),
     wave_period_(0),
     wave_direction_(0),
     swell_available_(false),
     swell_height_(0),
     swell_period_(0),
     swell_direction_(0),
     sea_temperature_available_(false),
     sea_temperature_(0),
     precipitation_available_(false),
     precipitation_type_(7), // Not available
     salinity_available_(false),
     salinity_(0),
     ice_available_(false),
     ice_(3) // Not available
 {
     // Parse timestamp
     std::time_t time = std::chrono::system_clock::to_time_t(timestamp);
     std::tm* tm = std::gmtime(&time);
     
     if (tm) {
         day_ = static_cast<uint8_t>(tm->tm_mday);
         hour_ = static_cast<uint8_t>(tm->tm_hour);
         minute_ = static_cast<uint8_t>(tm->tm_min);
     }
 }
 
 MeteorologicalData::MeteorologicalData(const BitVector& bits) {
     // Initialize all availability flags to false
     wind_speed_available_ = false;
     wind_gust_available_ = false;
     wind_direction_available_ = false;
     air_temperature_available_ = false;
     relative_humidity_available_ = false;
     dew_point_available_ = false;
     air_pressure_available_ = false;
     air_pressure_tendency_available_ = false;
     visibility_available_ = false;
     water_level_available_ = false;
     water_level_trend_available_ = false;
     surface_current_available_ = false;
     wave_available_ = false;
     swell_available_ = false;
     sea_temperature_available_ = false;
     precipitation_available_ = false;
     salinity_available_ = false;
     ice_available_ = false;
     
     // Required fields
     latitude_ = static_cast<int32_t>(bits.get_int(0, 24));
     longitude_ = static_cast<int32_t>(bits.get_int(24, 25));
     day_ = static_cast<uint8_t>(bits.get_uint(49, 5));
     hour_ = static_cast<uint8_t>(bits.get_uint(54, 5));
     minute_ = static_cast<uint8_t>(bits.get_uint(59, 6));
     
     // Optional fields
     size_t bit_pos = 65;
     
     // Wind speed
     uint16_t wind_speed = static_cast<uint16_t>(bits.get_uint(bit_pos, 10));
     bit_pos += 10;
     if (wind_speed != 0x3FF) {
         wind_speed_available_ = true;
         wind_speed_ = wind_speed;
     }
     
     // Wind gust
     uint16_t wind_gust = static_cast<uint16_t>(bits.get_uint(bit_pos, 10));
     bit_pos += 10;
     if (wind_gust != 0x3FF) {
         wind_gust_available_ = true;
         wind_gust_ = wind_gust;
     }
     
     // Wind direction
     uint16_t wind_direction = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (wind_direction != 0x1FF) {
         wind_direction_available_ = true;
         wind_direction_ = wind_direction;
     }
     
     // Air temperature
     int16_t air_temperature = static_cast<int16_t>(bits.get_int(bit_pos, 11));
     bit_pos += 11;
     if (air_temperature != -1024) {
         air_temperature_available_ = true;
         air_temperature_ = air_temperature;
     }
     
     // Relative humidity
     uint8_t relative_humidity = static_cast<uint8_t>(bits.get_uint(bit_pos, 7));
     bit_pos += 7;
     if (relative_humidity != 0x7F) {
         relative_humidity_available_ = true;
         relative_humidity_ = relative_humidity;
     }
     
     // Dew point
     int16_t dew_point = static_cast<int16_t>(bits.get_int(bit_pos, 11));
     bit_pos += 11;
     if (dew_point != -1024) {
         dew_point_available_ = true;
         dew_point_ = dew_point;
     }
     
     // Air pressure
     uint16_t air_pressure = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (air_pressure != 0x1FF) {
         air_pressure_available_ = true;
         air_pressure_ = air_pressure;
     }
     
     // Air pressure tendency
     uint8_t air_pressure_tendency = static_cast<uint8_t>(bits.get_uint(bit_pos, 2));
     bit_pos += 2;
     if (air_pressure_tendency != 0x3) {
         air_pressure_tendency_available_ = true;
         air_pressure_tendency_ = air_pressure_tendency;
     }
     
     // Horizontal visibility
     uint8_t horizontal_visibility = static_cast<uint8_t>(bits.get_uint(bit_pos, 8));
     bit_pos += 8;
     if (horizontal_visibility != 0xFF) {
         visibility_available_ = true;
         horizontal_visibility_ = horizontal_visibility;
     }
     
     // Water level
     int16_t water_level = static_cast<int16_t>(bits.get_int(bit_pos, 12));
     bit_pos += 12;
     if (water_level != -2048) {
         water_level_available_ = true;
         water_level_ = water_level;
     }
     
     // Water level trend
     uint8_t water_level_trend = static_cast<uint8_t>(bits.get_uint(bit_pos, 2));
     bit_pos += 2;
     if (water_level_trend != 0x3) {
         water_level_trend_available_ = true;
         water_level_trend_ = water_level_trend;
     }
     
     // Surface current speed
     uint16_t surface_current_speed = static_cast<uint16_t>(bits.get_uint(bit_pos, 8));
     bit_pos += 8;
     if (surface_current_speed != 0xFF) {
         surface_current_available_ = true;
         surface_current_speed_ = surface_current_speed;
     }
     
     // Surface current direction
     uint16_t surface_current_direction = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (surface_current_direction != 0x1FF) {
         surface_current_available_ = true;
         surface_current_direction_ = surface_current_direction;
     }
     
     // Wave height
     uint8_t wave_height = static_cast<uint8_t>(bits.get_uint(bit_pos, 8));
     bit_pos += 8;
     if (wave_height != 0xFF) {
         wave_available_ = true;
         wave_height_ = wave_height;
     }
     
     // Wave period
     uint8_t wave_period = static_cast<uint8_t>(bits.get_uint(bit_pos, 6));
     bit_pos += 6;
     if (wave_period != 0x3F) {
         wave_available_ = true;
         wave_period_ = wave_period;
     }
     
     // Wave direction
     uint16_t wave_direction = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (wave_direction != 0x1FF) {
         wave_available_ = true;
         wave_direction_ = wave_direction;
     }
     
     // Swell height
     uint8_t swell_height = static_cast<uint8_t>(bits.get_uint(bit_pos, 8));
     bit_pos += 8;
     if (swell_height != 0xFF) {
         swell_available_ = true;
         swell_height_ = swell_height;
     }
     
     // Swell period
     uint8_t swell_period = static_cast<uint8_t>(bits.get_uint(bit_pos, 6));
     bit_pos += 6;
     if (swell_period != 0x3F) {
         swell_available_ = true;
         swell_period_ = swell_period;
     }
     
     // Swell direction
     uint16_t swell_direction = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (swell_direction != 0x1FF) {
         swell_available_ = true;
         swell_direction_ = swell_direction;
     }
     
     // Sea temperature
     int16_t sea_temperature = static_cast<int16_t>(bits.get_int(bit_pos, 11));
     bit_pos += 11;
     if (sea_temperature != -1024) {
         sea_temperature_available_ = true;
         sea_temperature_ = sea_temperature;
     }
     
     // Precipitation
     uint8_t precipitation_type = static_cast<uint8_t>(bits.get_uint(bit_pos, 3));
     bit_pos += 3;
     if (precipitation_type != 0x7) {
         precipitation_available_ = true;
         precipitation_type_ = precipitation_type;
     }
     
     // Salinity
     uint16_t salinity = static_cast<uint16_t>(bits.get_uint(bit_pos, 9));
     bit_pos += 9;
     if (salinity != 0x1FF) {
         salinity_available_ = true;
         salinity_ = salinity;
     }
     
     // Ice
     uint8_t ice = static_cast<uint8_t>(bits.get_uint(bit_pos, 2));
     bit_pos += 2;
     if (ice != 0x3) {
         ice_available_ = true;
         ice_ = ice;
     }
 }
 
 double MeteorologicalData::get_latitude() const {
     return latitude_ / 60000.0; // Convert from 1/1000 minutes to degrees
 }
 
 double MeteorologicalData::get_longitude() const {
     return longitude_ / 60000.0; // Convert from 1/1000 minutes to degrees
 }
 
 std::chrono::system_clock::time_point MeteorologicalData::get_timestamp() const {
     // Get the current time to determine the year and month
     std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
     std::tm* now_tm = std::gmtime(&now);
     
     if (!now_tm) {
         return std::chrono::system_clock::time_point();
     }
     
     // Create a tm struct for the timestamp
     std::tm timestamp_tm = *now_tm;
     timestamp_tm.tm_mday = day_;
     timestamp_tm.tm_hour = hour_;
     timestamp_tm.tm_min = minute_;
     timestamp_tm.tm_sec = 0;
     
     // If the day of the month is greater than the current day of the month,
     // the timestamp is likely from the previous month
     if (day_ > now_tm->tm_mday) {
         timestamp_tm.tm_mon -= 1;
         if (timestamp_tm.tm_mon < 0) {
             timestamp_tm.tm_mon = 11;
             timestamp_tm.tm_year -= 1;
         }
     }
     
     // Convert to time_t and then to system_clock::time_point
     std::time_t timestamp = std::mktime(&timestamp_tm);
     return std::chrono::system_clock::from_time_t(timestamp);
 }
 
 float MeteorologicalData::get_wind_speed() const {
     if (!wind_speed_available_) {
         return -1.0f;
     }
     return wind_speed_ * 0.1f;
 }
 
 float MeteorologicalData::get_wind_gust() const {
     if (!wind_gust_available_) {
         return -1.0f;
     }
     return wind_gust_ * 0.1f;
 }
 
 int16_t MeteorologicalData::get_wind_direction() const {
     if (!wind_direction_available_) {
         return -1;
     }
     return static_cast<int16_t>(wind_direction_);
 }
 
 float MeteorologicalData::get_air_temperature() const {
     if (!air_temperature_available_) {
         return -1024.0f;
     }
     return air_temperature_ * 0.1f;
 }
 
 int8_t MeteorologicalData::get_relative_humidity() const {
     if (!relative_humidity_available_) {
         return -1;
     }
     return static_cast<int8_t>(relative_humidity_);
 }
 
 float MeteorologicalData::get_dew_point() const {
     if (!dew_point_available_) {
         return -1024.0f;
     }
     return dew_point_ * 0.1f;
 }
 
 int16_t MeteorologicalData::get_air_pressure() const {
     if (!air_pressure_available_) {
         return -1;
     }
     return static_cast<int16_t>(air_pressure_);
 }
 
 int8_t MeteorologicalData::get_air_pressure_tendency() const {
     if (!air_pressure_tendency_available_) {
         return -1;
     }
     return static_cast<int8_t>(air_pressure_tendency_);
 }
 
 float MeteorologicalData::get_horizontal_visibility() const {
     if (!visibility_available_) {
         return -1.0f;
     }
     return horizontal_visibility_ * 0.1f;
 }
 
 float MeteorologicalData::get_water_level() const {
     if (!water_level_available_) {
         return -327.68f;
     }
     return water_level_ * 0.01f;
 }
 
 int8_t MeteorologicalData::get_water_level_trend() const {
     if (!water_level_trend_available_) {
         return -1;
     }
     return static_cast<int8_t>(water_level_trend_);
 }
 
 float MeteorologicalData::get_surface_current_speed() const {
     if (!surface_current_available_) {
         return -1.0f;
     }
     return surface_current_speed_ * 0.1f;
 }
 
 int16_t MeteorologicalData::get_surface_current_direction() const {
     if (!surface_current_available_) {
         return -1;
     }
     return static_cast<int16_t>(surface_current_direction_);
 }
 
 float MeteorologicalData::get_wave_height() const {
     if (!wave_available_) {
         return -1.0f;
     }
     return wave_height_ * 0.1f;
 }
 
 int8_t MeteorologicalData::get_wave_period() const {
     if (!wave_available_) {
         return -1;
     }
     return static_cast<int8_t>(wave_period_);
 }
 
 int16_t MeteorologicalData::get_wave_direction() const {
     if (!wave_available_) {
         return -1;
     }
     return static_cast<int16_t>(wave_direction_);
 }
 
 float MeteorologicalData::get_swell_height() const {
     if (!swell_available_) {
         return -1.0f;
     }
     return swell_height_ * 0.1f;
 }
 
 int8_t MeteorologicalData::get_swell_period() const {
     if (!swell_available_) {
         return -1;
     }
     return static_cast<int8_t>(swell_period_);
 }
 
 int16_t MeteorologicalData::get_swell_direction() const {
     if (!swell_available_) {
         return -1;
     }
     return static_cast<int16_t>(swell_direction_);
 }
 
 float MeteorologicalData::get_sea_temperature() const {
     if (!sea_temperature_available_) {
         return -1024.0f;
     }
     return sea_temperature_ * 0.1f;
 }
 
 int8_t MeteorologicalData::get_precipitation_type() const {
     if (!precipitation_available_) {
         return 7; // Not available
     }
     return static_cast<int8_t>(precipitation_type_);
 }
 
 float MeteorologicalData::get_salinity() const {
     if (!salinity_available_) {
         return -1.0f;
     }
     return salinity_ * 0.1f;
 }
 
 int8_t MeteorologicalData::get_ice() const {
     if (!ice_available_) {
         return 3; // Not available
     }
     return static_cast<int8_t>(ice_);
 }
 
 void MeteorologicalData::set_latitude(double lat) {
     latitude_ = static_cast<int32_t>(std::round(lat * 60000.0)); // Convert from degrees to 1/1000 minutes
 }
 
 void MeteorologicalData::set_longitude(double lon) {
     longitude_ = static_cast<int32_t>(std::round(lon * 60000.0)); // Convert from degrees to 1/1000 minutes
 }
 
 void MeteorologicalData::set_timestamp(const std::chrono::system_clock::time_point& timestamp) {
     std::time_t time = std::chrono::system_clock::to_time_t(timestamp);
     std::tm* tm = std::gmtime(&time);
     
     if (tm) {
         day_ = static_cast<uint8_t>(tm->tm_mday);
         hour_ = static_cast<uint8_t>(tm->tm_hour);
         minute_ = static_cast<uint8_t>(tm->tm_min);
     }
 }
 
 void MeteorologicalData::set_wind_speed(float speed) {
     if (speed < 0.0f) {
         wind_speed_available_ = false;
     } else {
         wind_speed_available_ = true;
         wind_speed_ = static_cast<uint16_t>(std::round(speed * 10.0f));
     }
 }
 
 void MeteorologicalData::set_wind_gust(float gust) {
     if (gust < 0.0f) {
         wind_gust_available_ = false;
     } else {
         wind_gust_available_ = true;
         wind_gust_ = static_cast<uint16_t>(std::round(gust * 10.0f));
     }
 }
 
 void MeteorologicalData::set_wind_direction(int16_t direction) {
     if (direction < 0 || direction > 359) {
         wind_direction_available_ = false;
     } else {
         wind_direction_available_ = true;
         wind_direction_ = static_cast<uint16_t>(direction);
     }
 }
 
 void MeteorologicalData::set_air_temperature(float temperature) {
     if (std::isnan(temperature)) {
         air_temperature_available_ = false;
     } else {
         air_temperature_available_ = true;
         air_temperature_ = static_cast<int16_t>(std::round(temperature * 10.0f));
     }
 }
 
 void MeteorologicalData::set_relative_humidity(int8_t humidity) {
     if (humidity < 0 || humidity > 100) {
         relative_humidity_available_ = false;
     } else {
         relative_humidity_available_ = true;
         relative_humidity_ = static_cast<uint8_t>(humidity);
     }
 }
 
 void MeteorologicalData::set_dew_point(float dew_point) {
     if (std::isnan(dew_point)) {
         dew_point_available_ = false;
     } else {
         dew_point_available_ = true;
         dew_point_ = static_cast<int16_t>(std::round(dew_point * 10.0f));
     }
 }
 
 void MeteorologicalData::set_air_pressure(int16_t pressure) {
     if (pressure < 0) {
         air_pressure_available_ = false;
     } else {
         air_pressure_available_ = true;
         air_pressure_ = static_cast<uint16_t>(pressure);
     }
 }
 
 void MeteorologicalData::set_air_pressure_tendency(int8_t tendency) {
     if (tendency < 0 || tendency > 2) {
         air_pressure_tendency_available_ = false;
     } else {
         air_pressure_tendency_available_ = true;
         air_pressure_tendency_ = static_cast<uint8_t>(tendency);
     }
 }
 
 void MeteorologicalData::set_horizontal_visibility(float visibility) {
     if (visibility < 0.0f) {
         visibility_available_ = false;
     } else {
         visibility_available_ = true;
         horizontal_visibility_ = static_cast<uint8_t>(std::round(visibility * 10.0f));
     }
 }
 
 void MeteorologicalData::set_water_level(float level) {
     if (std::isnan(level)) {
         water_level_available_ = false;
     } else {
         water_level_available_ = true;
         water_level_ = static_cast<int16_t>(std::round(level * 100.0f));
     }
 }
 
 void MeteorologicalData::set_water_level_trend(int8_t trend) {
     if (trend < 0 || trend > 2) {
         water_level_trend_available_ = false;
     } else {
         water_level_trend_available_ = true;
         water_level_trend_ = static_cast<uint8_t>(trend);
     }
 }
 
 void MeteorologicalData::set_surface_current_speed(float speed) {
     if (speed < 0.0f) {
         surface_current_available_ = false;
     } else {
         surface_current_available_ = true;
         surface_current_speed_ = static_cast<uint16_t>(std::round(speed * 10.0f));
     }
 }
 
 void MeteorologicalData::set_surface_current_direction(int16_t direction) {
     if (direction < 0 || direction > 359) {
         surface_current_available_ = false;
     } else {
         surface_current_available_ = true;
         surface_current_direction_ = static_cast<uint16_t>(direction);
     }
 }
 
 void MeteorologicalData::set_wave_height(float height) {
     if (height < 0.0f) {
         wave_available_ = false;
     } else {
         wave_available_ = true;
         wave_height_ = static_cast<uint8_t>(std::round(height * 10.0f));
     }
 }
 
 void MeteorologicalData::set_wave_period(int8_t period) {
     if (period < 0) {
         wave_available_ = false;
     } else {
         wave_available_ = true;
         wave_period_ = static_cast<uint8_t>(period);
     }
 }
 
 void MeteorologicalData::set_wave_direction(int16_t direction) {
     if (direction < 0 || direction > 359) {
         wave_available_ = false;
     } else {
         wave_available_ = true;
         wave_direction_ = static_cast<uint16_t>(direction);
     }
 }
 
 void MeteorologicalData::set_swell_height(float height) {
     if (height < 0.0f) {
         swell_available_ = false;
     } else {
         swell_available_ = true;
         swell_height_ = static_cast<uint8_t>(std::round(height * 10.0f));
     }
 }
 
 void MeteorologicalData::set_swell_period(int8_t period) {
     if (period < 0) {
         swell_available_ = false;
     } else {
         swell_available_ = true;
         swell_period_ = static_cast<uint8_t>(period);
     }
 }
 
 void MeteorologicalData::set_swell_direction(int16_t direction) {
     if (direction < 0 || direction > 359) {
         swell_available_ = false;
     } else {
         swell_available_ = true;
         swell_direction_ = static_cast<uint16_t>(direction);
     }
 }
 
 void MeteorologicalData::set_sea_temperature(float temperature) {
     if (std::isnan(temperature)) {
         sea_temperature_available_ = false;
     } else {
         sea_temperature_available_ = true;
         sea_temperature_ = static_cast<int16_t>(std::round(temperature * 10.0f));
     }
 }
 
 void MeteorologicalData::set_precipitation_type(int8_t type) {
     if (type < 0 || type > 5) {
         precipitation_available_ = false;
     } else {
         precipitation_available_ = true;
         precipitation_type_ = static_cast<uint8_t>(type);
     }
 }
 
 void MeteorologicalData::set_salinity(float salinity) {
     if (salinity < 0.0f) {
         salinity_available_ = false;
     } else {
         salinity_available_ = true;
         salinity_ = static_cast<uint16_t>(std::round(salinity * 10.0f));
     }
 }
 
 void MeteorologicalData::set_ice(int8_t ice) {
     if (ice < 0 || ice > 1) {
         ice_available_ = false;
     } else {
         ice_available_ = true;
         ice_ = static_cast<uint8_t>(ice);
     }
 }
 
 void MeteorologicalData::to_bits(BitVector& bits) const {
     // Position and time
     bits.append_int(latitude_, 24);
     bits.append_int(longitude_, 25);
     bits.append_uint(day_, 5);
     bits.append_uint(hour_, 5);
     bits.append_uint(minute_, 6);
     
     // Wind speed
     if (wind_speed_available_) {
         bits.append_uint(wind_speed_, 10);
     } else {
         bits.append_uint(0x3FF, 10); // Not available
     }
     
     // Wind gust
     if (wind_gust_available_) {
         bits.append_uint(wind_gust_, 10);
     } else {
         bits.append_uint(0x3FF, 10); // Not available
     }
     
     // Wind direction
     if (wind_direction_available_) {
         bits.append_uint(wind_direction_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Air temperature
     if (air_temperature_available_) {
         bits.append_int(air_temperature_, 11);
     } else {
         bits.append_int(-1024, 11); // Not available
     }
     
     // Relative humidity
     if (relative_humidity_available_) {
         bits.append_uint(relative_humidity_, 7);
     } else {
         bits.append_uint(0x7F, 7); // Not available
     }
     
     // Dew point
     if (dew_point_available_) {
         bits.append_int(dew_point_, 11);
     } else {
         bits.append_int(-1024, 11); // Not available
     }
     
     // Air pressure
     if (air_pressure_available_) {
         bits.append_uint(air_pressure_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Air pressure tendency
     if (air_pressure_tendency_available_) {
         bits.append_uint(air_pressure_tendency_, 2);
     } else {
         bits.append_uint(0x3, 2); // Not available
     }
     
     // Horizontal visibility
     if (visibility_available_) {
         bits.append_uint(horizontal_visibility_, 8);
     } else {
         bits.append_uint(0xFF, 8); // Not available
     }
     
     // Water level
     if (water_level_available_) {
         bits.append_int(water_level_, 12);
     } else {
         bits.append_int(-2048, 12); // Not available
     }
     
     // Water level trend
     if (water_level_trend_available_) {
         bits.append_uint(water_level_trend_, 2);
     } else {
         bits.append_uint(0x3, 2); // Not available
     }
     
     // Surface current speed
     if (surface_current_available_) {
         bits.append_uint(surface_current_speed_, 8);
     } else {
         bits.append_uint(0xFF, 8); // Not available
     }
     
     // Surface current direction
     if (surface_current_available_) {
         bits.append_uint(surface_current_direction_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Wave height
     if (wave_available_) {
         bits.append_uint(wave_height_, 8);
     } else {
         bits.append_uint(0xFF, 8); // Not available
     }
     
     // Wave period
     if (wave_available_) {
         bits.append_uint(wave_period_, 6);
     } else {
         bits.append_uint(0x3F, 6); // Not available
     }
     
     // Wave direction
     if (wave_available_) {
         bits.append_uint(wave_direction_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Swell height
     if (swell_available_) {
         bits.append_uint(swell_height_, 8);
     } else {
         bits.append_uint(0xFF, 8); // Not available
     }
     
     // Swell period
     if (swell_available_) {
         bits.append_uint(swell_period_, 6);
     } else {
         bits.append_uint(0x3F, 6); // Not available
     }
     
     // Swell direction
     if (swell_available_) {
         bits.append_uint(swell_direction_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Sea temperature
     if (sea_temperature_available_) {
         bits.append_int(sea_temperature_, 11);
     } else {
         bits.append_int(-1024, 11); // Not available
     }
     
     // Precipitation type
     if (precipitation_available_) {
         bits.append_uint(precipitation_type_, 3);
     } else {
         bits.append_uint(0x7, 3); // Not available
     }
     
     // Salinity
     if (salinity_available_) {
         bits.append_uint(salinity_, 9);
     } else {
         bits.append_uint(0x1FF, 9); // Not available
     }
     
     // Ice
     if (ice_available_) {
         bits.append_uint(ice_, 2);
     } else {
         bits.append_uint(0x3, 2); // Not available
     }
 }
 
 BitVector MeteorologicalData::to_bit_vector() const {
     BitVector bits;
     to_bits(bits);
     return bits;
 }
 
 BinaryAddressedMessage MeteorologicalData::to_addressed_message(
     uint32_t source_mmsi,
     uint32_t dest_mmsi,
     uint8_t sequence_number,
     uint8_t repeat_indicator
 ) const {
     BinaryAddressedMessage message(source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
     message.set_application_id(BINARY_APP_ID_IMO, BINARY_APP_FI_METEO_HYDRO_DATA);
     message.set_data(to_bit_vector());
     return message;
 }
 
 BinaryBroadcastMessage MeteorologicalData::to_broadcast_message(
     uint32_t source_mmsi,
     uint8_t repeat_indicator
 ) const {
     BinaryBroadcastMessage message(source_mmsi, repeat_indicator);
     message.set_application_id(BINARY_APP_ID_IMO, BINARY_APP_FI_METEO_HYDRO_DATA);
     message.set_data(to_bit_vector());
     return message;
 }
 
 std::string MeteorologicalData::to_string() const {
     std::stringstream ss;
     
     ss << "Meteorological and Hydrological Data:\n";
     ss << "===================================\n";
     
     // Position and time
     ss << "Position: " << get_latitude() << "°, " << get_longitude() << "°\n";
     ss << "Time: Day " << static_cast<int>(day_) << ", " 
        << std::setw(2) << std::setfill('0') << static_cast<int>(hour_) << ":" 
        << std::setw(2) << std::setfill('0') << static_cast<int>(minute_) << " UTC\n\n";
     
     // Wind data
     if (wind_speed_available_) {
         ss << "Wind Speed: " << get_wind_speed() << " knots\n";
     }
     
     if (wind_gust_available_) {
         ss << "Wind Gust: " << get_wind_gust() << " knots\n";
     }
     
     if (wind_direction_available_) {
         ss << "Wind Direction: " << get_wind_direction() << "°\n";
     }
     
     // Air data
     if (air_temperature_available_) {
         ss << "Air Temperature: " << get_air_temperature() << " °C\n";
     }
     
     if (relative_humidity_available_) {
         ss << "Relative Humidity: " << static_cast<int>(get_relative_humidity()) << "%\n";
     }
     
     if (dew_point_available_) {
         ss << "Dew Point: " << get_dew_point() << " °C\n";
     }
     
     if (air_pressure_available_) {
         ss << "Air Pressure: " << get_air_pressure() << " hPa\n";
     }
     
     if (air_pressure_tendency_available_) {
         switch (air_pressure_tendency_) {
             case 0: ss << "Air Pressure Trend: Steady\n"; break;
             case 1: ss << "Air Pressure Trend: Decreasing\n"; break;
             case 2: ss << "Air Pressure Trend: Increasing\n"; break;
         }
     }
     
     if (visibility_available_) {
         ss << "Horizontal Visibility: " << get_horizontal_visibility() << " NM\n";
     }
     
     // Water data
     if (water_level_available_) {
         ss << "Water Level: " << get_water_level() << " m\n";
     }
     
     if (water_level_trend_available_) {
         switch (water_level_trend_) {
             case 0: ss << "Water Level Trend: Steady\n"; break;
             case 1: ss << "Water Level Trend: Decreasing\n"; break;
             case 2: ss << "Water Level Trend: Increasing\n"; break;
         }
     }
     
     // Surface current
     if (surface_current_available_) {
         ss << "Surface Current: " << get_surface_current_speed() << " knots, " 
            << get_surface_current_direction() << "°\n";
     }
     
     // Wave data
     if (wave_available_) {
         ss << "Wave Height: " << get_wave_height() << " m\n";
         ss << "Wave Period: " << static_cast<int>(get_wave_period()) << " s\n";
         ss << "Wave Direction: " << get_wave_direction() << "°\n";
     }
     
     // Swell data
     if (swell_available_) {
         ss << "Swell Height: " << get_swell_height() << " m\n";
         ss << "Swell Period: " << static_cast<int>(get_swell_period()) << " s\n";
         ss << "Swell Direction: " << get_swell_direction() << "°\n";
     }
     
     // Sea temperature
     if (sea_temperature_available_) {
         ss << "Sea Temperature: " << get_sea_temperature() << " °C\n";
     }
     
     // Precipitation
     if (precipitation_available_) {
         ss << "Precipitation: ";
         switch (precipitation_type_) {
             case 0: ss << "Reserved\n"; break;
             case 1: ss << "Rain\n"; break;
             case 2: ss << "Thunderstorm\n"; break;
             case 3: ss << "Freezing Rain\n"; break;
             case 4: ss << "Mixed/Ice\n"; break;
             case 5: ss << "Snow\n"; break;
             case 6: ss << "Reserved\n"; break;
         }
     }
     
     // Salinity
     if (salinity_available_) {
         ss << "Salinity: " << get_salinity() << " ‰\n";
     }
     
     // Ice
     if (ice_available_) {
         ss << "Ice: " << (ice_ == 0 ? "No" : "Yes") << "\n";
     }
     
     return ss.str();
 }
 
 } // namespace application
 } // namespace aislib