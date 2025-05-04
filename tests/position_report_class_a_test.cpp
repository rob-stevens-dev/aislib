#include <gtest/gtest.h>
#include "aislib/position_report_class_a.h"
#include <string>
#include <stdexcept>
#include <cmath>

using namespace aislib;

TEST(PositionReportClassATest, Constructor) {
    // Test with valid parameters
    PositionReportClassA message(1, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    EXPECT_EQ(message.get_message_type(), 1);
    EXPECT_EQ(message.get_mmsi(), 123456789);
    EXPECT_EQ(message.get_repeat_indicator(), 0);
    EXPECT_EQ(message.get_navigation_status(), PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    // Test with invalid message type
    EXPECT_THROW(PositionReportClassA(0, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE), std::invalid_argument);
    EXPECT_THROW(PositionReportClassA(4, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE), std::invalid_argument);
}

TEST(PositionReportClassATest, GettersAndSetters) {
    PositionReportClassA message(1, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    // Test navigation status
    message.set_navigation_status(PositionReportClassA::NavigationStatus::AT_ANCHOR);
    EXPECT_EQ(message.get_navigation_status(), PositionReportClassA::NavigationStatus::AT_ANCHOR);
    
    // Test rate of turn
    message.set_rate_of_turn(10.5f);
    EXPECT_NEAR(message.get_rate_of_turn(), 10.5f, 0.5f);
    
    message.set_rate_of_turn(-15.3f);
    EXPECT_NEAR(message.get_rate_of_turn(), -15.3f, 0.5f);
    
    message.set_rate_of_turn(std::numeric_limits<float>::quiet_NaN());
    EXPECT_TRUE(std::isnan(message.get_rate_of_turn()));
    
    // Test speed over ground
    message.set_speed_over_ground(12.4f);
    EXPECT_NEAR(message.get_speed_over_ground(), 12.4f, 0.1f);
    
    message.set_speed_over_ground(102.3f);
    EXPECT_NEAR(message.get_speed_over_ground(), 102.2f, 0.1f);
    
    message.set_speed_over_ground(std::numeric_limits<float>::quiet_NaN());
    EXPECT_TRUE(std::isnan(message.get_speed_over_ground()));
    
    // Test position accuracy
    message.set_position_accuracy(true);
    EXPECT_TRUE(message.get_position_accuracy());
    
    message.set_position_accuracy(false);
    EXPECT_FALSE(message.get_position_accuracy());
    
    // Test longitude
    message.set_longitude(123.456789);
    EXPECT_NEAR(message.get_longitude(), 123.456789, 0.000001);
    
    message.set_longitude(-123.456789);
    EXPECT_NEAR(message.get_longitude(), -123.456789, 0.000001);
    
    message.set_longitude(181.0);
    EXPECT_GT(message.get_longitude(), 180.0);
    
    // Test latitude
    message.set_latitude(45.678901);
    EXPECT_NEAR(message.get_latitude(), 45.678901, 0.000001);
    
    message.set_latitude(-45.678901);
    EXPECT_NEAR(message.get_latitude(), -45.678901, 0.000001);
    
    message.set_latitude(91.0);
    EXPECT_GT(message.get_latitude(), 90.0);
    
    // Test course over ground
    message.set_course_over_ground(123.4f);
    EXPECT_NEAR(message.get_course_over_ground(), 123.4f, 0.1f);
    
    message.set_course_over_ground(359.9f);
    EXPECT_NEAR(message.get_course_over_ground(), 359.9f, 0.1f);
    
    message.set_course_over_ground(std::numeric_limits<float>::quiet_NaN());
    EXPECT_TRUE(std::isnan(message.get_course_over_ground()));
    
    // Test true heading
    message.set_true_heading(123);
    EXPECT_EQ(message.get_true_heading(), 123);
    
    message.set_true_heading(359);
    EXPECT_EQ(message.get_true_heading(), 359);
    
    message.set_true_heading(511);
    EXPECT_EQ(message.get_true_heading(), 511);
    
    message.set_true_heading(360);
    EXPECT_EQ(message.get_true_heading(), 511);
    
    // Test timestamp
    message.set_timestamp(30);
    EXPECT_EQ(message.get_timestamp(), 30);
    
    message.set_timestamp(59);
    EXPECT_EQ(message.get_timestamp(), 59);
    
    message.set_timestamp(60);
    EXPECT_EQ(message.get_timestamp(), 60);
    
    message.set_timestamp(65);
    EXPECT_EQ(message.get_timestamp(), 60);
    
    // Test special maneuver indicator
    message.set_special_maneuver_indicator(1);
    EXPECT_EQ(message.get_special_maneuver_indicator(), 1);
    
    message.set_special_maneuver_indicator(2);
    EXPECT_EQ(message.get_special_maneuver_indicator(), 2);
    
    message.set_special_maneuver_indicator(3);
    EXPECT_EQ(message.get_special_maneuver_indicator(), 0);
    
    // Test RAIM flag
    message.set_raim_flag(true);
    EXPECT_TRUE(message.get_raim_flag());
    
    message.set_raim_flag(false);
    EXPECT_FALSE(message.get_raim_flag());
}

TEST(PositionReportClassATest, ToBits) {
    PositionReportClassA message(1, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    // Set some values
    message.set_rate_of_turn(10.5f);
    message.set_speed_over_ground(12.4f);
    message.set_position_accuracy(true);
    message.set_longitude(123.456789);
    message.set_latitude(45.678901);
    message.set_course_over_ground(123.4f);
    message.set_true_heading(123);
    message.set_timestamp(30);
    message.set_special_maneuver_indicator(1);
    message.set_raim_flag(true);
    
    // Convert to bits
    BitVector bits;
    message.to_bits(bits);
    
    // Check bit count
    EXPECT_EQ(bits.size(), 168);
    
    // Create a new message from bits
    PositionReportClassA message2(bits);
    
    // Check that the values match
    EXPECT_EQ(message2.get_message_type(), 1);
    EXPECT_EQ(message2.get_mmsi(), 123456789);
    EXPECT_EQ(message2.get_repeat_indicator(), 0);
    EXPECT_EQ(message2.get_navigation_status(), PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    EXPECT_NEAR(message2.get_rate_of_turn(), 10.5f, 0.5f);
    EXPECT_NEAR(message2.get_speed_over_ground(), 12.4f, 0.1f);
    EXPECT_TRUE(message2.get_position_accuracy());
    EXPECT_NEAR(message2.get_longitude(), 123.456789, 0.000001);
    EXPECT_NEAR(message2.get_latitude(), 45.678901, 0.000001);
    EXPECT_NEAR(message2.get_course_over_ground(), 123.4f, 0.1f);
    EXPECT_EQ(message2.get_true_heading(), 123);
    EXPECT_EQ(message2.get_timestamp(), 30);
    EXPECT_EQ(message2.get_special_maneuver_indicator(), 1);
    EXPECT_TRUE(message2.get_raim_flag());
}

TEST(PositionReportClassATest, ToStringNotAvailable) {
    PositionReportClassA message(1, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    // Set all values to "not available"
    message.set_rate_of_turn(std::numeric_limits<float>::quiet_NaN());
    message.set_speed_over_ground(std::numeric_limits<float>::quiet_NaN());
    message.set_position_accuracy(false);
    message.set_longitude(181.0);
    message.set_latitude(91.0);
    message.set_course_over_ground(std::numeric_limits<float>::quiet_NaN());
    message.set_true_heading(511);
    message.set_timestamp(60);
    message.set_special_maneuver_indicator(0);
    message.set_raim_flag(false);
    
    // Convert to string
    std::string str = message.to_string();
    
    // Check that the string contains appropriate "not available" messages
    EXPECT_NE(str.find("Not available"), std::string::npos);
}

TEST(PositionReportClassATest, FromBits) {
    // Create bits representing a valid message
    BitVector bits;
    
    // Message Type: 1 (6 bits)
    bits.append_uint(1, 6);
    
    // Repeat Indicator: 0 (2 bits)
    bits.append_uint(0, 2);
    
    // MMSI: 123456789 (30 bits)
    bits.append_uint(123456789, 30);
    
    // Navigation Status: 0 (4 bits)
    bits.append_uint(0, 4);
    
    // Rate of Turn: 0 (8 bits)
    bits.append_int(0, 8);
    
    // Speed Over Ground: 123 (10 bits)
    bits.append_uint(123, 10);
    
    // Position Accuracy: 1 (1 bit)
    bits.append_bit(true);
    
    // Longitude: 123.456789 degrees (28 bits)
    bits.append_int(static_cast<int32_t>(123.456789 * 600000.0), 28);
    
    // Latitude: 45.678901 degrees (27 bits)
    bits.append_int(static_cast<int32_t>(45.678901 * 600000.0), 27);
    
    // Course Over Ground: 123.4 degrees (12 bits)
    bits.append_uint(1234, 12);
    
    // True Heading: 123 degrees (9 bits)
    bits.append_uint(123, 9);
    
    // Timestamp: 30 seconds (6 bits)
    bits.append_uint(30, 6);
    
    // Special Maneuver Indicator: 1 (2 bits)
    bits.append_uint(1, 2);
    
    // Spare: 0 (3 bits)
    bits.append_uint(0, 3);
    
    // RAIM Flag: 1 (1 bit)
    bits.append_bit(true);
    
    // Radio Status: 0 (19 bits)
    bits.append_uint(0, 19);
    
    // Create message from bits
    PositionReportClassA message(bits);
    
    // Check values
    EXPECT_EQ(message.get_message_type(), 1);
    EXPECT_EQ(message.get_mmsi(), 123456789);
    EXPECT_EQ(message.get_repeat_indicator(), 0);
    EXPECT_EQ(message.get_navigation_status(), PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    EXPECT_EQ(message.get_rate_of_turn_raw(), 0);
    EXPECT_NEAR(message.get_speed_over_ground(), 12.3f, 0.1f);
    EXPECT_TRUE(message.get_position_accuracy());
    EXPECT_NEAR(message.get_longitude(), 123.456789, 0.000001);
    EXPECT_NEAR(message.get_latitude(), 45.678901, 0.000001);
    EXPECT_NEAR(message.get_course_over_ground(), 123.4f, 0.1f);
    EXPECT_EQ(message.get_true_heading(), 123);
    EXPECT_EQ(message.get_timestamp(), 30);
    EXPECT_EQ(message.get_special_maneuver_indicator(), 1);
    EXPECT_TRUE(message.get_raim_flag());
}

TEST(PositionReportClassATest, RateOfTurn) {
    PositionReportClassA message(1, 123456789, 0, PositionReportClassA::NavigationStatus::UNDER_WAY_USING_ENGINE);
    
    // Test rate of turn raw values
    
    // Not available
    message.set_rate_of_turn_raw(-128);
    EXPECT_TRUE(std::isnan(message.get_rate_of_turn()));
    
    // Turning left at more than 5 deg/30s
    message.set_rate_of_turn_raw(-127);
    EXPECT_TRUE(std::isinf(message.get_rate_of_turn()));
    EXPECT_LT(message.get_rate_of_turn(), 0.0f);
    
    // Turning right at more than 5 deg/30s
    message.set_rate_of_turn_raw(127);
    EXPECT_TRUE(std::isinf(message.get_rate_of_turn()));
    EXPECT_GT(message.get_rate_of_turn(), 0.0f);
    
    // No turn
    message.set_rate_of_turn_raw(0);
    EXPECT_EQ(message.get_rate_of_turn(), 0.0f);
    
    // Positive turn
    message.set_rate_of_turn_raw(1);
    EXPECT_GT(message.get_rate_of_turn(), 0.0f);
    
    // Negative turn
    message.set_rate_of_turn_raw(-1);
    EXPECT_LT(message.get_rate_of_turn(), 0.0f);
    
    // Test conversion from degrees per minute to raw values
    
    // Not available
    message.set_rate_of_turn(std::numeric_limits<float>::quiet_NaN());
    EXPECT_EQ(message.get_rate_of_turn_raw(), -128);
    
    // Extreme positive turn
    message.set_rate_of_turn(1000.0f);
    EXPECT_EQ(message.get_rate_of_turn_raw(), 127);
    
    // Extreme negative turn
    message.set_rate_of_turn(-1000.0f);
    EXPECT_EQ(message.get_rate_of_turn_raw(), -127);
    
    // No turn
    message.set_rate_of_turn(0.0f);
    EXPECT_EQ(message.get_rate_of_turn_raw(), 0);
    
    // Positive turn
    message.set_rate_of_turn(10.0f);
    EXPECT_GT(message.get_rate_of_turn_raw(), 0);
    
    // Negative turn
    message.set_rate_of_turn(-10.0f);
    EXPECT_LT(message.get_rate_of_turn_raw(), 0);
}