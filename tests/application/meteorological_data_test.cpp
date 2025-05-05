#include <gtest/gtest.h>
#include "aislib/application/meteorological_data.h"
#include "aislib/bit_vector.h"
#include <cmath>
#include <chrono>

using namespace aislib;
using namespace aislib::application;

TEST(MeteorologicalDataTest, Constructor) {
    // Test constructor with required parameters
    int32_t lat = 3545123;  // 59.0854° N
    int32_t lon = 1063642;  // 17.7274° E
    
    auto now = std::chrono::system_clock::now();
    
    MeteorologicalData data(lat, lon, now);
    
    // Check required fields
    EXPECT_NEAR(data.get_latitude(), 59.0854, 0.0001);
    EXPECT_NEAR(data.get_longitude(), 17.7274, 0.0001);
    
    // Time should be set but we can't test exact values as they depend on now
    
    // Check that optional fields are not available
    EXPECT_FLOAT_EQ(data.get_wind_speed(), -1.0f);
    EXPECT_FLOAT_EQ(data.get_wind_gust(), -1.0f);
    EXPECT_EQ(data.get_wind_direction(), -1);
    EXPECT_FLOAT_EQ(data.get_air_temperature(), -1024.0f);
    EXPECT_EQ(data.get_relative_humidity(), -1);
    EXPECT_FLOAT_EQ(data.get_dew_point(), -1024.0f);
    EXPECT_EQ(data.get_air_pressure(), -1);
    EXPECT_EQ(data.get_air_pressure_tendency(), -1);
    EXPECT_FLOAT_EQ(data.get_horizontal_visibility(), -1.0f);
    EXPECT_FLOAT_EQ(data.get_water_level(), -327.68f);
    EXPECT_EQ(data.get_water_level_trend(), -1);
    EXPECT_FLOAT_EQ(data.get_surface_current_speed(), -1.0f);
    EXPECT_EQ(data.get_surface_current_direction(), -1);
    EXPECT_FLOAT_EQ(data.get_wave_height(), -1.0f);
    EXPECT_EQ(data.get_wave_period(), -1);
    EXPECT_EQ(data.get_wave_direction(), -1);
    EXPECT_FLOAT_EQ(data.get_swell_height(), -1.0f);
    EXPECT_EQ(data.get_swell_period(), -1);
    EXPECT_EQ(data.get_swell_direction(), -1);
    EXPECT_FLOAT_EQ(data.get_sea_temperature(), -1024.0f);
    EXPECT_EQ(data.get_precipitation_type(), 7);
    EXPECT_FLOAT_EQ(data.get_salinity(), -1.0f);
    EXPECT_EQ(data.get_ice(), 3);
}

TEST(MeteorologicalDataTest, SettersAndGetters) {
    // Create data object
    int32_t lat = 3545123;  // 59.0854° N
    int32_t lon = 1063642;  // 17.7274° E
    auto now = std::chrono::system_clock::now();
    
    MeteorologicalData data(lat, lon, now);
    
    // Set wind parameters
    data.set_wind_speed(15.7f);
    data.set_wind_gust(22.3f);
    data.set_wind_direction(270);
    
    // Set air parameters
    data.set_air_temperature(25.4f);
    data.set_relative_humidity(85);
    data.set_dew_point(22.5f);
    data.set_air_pressure(1012);
    data.set_air_pressure_tendency(0);
    data.set_horizontal_visibility(8.5f);
    
    // Set water parameters
    data.set_water_level(2.3f);
    data.set_water_level_trend(2);
    data.set_surface_current_speed(3.5f);
    data.set_surface_current_direction(180);
    
    // Set wave parameters
    data.set_wave_height(1.2f);
    data.set_wave_period(5);
    data.set_wave_direction(225);
    
    // Set swell parameters
    data.set_swell_height(0.8f);
    data.set_swell_period(8);
    data.set_swell_direction(210);
    
    // Set other parameters
    data.set_sea_temperature(18.5f);
    data.set_precipitation_type(1);
    data.set_salinity(32.5f);
    data.set_ice(0);
    
    // Check wind parameters
    EXPECT_NEAR(data.get_wind_speed(), 15.7f, 0.1f);
    EXPECT_NEAR(data.get_wind_gust(), 22.3f, 0.1f);
    EXPECT_EQ(data.get_wind_direction(), 270);
    
    // Check air parameters
    EXPECT_NEAR(data.get_air_temperature(), 25.4f, 0.1f);
    EXPECT_EQ(data.get_relative_humidity(), 85);
    EXPECT_NEAR(data.get_dew_point(), 22.5f, 0.1f);
    EXPECT_EQ(data.get_air_pressure(), 1012);
    EXPECT_EQ(data.get_air_pressure_tendency(), 0);
    EXPECT_NEAR(data.get_horizontal_visibility(), 8.5f, 0.1f);
    
    // Check water parameters
    EXPECT_NEAR(data.get_water_level(), 2.3f, 0.01f);
    EXPECT_EQ(data.get_water_level_trend(), 2);
    EXPECT_NEAR(data.get_surface_current_speed(), 3.5f, 0.1f);
    EXPECT_EQ(data.get_surface_current_direction(), 180);
    
    // Check wave parameters
    EXPECT_NEAR(data.get_wave_height(), 1.2f, 0.1f);
    EXPECT_EQ(data.get_wave_period(), 5);
    EXPECT_EQ(data.get_wave_direction(), 225);
    
    // Check swell parameters
    EXPECT_NEAR(data.get_swell_height(), 0.8f, 0.1f);
    EXPECT_EQ(data.get_swell_period(), 8);
    EXPECT_EQ(data.get_swell_direction(), 210);
    
    // Check other parameters
    EXPECT_NEAR(data.get_sea_temperature(), 18.5f, 0.1f);
    EXPECT_EQ(data.get_precipitation_type(), 1);
    EXPECT_NEAR(data.get_salinity(), 32.5f, 0.1f);
    EXPECT_EQ(data.get_ice(), 0);
}

