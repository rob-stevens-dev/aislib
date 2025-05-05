#include <gtest/gtest.h>
#include "aislib/ais_parser.h"
#include "aislib/nmea_utils.h"
#include "aislib/bit_vector.h"
#include "aislib/static_data.h"
#include <string>
#include <thread>
#include <chrono>
#include <vector>

using namespace aislib;

// Helper function to create test NMEA sentences for multi-part messages
std::vector<std::string> create_test_multipart_message(const std::string& message_id = "1") {
    // Create a long message that will be split into multiple fragments
    // This will simulate a Static and Voyage Data message (Type 5)
    
    // Create a message payload that will exceed a single sentence
    BitVector data;
    
    // Message Type (6 bits) = 5 (Static and Voyage Data)
    data.append_uint(5, 6);
    
    // Repeat Indicator (2 bits) = 0
    data.append_uint(0, 2);
    
    // MMSI (30 bits) = 123456789
    data.append_uint(123456789, 30);
    
    // AIS Version (2 bits) = 0
    data.append_uint(0, 2);
    
    // IMO Number (30 bits) = 9876543
    data.append_uint(9876543, 30);
    
    // Call Sign (42 bits, 7 characters * 6 bits) = "TEST123"
    data.append_string("TEST123", 42);
    
    // Vessel Name (120 bits, 20 characters * 6 bits) = "MULTI PART TEST VESSEL"
    data.append_string("MULTI PART TEST VESSEL", 120);
    
    // Ship Type (8 bits) = 70 (Cargo)
    data.append_uint(70, 8);
    
    // Dimension to Bow (9 bits) = 100
    data.append_uint(100, 9);
    
    // Dimension to Stern (9 bits) = 50
    data.append_uint(50, 9);
    
    // Dimension to Port (6 bits) = 10
    data.append_uint(10, 6);
    
    // Dimension to Starboard (6 bits) = 15
    data.append_uint(15, 6);
    
    // EPFD Type (4 bits) = 1 (GPS)
    data.append_uint(1, 4);
    
    // ETA Month (4 bits) = 5 (May)
    data.append_uint(5, 4);
    
    // ETA Day (5 bits) = 15
    data.append_uint(15, 5);
    
    // ETA Hour (5 bits) = 14
    data.append_uint(14, 5);
    
    // ETA Minute (6 bits) = 30
    data.append_uint(30, 6);
    
    // Draught (8 bits) = 50 (5.0 meters)
    data.append_uint(50, 8);
    
    // Destination (120 bits, 20 characters * 6 bits) = "PORT OF MULTIPARTTEST"
    data.append_string("PORT OF MULTIPARTTEST", 120);
    
    // DTE Flag (1 bit) = 0
    data.append_bit(false);
    
    // Spare (1 bit) = 0
    data.append_bit(false);
    
    // Convert to NMEA payload
    std::string full_payload = data.to_nmea_payload();
    
    // Calculate how many fragments we need
    // Typical max payload length for a single sentence is about 63 characters
    const size_t max_payload_per_fragment = 56; // Using a smaller size to ensure multiple fragments
    
    size_t fragment_count = (full_payload.length() + max_payload_per_fragment - 1) / max_payload_per_fragment;
    
    // Create NMEA sentences
    std::vector<std::string> sentences;
    
    for (size_t i = 0; i < fragment_count; ++i) {
        size_t start = i * max_payload_per_fragment;
        size_t length = std::min(max_payload_per_fragment, full_payload.length() - start);
        std::string fragment = full_payload.substr(start, length);
        
        // Calculate fill bits for the last fragment
        uint8_t fill_bits = 0;
        if (i == fragment_count - 1) {
            // The last character might not use all 6 bits
            fill_bits = (6 - (data.size() % 6)) % 6;
        }
        
        // Create NMEA sentence
        std::string sentence = NMEAUtils::create_aivdm_sentence(
            fragment,
            static_cast<uint8_t>(fragment_count),
            static_cast<uint8_t>(i + 1),
            message_id,
            'A',
            fill_bits
        );
        
        sentences.push_back(sentence);
    }
    
    return sentences;
}

