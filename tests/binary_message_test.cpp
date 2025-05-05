/**
 * @file binary_message_test.cpp
 * @brief Unit tests for binary message classes
 * 
 * This file contains unit tests for the binary message classes in the AISLIB
 * library, including BinaryAddressedMessage, BinaryBroadcastMessage, AreaNotice,
 * and MeteorologicalData.
 * 
 * The tests verify:
 * - Construction and property access of binary messages
 * - Setting and retrieving application IDs and binary data
 * - Serialization and deserialization of binary messages
 * - Creation and manipulation of Area Notices with different subareas
 * - Creation and manipulation of Meteorological Data with various parameters
 * - Conversion of application-specific messages to binary messages
 * - Conversion of binary messages to NMEA sentences
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

#include <gtest/gtest.h>
#include "aislib/binary_message.h"
#include "aislib/binary_addressed_message.h"
#include "aislib/binary_broadcast_message.h"
#include "aislib/application/area_notice.h"
#include "aislib/application/meteorological_data.h"
#include <string>
#include <memory>
#include <chrono>

using namespace aislib;

TEST(BinaryMessageTest, BinaryAddressedMessageBasic) {
    // Create a binary addressed message
    uint32_t source_mmsi = 123456789;
    uint32_t dest_mmsi = 987654321;
    uint8_t sequence_number = 0;
    uint8_t repeat_indicator = 0;
    
    BinaryAddressedMessage message(source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
    
    // Check basic properties
    EXPECT_EQ(message.get_message_type(), 6);
    EXPECT_EQ(message.get_mmsi(), source_mmsi);
    EXPECT_EQ(message.get_dest_mmsi(), dest_mmsi);
    EXPECT_EQ(message.get_sequence_number(), sequence_number);
    EXPECT_EQ(message.get_repeat_indicator(), repeat_indicator);
    EXPECT_FALSE(message.get_retransmit_flag());
    
    // Set application ID
    message.set_application_id(1, 31); // DAC 1 = IMO/International, FI 31 = Meteorological & Hydrological Data
    
    // Check application ID
    EXPECT_EQ(message.get_dac(), 1);
    EXPECT_EQ(message.get_fi(), 31);
    
    // Set retransmit flag
    message.set_retransmit_flag(true);
    EXPECT_TRUE(message.get_retransmit_flag());
    
    // Create binary data
    BitVector data;
    data.append_uint(42, 8);
    data.append_string("TEST", 4 * 6);
    data.append_int(-123, 16);
    
    // Set data
    message.set_data(data);
    
    // Check data
    const BitVector& data_ref = message.get_data();
    EXPECT_EQ(data_ref.size(), 8 + 4 * 6 + 16);
    EXPECT_EQ(data_ref.get_uint(0, 8), 42);
    EXPECT_EQ(data_ref.get_string(8, 4 * 6), "TEST");
    EXPECT_EQ(data_ref.get_int(8 + 4 * 6, 16), -123);
    
    // Convert to bits and back
    BitVector bits;
    message.to_bits(bits);
    
    BinaryAddressedMessage message2(bits);
    
    // Check properties after reconstruction
    EXPECT_EQ(message2.get_message_type(), 6);
    EXPECT_EQ(message2.get_mmsi(), source_mmsi);
    EXPECT_EQ(message2.get_dest_mmsi(), dest_mmsi);
    EXPECT_EQ(message2.get_sequence_number(), sequence_number);
    EXPECT_EQ(message2.get_repeat_indicator(), repeat_indicator);
    EXPECT_TRUE(message2.get_retransmit_flag());
    EXPECT_EQ(message2.get_dac(), 1);
    EXPECT_EQ(message2.get_fi(), 31);
    
    // Check data after reconstruction
    const BitVector& data_ref2 = message2.get_data();
    EXPECT_EQ(data_ref2.size(), 8 + 4 * 6 + 16);
    EXPECT_EQ(data_ref2.get_uint(0, 8), 42);
    EXPECT_EQ(data_ref2.get_string(8, 4 * 6), "TEST");
    EXPECT_EQ(data_ref2.get_int(8 + 4 * 6, 16), -123);
}

TEST(BinaryMessageTest, BinaryBroadcastMessageBasic) {
    // Create a binary broadcast message
    uint32_t source_mmsi = 123456789;
    uint8_t repeat_indicator = 0;
    
    BinaryBroadcastMessage message(source_mmsi, repeat_indicator);
    
    // Check basic properties
    EXPECT_EQ(message.get_message_type(), 8);
    EXPECT_EQ(message.get_mmsi(), source_mmsi);
    EXPECT_EQ(message.get_repeat_indicator(), repeat_indicator);
    
    // Set application ID
    message.set_application_id(1, 22); // DAC 1 = IMO/International, FI 22 = Area Notice
    
    // Check application ID
    EXPECT_EQ(message.get_dac(), 1);
    EXPECT_EQ(message.get_fi(), 22);
    
    // Create binary data
    BitVector data;
    data.append_uint(1, 8);
    data.append_uint(5, 8);
    data.append_uint(120, 16);
    data.append_string("WARNING", 7 * 6);
    
    // Set data
    message.set_data(data);
    
    // Check data
    const BitVector& data_ref = message.get_data();
    EXPECT_EQ(data_ref.size(), 8 + 8 + 16 + 7 * 6);
    EXPECT_EQ(data_ref.get_uint(0, 8), 1);
    EXPECT_EQ(data_ref.get_uint(8, 8), 5);
    EXPECT_EQ(data_ref.get_uint(16, 16), 120);
    EXPECT_EQ(data_ref.get_string(32, 7 * 6), "WARNING");
    
    // Convert to bits and back
    BitVector bits;
    message.to_bits(bits);
    
    BinaryBroadcastMessage message2(bits);
    
    // Check properties after reconstruction
    EXPECT_EQ(message2.get_message_type(), 8);
    EXPECT_EQ(message2.get_mmsi(), source_mmsi);
    EXPECT_EQ(message2.get_repeat_indicator(), repeat_indicator);
    EXPECT_EQ(message2.get_dac(), 1);
    EXPECT_EQ(message2.get_fi(), 22);
    
    // Check data after reconstruction
    const BitVector& data_ref2 = message2.get_data();
    EXPECT_EQ(data_ref2.size(), 8 + 8 + 16 + 7 * 6);
    EXPECT_EQ(data_ref2.get_uint(0, 8), 1);
    EXPECT_EQ(data_ref2.get_uint(8, 8), 5);
    EXPECT_EQ(data_ref2.get_uint(16, 16), 120);
    EXPECT_EQ(data_ref2.get_string(32, 7 * 6), "WARNING");
}

TEST(BinaryMessageTest, AreaNotice) {
    // Create an Area Notice
    uint8_t message_version = 1;
    auto notice_type = application::AreaNotice::NoticeType::WARNING_STORM;
    
    // Set start time to current time
    auto start_time = std::chrono::system_clock::now();
    
    // Set duration to 2 hours (120 minutes)
    uint16_t duration = 120;
    
    // Create subareas
    std::vector<application::AreaNotice::SubArea> subareas;
    
    // Add a circle
    subareas.push_back(application::AreaNotice::SubArea::Circle(
        -122.4194,  // longitude
        37.7749,    // latitude
        5000        // radius in meters
    ));
    
    // Create the area notice
    application::AreaNotice notice(message_version, notice_type, start_time, duration, subareas);
    
    // Check properties
    EXPECT_EQ(notice.get_message_version(), message_version);
    EXPECT_EQ(notice.get_notice_type(), notice_type);
    EXPECT_EQ(notice.get_duration(), duration);
    EXPECT_EQ(notice.get_subareas().size(), 1);
    
    // Convert to bits and back
    BitVector bits = notice.to_bit_vector();
    application::AreaNotice notice2(bits);
    
    // Check properties after reconstruction
    EXPECT_EQ(notice2.get_message_version(), message_version);
    EXPECT_EQ(notice2.get_notice_type(), notice_type);
    EXPECT_EQ(notice2.get_duration(), duration);
    EXPECT_EQ(notice2.get_subareas().size(), 1);
    
    // Convert to binary broadcast message
    BinaryBroadcastMessage message = notice.to_broadcast_message(123456789, 0);
    
    // Check binary message properties
    EXPECT_EQ(message.get_message_type(), 8);
    EXPECT_EQ(message.get_mmsi(), 123456789);
    EXPECT_EQ(message.get_repeat_indicator(), 0);
    EXPECT_EQ(message.get_dac(), 1);
    EXPECT_EQ(message.get_fi(), 22); // Area Notice
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    EXPECT_GT(sentences.size(), 0);
}

TEST(BinaryMessageTest, MeteorologicalData) {
    // Create Meteorological Data
    int32_t lat = static_cast<int32_t>(37.8 * 60000);  // 1/1000 minutes
    int32_t lon = static_cast<int32_t>(-122.4 * 60000); // 1/1000 minutes
    
    // Set timestamp to current time
    auto timestamp = std::chrono::system_clock::now();
    
    // Create the meteorological data
    application::MeteorologicalData meteo(lat, lon, timestamp);
    
    // Set some fields
    meteo.set_wind_speed(15.5f);
    meteo.set_wind_direction(270);
    meteo.set_air_temperature(18.5f);
    
    // Check properties
    EXPECT_FLOAT_EQ(meteo.get_latitude(), 37.8);
    EXPECT_FLOAT_EQ(meteo.get_longitude(), -122.4);
    EXPECT_FLOAT_EQ(meteo.get_wind_speed(), 15.5f);
    EXPECT_EQ(meteo.get_wind_direction(), 270);
    EXPECT_FLOAT_EQ(meteo.get_air_temperature(), 18.5f);
    
    // Convert to bits and back
    BitVector bits = meteo.to_bit_vector();
    application::MeteorologicalData meteo2(bits);
    
    // Check properties after reconstruction
    EXPECT_NEAR(meteo2.get_latitude(), 37.8, 0.0001);
    EXPECT_NEAR(meteo2.get_longitude(), -122.4, 0.0001);
    EXPECT_NEAR(meteo2.get_wind_speed(), 15.5f, 0.1f);
    EXPECT_EQ(meteo2.get_wind_direction(), 270);
    EXPECT_NEAR(meteo2.get_air_temperature(), 18.5f, 0.1f);
    
    // Convert to binary broadcast message
    BinaryBroadcastMessage message = meteo.to_broadcast_message(123456789, 0);
    
    // Check binary message properties
    EXPECT_EQ(message.get_message_type(), 8);
    EXPECT_EQ(message.get_mmsi(), 123456789);
    EXPECT_EQ(message.get_repeat_indicator(), 0);
    EXPECT_EQ(message.get_dac(), 1);
    EXPECT_EQ(message.get_fi(), 31); // Meteorological & Hydrological data
    
    // Convert to NMEA sentences
    auto sentences = message.to_nmea();
    EXPECT_GT(sentences.size(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}