TEST(MeteorologicalDataTest, ToBits) {
    // Create data object with some parameters
    int32_t lat = 3545123;  // 59.0854° N
    int32_t lon = 1063642;  // 17.7274° E
    
    // Use a fixed timestamp to ensure reproducible results
    std::tm time_parts = {};
    time_parts.tm_year = 2024 - 1900;
    time_parts.tm_mon = 4;  // May (0-based)
    time_parts.tm_mday = 15;
    time_parts.tm_hour = 12;
    time_parts.tm_min = 30;
    time_parts.tm_sec = 0;
    std::time_t time = std::mktime(&time_parts);
    auto timestamp = std::chrono::system_clock::from_time_t(time);
    
    MeteorologicalData data(lat, lon, timestamp);
    
    // Set a few parameters
    data.set_wind_speed(15.7f);
    data.set_wind_direction(270);
    data.set_air_temperature(25.4f);
    data.set_air_pressure(1012);
    
    // Convert to bits
    BitVector bits = data.to_bit_vector();
    
    // Check that the bit vector has a reasonable size
    EXPECT_GT(bits.size(), 100);
    
    // Recreate from bits
    MeteorologicalData data2(bits);
    
    // Check parameters
    EXPECT_NEAR(data2.get_latitude(), 59.0854, 0.0001);
    EXPECT_NEAR(data2.get_longitude(), 17.7274, 0.0001);
    
    EXPECT_NEAR(data2.get_wind_speed(), 15.7f, 0.1f);
    EXPECT_EQ(data2.get_wind_direction(), 270);
    EXPECT_NEAR(data2.get_air_temperature(), 25.4f, 0.1f);
    EXPECT_EQ(data2.get_air_pressure(), 1012);
    
    // Check parameters that weren't set
    EXPECT_FLOAT_EQ(data2.get_wind_gust(), -1.0f);
    EXPECT_EQ(data2.get_relative_humidity(), -1);
}

TEST(MeteorologicalDataTest, ToMessage) {
    // Create data object
    int32_t lat = 3545123;  // 59.0854° N
    int32_t lon = 1063642;  // 17.7274° E
    auto now = std::chrono::system_clock::now();
    
    MeteorologicalData data(lat, lon, now);
    
    // Set some parameters
    data.set_wind_speed(15.7f);
    data.set_air_temperature(25.4f);
    
    // Create binary addressed message
    uint32_t source_mmsi = 123456789;
    uint32_t dest_mmsi = 987654321;
    uint8_t sequence_number = 0;
    uint8_t repeat_indicator = 0;
    
    BinaryAddressedMessage msg1 = data.to_addressed_message(
        source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
    
    // Check message parameters
    EXPECT_EQ(msg1.get_message_type(), 6);
    EXPECT_EQ(msg1.get_mmsi(), source_mmsi);
    EXPECT_EQ(msg1.get_dest_mmsi(), dest_mmsi);
    EXPECT_EQ(msg1.get_sequence_number(), sequence_number);
    EXPECT_EQ(msg1.get_repeat_indicator(), repeat_indicator);
    EXPECT_EQ(msg1.get_dac(), BINARY_APP_ID_IMO);
    EXPECT_EQ(msg1.get_fi(), BINARY_APP_FI_METEO_HYDRO_DATA);
    
    // Create binary broadcast message
    BinaryBroadcastMessage msg2 = data.to_broadcast_message(source_mmsi, repeat_indicator);
    
    // Check message parameters
    EXPECT_EQ(msg2.get_message_type(), 8);
    EXPECT_EQ(msg2.get_mmsi(), source_mmsi);
    EXPECT_EQ(msg2.get_repeat_indicator(), repeat_indicator);
    EXPECT_EQ(msg2.get_dac(), BINARY_APP_ID_IMO);
    EXPECT_EQ(msg2.get_fi(), BINARY_APP_FI_METEO_HYDRO_DATA);
}

TEST(MeteorologicalDataTest, ToString) {
    // Create data object
    int32_t lat = 3545123;  // 59.0854° N
    int32_t lon = 1063642;  // 17.7274° E
    auto now = std::chrono::system_clock::now();
    
    MeteorologicalData data(lat, lon, now);
    
    // Set some parameters
    data.set_wind_speed(15.7f);
    data.set_wind_direction(270);
    data.set_air_temperature(25.4f);
    
    // Convert to string
    std::string str = data.to_string();
    
    // Check that the string contains important information
    EXPECT_NE(str.find("Meteorological and Hydrological Data"), std::string::npos);
    EXPECT_NE(str.find("59.0854"), std::string::npos);
    EXPECT_NE(str.find("17.7274"), std::string::npos);
    EXPECT_NE(str.find("Wind Speed: 15.7"), std::string::npos);
    EXPECT_NE(str.find("Wind Direction: 270"), std::string::npos);
    EXPECT_NE(str.find("Air Temperature: 25.4"), std::string::npos);
}