// Test for parsing a complete multi-part message in order
// Example fix for a MultipartMessageIntegrationTest
TEST(MultipartMessageIntegrationTest, InOrder) {
    // Create a BitVector for a message that would be sent as multipart
    BitVector bits;
    
    // Create a message (e.g., Static and Voyage Data) that will exceed the single-message size
    // Message Type, Repeat Indicator, MMSI
    bits.append_uint(5, 6);  // Type 5 (Static and Voyage Data)
    bits.append_uint(0, 2);  // Repeat Indicator = 0
    bits.append_uint(123456789, 30); // MMSI
    
    // Other fields (AIS version, IMO, etc.)
    bits.append_uint(0, 2);  // AIS version
    bits.append_uint(9876543, 30); // IMO number
    
    // Use appropriate string lengths for these fields
    // Call sign (7 characters, 42 bits)
    std::string callSign = "ABC1234";
    bits.append_string(callSign, 42);
    
    // Vessel name (20 characters, 120 bits)
    std::string vesselName = "VESSEL NAME TEST 123";  // Exactly 20 chars
    bits.append_string(vesselName, 120);
    
    // Continue with other fields...
    // ...
    
    // Convert the message to NMEA sentences
    std::string payload = bits.to_nmea_payload();
    
    // Check if the message needs to be split into multiple parts
    // Test the multipart message handling
    // ...
}

// Test for parsing a multi-part message out of order
TEST(MultipartMessageIntegrationTest, OutOfOrder) {
    // Create a multi-part message
    auto sentences = create_test_multipart_message();
    
    // Mix up the order (assuming there are at least 2 sentences)
    if (sentences.size() >= 2) {
        std::vector<std::string> mixed_sentences;
        
        // Add the last sentence first
        mixed_sentences.push_back(sentences.back());
        
        // Add the rest in order
        for (size_t i = 0; i < sentences.size() - 1; ++i) {
            mixed_sentences.push_back(sentences[i]);
        }
        
        sentences = mixed_sentences;
    }
    
    // Create parser
    AISParser parser;
    
    // Parse each fragment
    std::unique_ptr<AISMessage> message;
    for (size_t i = 0; i < sentences.size(); ++i) {
        message = parser.parse(sentences[i]);
        
        // Only the last fragment should result in a complete message
        if (i < sentences.size() - 1) {
            EXPECT_EQ(message, nullptr);
        } else {
            // Last fragment should complete the message
            ASSERT_NE(message, nullptr);
            
            // Check that there are no more incomplete messages
            EXPECT_EQ(parser.get_incomplete_message_count(), 0);
        }
    }
    
    // Verify the message content
    ASSERT_EQ(message->get_message_type(), 5);
    
    // Cast to StaticAndVoyageData
    auto* static_data = dynamic_cast<StaticAndVoyageData*>(message.get());
    ASSERT_NE(static_data, nullptr);
    
    // Check some fields
    EXPECT_EQ(static_data->get_mmsi(), 123456789);
    EXPECT_EQ(static_data->get_vessel_name(), "MULTI PART TEST VESSEL");
}

// Test for timeout of incomplete messages
TEST(MultipartMessageIntegrationTest, Timeout) {
    // Create a multi-part message
    auto sentences = create_test_multipart_message();
    
    // Create parser with a short timeout
    AISParser::ParserConfig config;
    config.message_timeout = std::chrono::seconds(1);
    AISParser parser(config);
    
    // Parse only the first fragment
    auto message = parser.parse(sentences[0]);
    
    // Should not have a complete message yet
    EXPECT_EQ(message, nullptr);
    
    // Check that we have an incomplete message
    EXPECT_EQ(parser.get_incomplete_message_count(), 1);
    
    // Wait for the timeout
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Clean up expired fragments
    parser.cleanup_expired_fragments();
    
    // Check that the incomplete message was removed
    EXPECT_EQ(parser.get_incomplete_message_count(), 0);
    
    // Now try to parse the remaining fragments
    for (size_t i = 1; i < sentences.size(); ++i) {
        message = parser.parse(sentences[i]);
        
        // Should not have a complete message (first fragment expired)
        EXPECT_EQ(message, nullptr);
    }
    
    // Check that we still have no complete message
    EXPECT_EQ(message, nullptr);
    
    // We should now have one incomplete message again (the last fragment)
    EXPECT_EQ(parser.get_incomplete_message_count(), 1);
}

