#include <gtest/gtest.h>
#include "aislib/position_report_class_b.h"
#include "aislib/bit_vector.h"
#include <string>
#include <memory>

using namespace aislib;

class StandardPositionReportClassBTest_Constructor_Test : public ::testing::Test {
protected:
    StandardPositionReportClassB message;

    StandardPositionReportClassBTest_Constructor_Test() : message(123456789, 0) {
    }
};

TEST_F(StandardPositionReportClassBTest_Constructor_Test, DefaultValues) {
    // Check message type
    EXPECT_EQ(message.get_message_type(), 18);

    // Check MMSI and repeat indicator
    EXPECT_EQ(message.get_mmsi(), 123456789);
    EXPECT_EQ(message.get_repeat_indicator(), 0);

    // Check default values for navigation data
    EXPECT_FLOAT_EQ(message.get_speed_over_ground(), -1.0f);  // Not available
    EXPECT_FALSE(message.get_position_accuracy());
    EXPECT_GT(message.get_longitude(), 180.0);  // Not available
    EXPECT_GT(message.get_latitude(), 90.0);  // Not available
    EXPECT_FLOAT_EQ(message.get_course_over_ground(), -1.0f);  // Not available
    EXPECT_EQ(message.get_true_heading(), 511);  // Not available
    EXPECT_EQ(message.get_timestamp(), 60);  // Not available

    // Check default flags
    EXPECT_TRUE(message.get_cs_flag());
    EXPECT_FALSE(message.get_display_flag());
    EXPECT_FALSE(message.get_dsc_flag());
    EXPECT_FALSE(message.get_band_flag());
    EXPECT_FALSE(message.get_message_22_flag());
    EXPECT_FALSE(message.get_assigned_flag());
    EXPECT_FALSE(message.get_raim_flag());
}

class StandardPositionReportClassBTest_Navigation_Test : public ::testing::Test {
protected:
    StandardPositionReportClassB message;

    StandardPositionReportClassBTest_Navigation_Test() : message(123456789, 0) {
    }
};

TEST_F(StandardPositionReportClassBTest_Navigation_Test, SetValues) {
    // Set navigation values
    message.set_speed_over_ground(10.5f);
    message.set_position_accuracy(true);
    message.set_longitude(45.5);
    message.set_latitude(-30.2);
    message.set_course_over_ground(237.5f);
    message.set_true_heading(235);
    message.set_timestamp(45);

    // Check values
    EXPECT_FLOAT_EQ(message.get_speed_over_ground(), 10.5f);
    EXPECT_TRUE(message.get_position_accuracy());
    EXPECT_DOUBLE_EQ(message.get_longitude(), 45.5);
    EXPECT_DOUBLE_EQ(message.get_latitude(), -30.2);
    EXPECT_FLOAT_EQ(message.get_course_over_ground(), 237.5f);
    EXPECT_EQ(message.get_true_heading(), 235);
    EXPECT_EQ(message.get_timestamp(), 45);
}

class StandardPositionReportClassBTest_Flags_Test : public ::testing::Test {
protected:
    StandardPositionReportClassB message;

    StandardPositionReportClassBTest_Flags_Test() : message(123456789, 0) {
    }
};

TEST_F(StandardPositionReportClassBTest_Flags_Test, SetFlags) {
    // Set flag values
    message.set_cs_flag(false);
    message.set_display_flag(true);
    message.set_dsc_flag(true);
    message.set_band_flag(true);
    message.set_message_22_flag(true);
    message.set_assigned_flag(true);
    message.set_raim_flag(true);
    message.set_radio_status(0x12345);

    // Check values
    EXPECT_FALSE(message.get_cs_flag());
    EXPECT_TRUE(message.get_display_flag());
    EXPECT_TRUE(message.get_dsc_flag());
    EXPECT_TRUE(message.get_band_flag());
    EXPECT_TRUE(message.get_message_22_flag());
    EXPECT_TRUE(message.get_assigned_flag());
    EXPECT_TRUE(message.get_raim_flag());
    EXPECT_EQ(message.get_radio_status(), 0x12345);
}

class StandardPositionReportClassBTest_Serialization_Test : public ::testing::Test {
protected:
    StandardPositionReportClassB message;

    StandardPositionReportClassBTest_Serialization_Test() : message(123456789, 0) {
        // Set some values
        message.set_speed_over_ground(10.5f);
        message.set_position_accuracy(true);
        message.set_longitude(45.5);
        message.set_latitude(-30.2);
        message.set_course_over_ground(237.5f);
        message.set_true_heading(235);
        message.set_timestamp(45);
        message.set_cs_flag(false);
        message.set_display_flag(true);
        message.set_dsc_flag(true);
        message.set_band_flag(true);
        message.set_message_22_flag(true);
        message.set_assigned_flag(false);
        message.set_raim_flag(true);
        message.set_radio_status(0x12345);
    }
};

TEST_F(StandardPositionReportClassBTest_Serialization_Test, SerializeDeserialize) {
    // Serialize to BitVector
    BitVector bits;
    message.to_bits(bits);

    // Deserialize
    auto deserialized = std::make_unique<StandardPositionReportClassB>(bits);

    // Check values
    EXPECT_EQ(deserialized->get_message_type(), 18);
    EXPECT_EQ(deserialized->get_mmsi(), 123456789);
    EXPECT_EQ(deserialized->get_repeat_indicator(), 0);
    EXPECT_FLOAT_EQ(deserialized->get_speed_over_ground(), 10.5f);
    EXPECT_TRUE(deserialized->get_position_accuracy());
    EXPECT_DOUBLE_EQ(deserialized->get_longitude(), 45.5);
    EXPECT_DOUBLE_EQ(deserialized->get_latitude(), -30.2);
    EXPECT_FLOAT_EQ(deserialized->get_course_over_ground(), 237.5f);
    EXPECT_EQ(deserialized->get_true_heading(), 235);
    EXPECT_EQ(deserialized->get_timestamp(), 45);
    EXPECT_FALSE(deserialized->get_cs_flag());
    EXPECT_TRUE(deserialized->get_display_flag());
    EXPECT_TRUE(deserialized->get_dsc_flag());
    EXPECT_TRUE(deserialized->get_band_flag());
    EXPECT_TRUE(deserialized->get_message_22_flag());
    EXPECT_FALSE(deserialized->get_assigned_flag());
    EXPECT_TRUE(deserialized->get_raim_flag());
    EXPECT_EQ(deserialized->get_radio_status(), 0x12345);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}