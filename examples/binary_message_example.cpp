/**
 * @file binary_message_example.cpp
 * @brief Example application for using binary messages
 * 
 * This file provides example code for using the binary message classes in
 * the AISLIB library. It demonstrates how to create and work with Binary
 * Addressed Messages, Binary Broadcast Messages, Area Notices, and
 * Meteorological Data.
 * 
 * The examples show how to:
 * - Create binary messages with application-specific data
 * - Set application IDs and binary data
 * - Create Area Notices with different types of subareas
 * - Create Meteorological Data with various parameters
 * - Convert application-specific messages to binary messages
 * - Convert binary messages to NMEA sentences
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

#include "aislib/binary_message.h"
#include "aislib/binary_addressed_message.h"
#include "aislib/binary_broadcast_message.h"
#include "aislib/application/area_notice.h"
#include "aislib/application/meteorological_data.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <vector>

using namespace aislib;

// Helper function to print a message's NMEA sentences
void print_nmea_sentences(const std::vector<std::string>& sentences) {
    std::cout << "  NMEA sentence(s):" << std::endl;
    for (const auto& sentence : sentences) {
        std::cout << "    " << sentence << std::endl;
    }
}

// Example function for Binary Addressed Message (Type 6)
void addressed_message_example() {
    std::cout << "==============================================" << std::endl;
    std::cout << "Binary Addressed Message (Type 6) Example" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Create a Binary Addressed Message
    uint32_t source_mmsi = 123456789;
    uint32_t dest_mmsi = 987654321;
    uint8_t sequence_number = 0;
    uint8_t repeat_indicator = 0;
    
    BinaryAddressedMessage message(source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
    
    // Set application ID (DAC and FI)
    message.set_application_id(1, 31); // DAC 1 = IMO/International, FI 31 = Meteorological & Hydrological Data
    
    // Create binary data (payload)
    BitVector data;
    
    // Add some example data to the payload
    data.append_uint(42, 8);            // Example 8-bit value
    data.append_string("TEST", 4 * 6);  // 4 characters, 6 bits each
    data.append_int(-123, 16);          // Example 16-bit signed value
    
    // Set the data
    message.set_data(data);
    
    // Set retransmit flag
    message.set_retransmit_flag(false);
    
    // Display message details
    std::cout << message.to_string() << std::endl;
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    print_nmea_sentences(sentences);
    
    std::cout << std::endl;
}

// Example function for Binary Broadcast Message (Type 8)
void broadcast_message_example() {
    std::cout << "==============================================" << std::endl;
    std::cout << "Binary Broadcast Message (Type 8) Example" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Create a Binary Broadcast Message
    uint32_t source_mmsi = 123456789;
    uint8_t repeat_indicator = 0;
    
    BinaryBroadcastMessage message(source_mmsi, repeat_indicator);
    
    // Set application ID (DAC and FI)
    message.set_application_id(1, 22); // DAC 1 = IMO/International, FI 22 = Area Notice
    
    // Create binary data (payload)
    BitVector data;
    
    // Add some example data to the payload
    data.append_uint(1, 8);             // Example 8-bit value
    data.append_uint(5, 8);             // Example 8-bit value
    data.append_uint(120, 16);          // Example 16-bit value
    data.append_string("WARNING", 7 * 6); // 7 characters, 6 bits each
    
    // Set the data
    message.set_data(data);
    
    // Display message details
    std::cout << message.to_string() << std::endl;
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    print_nmea_sentences(sentences);
    
    std::cout << std::endl;
}

// Example function for Area Notice
void area_notice_example() {
    std::cout << "==============================================" << std::endl;
    std::cout << "Area Notice Example" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Create an Area Notice
    uint8_t message_version = 1;
    auto notice_type = application::AreaNotice::NoticeType::WARNING_STORM;
    
    // Set start time to current time
    auto start_time = std::chrono::system_clock::now();
    
    // Set duration to 2 hours (120 minutes)
    uint16_t duration = 120;
    
    // Create subareas
    std::vector<application::AreaNotice::SubArea> subareas;
    
    // Add a circle centered at 37.7749° N, 122.4194° W (San Francisco) with 5km radius
    subareas.push_back(application::AreaNotice::SubArea::Circle(
        -122.4194,  // longitude
        37.7749,    // latitude
        5000        // radius in meters
    ));
    
    // Add a rectangle centered at 37.8° N, 122.4° W
    subareas.push_back(application::AreaNotice::SubArea::Rectangle(
        -122.4,  // longitude
        37.8,    // latitude
        2000,    // east dimension in meters
        3000,    // north dimension in meters
        45       // orientation in degrees
    ));
    
    // Add a text message
    subareas.push_back(application::AreaNotice::SubArea::Text(
        -122.45,  // longitude
        37.75,    // latitude
        "STORM WARNING"  // text
    ));
    
    // Create the area notice
    application::AreaNotice notice(message_version, notice_type, start_time, duration, subareas);
    
    // Display the area notice
    std::cout << notice.to_string() << std::endl;
    
    // Convert to a binary broadcast message
    BinaryBroadcastMessage message = notice.to_broadcast_message(123456789, 0);
    
    // Display the broadcast message
    std::cout << "As Binary Broadcast Message:" << std::endl;
    std::cout << message.to_string() << std::endl;
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    print_nmea_sentences(sentences);
    
    std::cout << std::endl;
}

// Example function for Meteorological Data
void meteorological_data_example() {
    std::cout << "==============================================" << std::endl;
    std::cout << "Meteorological Data Example" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Create Meteorological Data for a station in San Francisco Bay
    // Position: 37.8° N, 122.4° W
    int32_t lat = static_cast<int32_t>(37.8 * 60000);  // 1/1000 minutes
    int32_t lon = static_cast<int32_t>(-122.4 * 60000); // 1/1000 minutes
    
    // Set timestamp to current time
    auto timestamp = std::chrono::system_clock::now();
    
    // Create the meteorological data
    application::MeteorologicalData meteo(lat, lon, timestamp);
    
    // Set meteorological fields
    meteo.set_wind_speed(15.5f);              // 15.5 knots
    meteo.set_wind_gust(20.8f);               // 20.8 knots
    meteo.set_wind_direction(270);            // 270 degrees (west)
    meteo.set_air_temperature(18.5f);         // 18.5 degrees Celsius
    meteo.set_relative_humidity(85);          // 85%
    meteo.set_dew_point(15.8f);               // 15.8 degrees Celsius
    meteo.set_air_pressure(1013);             // 1013 hPa
    meteo.set_air_pressure_tendency(2);       // Increasing
    meteo.set_horizontal_visibility(10.0f);   // 10.0 nautical miles
    meteo.set_water_level(1.5f);              // 1.5 meters
    meteo.set_water_level_trend(0);           // Steady
    meteo.set_surface_current_speed(2.5f);    // 2.5 knots
    meteo.set_surface_current_direction(225); // 225 degrees
    meteo.set_wave_height(1.2f);              // 1.2 meters
    meteo.set_wave_period(8);                 // 8 seconds
    meteo.set_wave_direction(270);            // 270 degrees
    meteo.set_sea_temperature(15.5f);         // 15.5 degrees Celsius
    meteo.set_precipitation_type(1);          // Rain
    meteo.set_salinity(32.5f);                // 32.5 parts per thousand
    meteo.set_ice(0);                         // No ice
    
    // Display the meteorological data
    std::cout << meteo.to_string() << std::endl;
    
    // Convert to a binary broadcast message
    BinaryBroadcastMessage message = meteo.to_broadcast_message(123456789, 0);
    
    // Display the broadcast message
    std::cout << "As Binary Broadcast Message:" << std::endl;
    std::cout << message.to_string() << std::endl;
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    print_nmea_sentences(sentences);
    
    std::cout << std::endl;
}

int main() {
    std::cout << "AISLIB Binary Message Examples" << std::endl;
    std::cout << "=============================" << std::endl << std::endl;
    
    // Basic binary message examples
    addressed_message_example();
    broadcast_message_example();
    
    // Application-specific examples
    area_notice_example();
    meteorological_data_example();
    
    return 0;
}