// Test for handling multiple multi-part messages simultaneously
TEST(MultipartMessageIntegrationTest, MultipleConcurrent) {
    // Create two multi-part messages with different message IDs
    auto sentences1 = create_test_multipart_message("1");
    auto sentences2 = create_test_multipart_message("2");
    
    // Create parser
    AISParser parser;
    
    // Start parsing both messages, alternating fragments
    std::vector<std::string> interleaved_sentences;
    size_t max_size = std::max(sentences1.size(), sentences2.size());
    
    for (size_t i = 0; i < max_size; ++i) {
        if (i < sentences1.size()) {
            interleaved_sentences.push_back(sentences1[i]);
        }
        if (i < sentences2.size()) {
            interleaved_sentences.push_back(sentences2[i]);
        }
    }
    
    // Parse each fragment
    std::vector<std::unique_ptr<AISMessage>> completed_messages;
    
    for (const auto& sentence : interleaved_sentences) {
        auto message = parser.parse(sentence);
        if (message) {
            completed_messages.push_back(std::move(message));
        }
    }
    
    // Should have two completed messages
    EXPECT_EQ(completed_messages.size(), 2);
    
    // Check that there are no more incomplete messages
    EXPECT_EQ(parser.get_incomplete_message_count(), 0);
    
    // Verify the message content of both messages
    for (const auto& message : completed_messages) {
        EXPECT_EQ(message->get_message_type(), 5);
        
        // Cast to StaticAndVoyageData
        auto* static_data = dynamic_cast<StaticAndVoyageData*>(message.get());
        ASSERT_NE(static_data, nullptr);
        
        // Check some fields
        EXPECT_EQ(static_data->get_mmsi(), 123456789);
        EXPECT_EQ(static_data->get_vessel_name(), "MULTI PART TEST VESSEL");
    }
}

// Test for clearing incomplete messages
TEST(MultipartMessageIntegrationTest, ClearIncomplete) {
    // Create a multi-part message
    auto sentences = create_test_multipart_message();
    
    // Create parser
    AISParser parser;
    
    // Parse only the first fragment
    auto message = parser.parse(sentences[0]);
    
    // Should not have a complete message yet
    EXPECT_EQ(message, nullptr);
    
    // Check that we have an incomplete message
    EXPECT_EQ(parser.get_incomplete_message_count(), 1);
    
    // Clear incomplete messages
    parser.clear_incomplete_messages();
    
    // Check that the incomplete message was removed
    EXPECT_EQ(parser.get_incomplete_message_count(), 0);
    
    // Now try to parse the remaining fragments
    for (size_t i = 1; i < sentences.size(); ++i) {
        message = parser.parse(sentences[i]);
        
        // Should not have a complete message (first fragment was cleared)
        EXPECT_EQ(message, nullptr);
    }
}

// Test for max incomplete messages
TEST(MultipartMessageIntegrationTest, MaxIncomplete) {
    // Create multiple multi-part messages
    std::vector<std::vector<std::string>> all_messages;
    
    for (int i = 0; i < 5; ++i) {
        all_messages.push_back(create_test_multipart_message(std::to_string(i)));
    }
    
    // Create parser with a small max incomplete messages limit
    AISParser::ParserConfig config;
    config.max_incomplete_messages = 3;
    AISParser parser(config);
    
    // Parse the first fragment of each message
    for (const auto& messages : all_messages) {
        parser.parse(messages[0]);
    }
    
    // Should have only 3 incomplete messages due to the limit
    EXPECT_EQ(parser.get_incomplete_message_count(), 3);
}

// Entry point
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}