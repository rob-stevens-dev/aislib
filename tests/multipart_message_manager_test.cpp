#include <gtest/gtest.h>
#include "aislib/multipart_message_manager.h"
#include <string>
#include <thread>
#include <chrono>

using namespace aislib;

TEST(MultipartMessageManagerTest, SingleFragment) {
    MultipartMessageManager manager;
    
    // Add a single fragment
    std::string payload = "15MgK45P3@G?fl0E`JbR0OwT0@MS";
    auto bits = manager.add_fragment(1, 1, "1", 'A', payload, 0);
    
    // Check that the message is complete
    ASSERT_TRUE(bits.has_value());
    
    // Check the content
    BitVector expected(payload);
    EXPECT_EQ(bits->size(), expected.size());
    
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(bits->get_bit(i), expected.get_bit(i));
    }
}

TEST(MultipartMessageManagerTest, MultipleFragments) {
    MultipartMessageManager manager;
    
    // Add first fragment
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "2", 'A', payload1, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits1.has_value());
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 1);
    
    // Add second fragment
    std::string payload2 = "`JbR0OwT0@MS";
    auto bits2 = manager.add_fragment(2, 2, "2", 'A', payload2, 0);
    
    // Check that the message is complete
    ASSERT_TRUE(bits2.has_value());
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 0);
    
    // Check the content
    BitVector expected1(payload1);
    BitVector expected2(payload2);
    BitVector expected;
    
    for (size_t i = 0; i < expected1.size(); ++i) {
        expected.append_bit(expected1.get_bit(i));
    }
    
    for (size_t i = 0; i < expected2.size(); ++i) {
        expected.append_bit(expected2.get_bit(i));
    }
    
    EXPECT_EQ(bits2->size(), expected.size());
    
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(bits2->get_bit(i), expected.get_bit(i));
    }
}

TEST(MultipartMessageManagerTest, FragmentsWithFillBits) {
    MultipartMessageManager manager;
    
    // Add first fragment
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "3", 'A', payload1, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits1.has_value());
    
    // Add second fragment with fill bits
    std::string payload2 = "`JbR0OwT0@MS";
    auto bits2 = manager.add_fragment(2, 2, "3", 'A', payload2, 2);
    
    // Check that the message is complete
    ASSERT_TRUE(bits2.has_value());
    
    // Check the content
    BitVector expected1(payload1);
    
    // For the second fragment, we need to handle fill bits
    BitVector expected2;
    BitVector temp(payload2);
    
    // Copy all bits except the last 2 (fill bits)
    for (size_t i = 0; i < temp.size() - 2; ++i) {
        expected2.append_bit(temp.get_bit(i));
    }
    
    // Combine the fragments
    BitVector expected;
    
    for (size_t i = 0; i < expected1.size(); ++i) {
        expected.append_bit(expected1.get_bit(i));
    }
    
    for (size_t i = 0; i < expected2.size(); ++i) {
        expected.append_bit(expected2.get_bit(i));
    }
    
    EXPECT_EQ(bits2->size(), expected.size());
    
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(bits2->get_bit(i), expected.get_bit(i));
    }
}

TEST(MultipartMessageManagerTest, OutOfOrderFragments) {
    MultipartMessageManager manager;
    
    // Add second fragment first
    std::string payload2 = "`JbR0OwT0@MS";
    auto bits2 = manager.add_fragment(2, 2, "4", 'A', payload2, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits2.has_value());
    
    // Add first fragment
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "4", 'A', payload1, 0);
    
    // Check that the message is complete
    ASSERT_TRUE(bits1.has_value());
    
    // Check the content (same as MultipleFragments test)
    BitVector expected1(payload1);
    BitVector expected2(payload2);
    BitVector expected;
    
    for (size_t i = 0; i < expected1.size(); ++i) {
        expected.append_bit(expected1.get_bit(i));
    }
    
    for (size_t i = 0; i < expected2.size(); ++i) {
        expected.append_bit(expected2.get_bit(i));
    }
    
    EXPECT_EQ(bits1->size(), expected.size());
    
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(bits1->get_bit(i), expected.get_bit(i));
    }
}

TEST(MultipartMessageManagerTest, DuplicateFragments) {
    MultipartMessageManager manager;
    
    // Add first fragment
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "5", 'A', payload1, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits1.has_value());
    
    // Add first fragment again (should be ignored)
    auto bits1_dup = manager.add_fragment(1, 2, "5", 'A', payload1, 0);
    
    // Check that the message is still not complete
    EXPECT_FALSE(bits1_dup.has_value());
    
    // Add second fragment
    std::string payload2 = "`JbR0OwT0@MS";
    auto bits2 = manager.add_fragment(2, 2, "5", 'A', payload2, 0);
    
    // Check that the message is complete
    ASSERT_TRUE(bits2.has_value());
}

TEST(MultipartMessageManagerTest, DifferentChannels) {
    MultipartMessageManager manager;
    
    // Add first fragment on channel A
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "6", 'A', payload1, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits1.has_value());
    
    // Add second fragment on channel B (should be treated as different message)
    std::string payload2 = "`JbR0OwT0@MS";
    auto bits2 = manager.add_fragment(2, 2, "6", 'B', payload2, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits2.has_value());
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 2);
    
    // Add second fragment on channel A
    auto bits3 = manager.add_fragment(2, 2, "6", 'A', payload2, 0);
    
    // Check that the message is complete
    ASSERT_TRUE(bits3.has_value());
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 1);
}

TEST(MultipartMessageManagerTest, Timeout) {
    // Create manager with short timeout
    MultipartMessageManager manager(std::chrono::seconds(1));
    
    // Add first fragment
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "7", 'A', payload1, 0);
    
    // Check that the message is not complete
    EXPECT_FALSE(bits1.has_value());
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 1);
    
    // Wait for timeout
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Clean up expired messages
    manager.cleanup_expired();
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 0);
}

TEST(MultipartMessageManagerTest, MaxMessages) {
    // Create manager with small max messages
    MultipartMessageManager manager(std::chrono::seconds(60), 2);
    
    // Add first fragment of first message
    std::string payload1 = "55MgK45P3@G?fl0E";
    auto bits1 = manager.add_fragment(1, 2, "8", 'A', payload1, 0);
    
    // Add first fragment of second message
    auto bits2 = manager.add_fragment(1, 2, "9", 'A', payload1, 0);
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 2);
    
    // Add first fragment of third message (should cause cleanup)
    auto bits3 = manager.add_fragment(1, 2, "10", 'A', payload1, 0);
    
    // Check incomplete count (oldest message should be removed)
    EXPECT_EQ(manager.get_incomplete_count(), 2);
}

TEST(MultipartMessageManagerTest, Clear) {
    MultipartMessageManager manager;
    
    // Add some fragments
    std::string payload = "55MgK45P3@G?fl0E";
    manager.add_fragment(1, 2, "11", 'A', payload, 0);
    manager.add_fragment(1, 2, "12", 'A', payload, 0);
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 2);
    
    // Clear
    manager.clear();
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 0);
}

TEST(MultipartMessageManagerTest, SetTimeout) {
    MultipartMessageManager manager;
    
    // Add a fragment
    std::string payload = "55MgK45P3@G?fl0E";
    manager.add_fragment(1, 2, "13", 'A', payload, 0);
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 1);
    
    // Set short timeout
    manager.set_timeout(std::chrono::seconds(1));
    
    // Wait for timeout
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Clean up expired messages
    manager.cleanup_expired();
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 0);
}

TEST(MultipartMessageManagerTest, SetMaxMessages) {
    MultipartMessageManager manager;
    
    // Add some fragments
    std::string payload = "55MgK45P3@G?fl0E";
    manager.add_fragment(1, 2, "14", 'A', payload, 0);
    manager.add_fragment(1, 2, "15", 'A', payload, 0);
    manager.add_fragment(1, 2, "16", 'A', payload, 0);
    
    // Check incomplete count
    EXPECT_EQ(manager.get_incomplete_count(), 3);
    
    // Set small max messages
    manager.set_max_messages(1);
    
    // Check incomplete count - should have been reduced to 1
    EXPECT_EQ(manager.get_incomplete_count(), 1);
}

TEST(MultipartMessageManagerTest, InvalidFragmentNumber) {
    MultipartMessageManager manager;
    
    // Fragment number 0
    EXPECT_THROW(manager.add_fragment(0, 2, "18", 'A', "payload", 0), std::invalid_argument);
    
    // Fragment number > fragment count
    EXPECT_THROW(manager.add_fragment(3, 2, "18", 'A', "payload", 0), std::invalid_argument);
}

TEST(MultipartMessageManagerTest, InvalidChannel) {
    MultipartMessageManager manager;
    
    // Invalid channel
    EXPECT_THROW(manager.add_fragment(1, 2, "19", 'C', "payload", 0), std::invalid_argument);
}

TEST(MultipartMessageManagerTest, InvalidFillBits) {
    MultipartMessageManager manager;
    
    // Invalid fill bits
    EXPECT_THROW(manager.add_fragment(1, 2, "20", 'A', "payload", 6), std::invalid_argument);
}