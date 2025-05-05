/**
 * @file multipart_example.cpp
 * @brief Example application for demonstrating multi-part message handling
 * 
 * This example shows how to use the AISParser to handle multi-part AIS messages
 * in various scenarios, including:
 * - Processing fragments in order
 * - Processing fragments out of order
 * - Handling timeouts
 * - Managing multiple concurrent multi-part messages
 */

 #include "aislib/ais_parser.h"
 #include "aislib/nmea_utils.h"
 #include "aislib/bit_vector.h"
 #include "aislib/static_data.h"
 #include <iostream>
 #include <string>
 #include <vector>
 #include <chrono>
 #include <thread>
 #include <iomanip>
 #include <algorithm> // For std::max
 
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
 
 // Helper function to print message details
 void print_message_details(const AISMessage* message) {
     if (!message) {
         std::cout << "  No message available" << std::endl;
         return;
     }
     
     std::cout << "  Message Type: " << static_cast<int>(message->get_message_type()) << std::endl;
     std::cout << "  MMSI: " << message->get_mmsi() << std::endl;
     
     // Check if it's a Static and Voyage Data message
     if (message->get_message_type() == 5) {
         const auto* static_data = dynamic_cast<const StaticAndVoyageData*>(message);
         if (static_data) {
             std::cout << "  Vessel Name: " << static_data->get_vessel_name() << std::endl;
             std::cout << "  Call Sign: " << static_data->get_call_sign() << std::endl;
             std::cout << "  IMO Number: " << static_data->get_imo_number() << std::endl;
             std::cout << "  Destination: " << static_data->get_destination() << std::endl;
             std::cout << "  Dimensions: " 
                       << static_data->get_dimension_to_bow() << "m (bow) x "
                       << static_data->get_dimension_to_stern() << "m (stern) x "
                       << static_cast<int>(static_data->get_dimension_to_port()) << "m (port) x "
                       << static_cast<int>(static_data->get_dimension_to_starboard()) << "m (starboard)" 
                       << std::endl;
             
             // ETA
             std::cout << "  ETA: ";
             if (static_data->get_eta_month() == 0 || static_data->get_eta_day() == 0) {
                 std::cout << "Not available" << std::endl;
             } else {
                 std::cout << "Month " << static_cast<int>(static_data->get_eta_month())
                           << ", Day " << static_cast<int>(static_data->get_eta_day())
                           << ", " << static_cast<int>(static_data->get_eta_hour())
                           << ":" << std::setw(2) << std::setfill('0') 
                           << static_cast<int>(static_data->get_eta_minute()) << " UTC" 
                           << std::endl;
             }
         }
     }
 }
 
 // Example 1: Processing fragments in order
 void example_in_order() {
     std::cout << "Example 1: Processing fragments in order" << std::endl;
     std::cout << "=========================================" << std::endl;
     
     // Create a multi-part message
     auto sentences = create_test_multipart_message();
     
     std::cout << "Created a multi-part message with " << sentences.size() 
               << " fragments." << std::endl;
     
     // Create parser
     AISParser parser;
     
     // Process fragments in order
     std::cout << "Processing fragments in order:" << std::endl;
     
     for (size_t i = 0; i < sentences.size(); ++i) {
         std::cout << "Fragment " << (i + 1) << "/" << sentences.size() << ":" << std::endl;
         std::cout << "  " << sentences[i] << std::endl;
         
         auto message = parser.parse(sentences[i]);
         
         if (message) {
             std::cout << "  -> Complete message received!" << std::endl;
             print_message_details(message.get());
         } else {
             auto error = parser.get_last_error();
             if (error.type == AISParser::ParseError::ErrorType::NONE) {
                 std::cout << "  -> Fragment processed, message incomplete. "
                           << "Incomplete count: " << parser.get_incomplete_message_count() 
                           << std::endl;
             } else {
                 std::cout << "  -> Error: " << error.message << std::endl;
             }
         }
         
         std::cout << std::endl;
     }
     
     std::cout << std::endl;
 }
 
 // Example 2: Processing fragments out of order
 void example_out_of_order() {
     std::cout << "Example 2: Processing fragments out of order" << std::endl;
     std::cout << "============================================" << std::endl;
     
     // Create a multi-part message
     auto sentences = create_test_multipart_message();
     
     std::cout << "Created a multi-part message with " << sentences.size() 
               << " fragments." << std::endl;
     
     // Mix up the order
     if (sentences.size() >= 2) {
         // Start with the middle fragment
         size_t mid = sentences.size() / 2;
         std::vector<std::string> mixed_sentences;
         
         // Add middle fragment first
         mixed_sentences.push_back(sentences[mid]);
         
         // Add the rest in reverse order
         for (size_t i = sentences.size() - 1; i > mid; --i) {
             mixed_sentences.push_back(sentences[i]);
         }
         
         // Add the first fragments last
         for (size_t i = 0; i < mid; ++i) {
             mixed_sentences.push_back(sentences[i]);
         }
         
         sentences = mixed_sentences;
     }
     
     // Create parser
     AISParser parser;
     
     // Process fragments in the mixed order
     std::cout << "Processing fragments out of order:" << std::endl;
     
     for (size_t i = 0; i < sentences.size(); ++i) {
         // Extract the fragment number from the sentence
         std::vector<std::string> fields = NMEAUtils::parse_fields(sentences[i]);
         int fragment_num = std::stoi(fields[2]);
         int fragment_count = std::stoi(fields[1]);
         
         std::cout << "Fragment " << fragment_num << "/" << fragment_count 
                   << " (position " << (i + 1) << " in sequence):" << std::endl;
         std::cout << "  " << sentences[i] << std::endl;
         
         auto message = parser.parse(sentences[i]);
         
         if (message) {
             std::cout << "  -> Complete message received!" << std::endl;
             print_message_details(message.get());
         } else {
             auto error = parser.get_last_error();
             if (error.type == AISParser::ParseError::ErrorType::NONE) {
                 std::cout << "  -> Fragment processed, message incomplete. "
                           << "Incomplete count: " << parser.get_incomplete_message_count() 
                           << std::endl;
             } else {
                 std::cout << "  -> Error: " << error.message << std::endl;
             }
         }
         
         std::cout << std::endl;
     }
     
     std::cout << std::endl;
 }
 
 // Example 3: Handling timeouts
 void example_timeout() {
     std::cout << "Example 3: Handling timeouts" << std::endl;
     std::cout << "============================" << std::endl;
     
     // Create a multi-part message
     auto sentences = create_test_multipart_message();
     
     std::cout << "Created a multi-part message with " << sentences.size() 
               << " fragments." << std::endl;
     
     // Create parser with a short timeout
     AISParser::ParserConfig config;
     config.message_timeout = std::chrono::seconds(3);
     AISParser parser(config);
     
     std::cout << "Configured parser with a 3-second timeout." << std::endl;
     
     // Process only the first fragment
     std::cout << "Processing first fragment:" << std::endl;
     std::cout << "  " << sentences[0] << std::endl;
     
     auto message = parser.parse(sentences[0]);
     
     if (!message) {
         auto error = parser.get_last_error();
         if (error.type == AISParser::ParseError::ErrorType::NONE) {
             std::cout << "  -> Fragment processed, message incomplete. "
                       << "Incomplete count: " << parser.get_incomplete_message_count() 
                       << std::endl;
         } else {
             std::cout << "  -> Error: " << error.message << std::endl;
         }
     }
     
     std::cout << std::endl;
     
     // Wait for the timeout
     std::cout << "Waiting for timeout (4 seconds)..." << std::endl;
     std::this_thread::sleep_for(std::chrono::seconds(4));
     
     // Clean up expired fragments
     std::cout << "Cleaning up expired fragments..." << std::endl;
     parser.cleanup_expired_fragments();
     std::cout << "  Incomplete count after cleanup: " << parser.get_incomplete_message_count() << std::endl;
     
     std::cout << std::endl;
     
     // Try to process the last fragment
     std::cout << "Processing last fragment:" << std::endl;
     std::cout << "  " << sentences.back() << std::endl;
     
     message = parser.parse(sentences.back());
     
     if (message) {
         std::cout << "  -> Complete message received! (This shouldn't happen)" << std::endl;
     } else {
         auto error = parser.get_last_error();
         if (error.type == AISParser::ParseError::ErrorType::NONE) {
             std::cout << "  -> Fragment processed, message incomplete (as expected). "
                       << "Incomplete count: " << parser.get_incomplete_message_count() 
                       << std::endl;
             std::cout << "  -> The message remains incomplete because the first fragment timed out." << std::endl;
         } else {
             std::cout << "  -> Error: " << error.message << std::endl;
         }
     }
     
     std::cout << std::endl;
 }
 
 // Example 4: Managing multiple concurrent multi-part messages
 void example_multiple_concurrent() {
     std::cout << "Example 4: Managing multiple concurrent multi-part messages" << std::endl;
     std::cout << "=======================================================" << std::endl;
     
     // Create three multi-part messages with different message IDs
     auto sentences1 = create_test_multipart_message("1");
     auto sentences2 = create_test_multipart_message("2");
     auto sentences3 = create_test_multipart_message("3");
     
     std::cout << "Created three multi-part messages:" << std::endl;
     std::cout << "  Message 1: " << sentences1.size() << " fragments" << std::endl;
     std::cout << "  Message 2: " << sentences2.size() << " fragments" << std::endl;
     std::cout << "  Message 3: " << sentences3.size() << " fragments" << std::endl;
     
     // Create parser
     AISParser parser;
     
     // Process fragments in an interleaved manner
     std::cout << "Processing fragments from all messages in an interleaved manner:" << std::endl;
     
     // Fix: Use individual std::max calls instead of initializer list
     size_t max_size = std::max(sentences1.size(), 
                       std::max(sentences2.size(), sentences3.size()));
     int message_count = 0;
     
     for (size_t i = 0; i < max_size; ++i) {
         // Process fragment from message 1 if available
         if (i < sentences1.size()) {
             std::vector<std::string> fields = NMEAUtils::parse_fields(sentences1[i]);
             int fragment_num = std::stoi(fields[2]);
             int fragment_count = std::stoi(fields[1]);
             std::string message_id = fields[3];
             
             std::cout << "Message 1, Fragment " << fragment_num << "/" << fragment_count << ":" << std::endl;
             std::cout << "  " << sentences1[i] << std::endl;
             
             auto message = parser.parse(sentences1[i]);
             
             if (message) {
                 std::cout << "  -> Complete message 1 received!" << std::endl;
                 print_message_details(message.get());
                 message_count++;
             } else {
                 auto error = parser.get_last_error();
                 if (error.type == AISParser::ParseError::ErrorType::NONE) {
                     std::cout << "  -> Fragment processed, message incomplete. "
                               << "Incomplete count: " << parser.get_incomplete_message_count() 
                               << std::endl;
                 } else {
                     std::cout << "  -> Error: " << error.message << std::endl;
                 }
             }
             
             std::cout << std::endl;
         }
         
         // Process fragment from message 2 if available
         if (i < sentences2.size()) {
             std::vector<std::string> fields = NMEAUtils::parse_fields(sentences2[i]);
             int fragment_num = std::stoi(fields[2]);
             int fragment_count = std::stoi(fields[1]);
             std::string message_id = fields[3];
             
             std::cout << "Message 2, Fragment " << fragment_num << "/" << fragment_count << ":" << std::endl;
             std::cout << "  " << sentences2[i] << std::endl;
             
             auto message = parser.parse(sentences2[i]);
             
             if (message) {
                 std::cout << "  -> Complete message 2 received!" << std::endl;
                 print_message_details(message.get());
                 message_count++;
             } else {
                 auto error = parser.get_last_error();
                 if (error.type == AISParser::ParseError::ErrorType::NONE) {
                     std::cout << "  -> Fragment processed, message incomplete. "
                               << "Incomplete count: " << parser.get_incomplete_message_count() 
                               << std::endl;
                 } else {
                     std::cout << "  -> Error: " << error.message << std::endl;
                 }
             }
             
             std::cout << std::endl;
         }
         
         // Process fragment from message 3 if available
         if (i < sentences3.size()) {
             std::vector<std::string> fields = NMEAUtils::parse_fields(sentences3[i]);
             int fragment_num = std::stoi(fields[2]);
             int fragment_count = std::stoi(fields[1]);
             std::string message_id = fields[3];
             
             std::cout << "Message 3, Fragment " << fragment_num << "/" << fragment_count << ":" << std::endl;
             std::cout << "  " << sentences3[i] << std::endl;
             
             auto message = parser.parse(sentences3[i]);
             
             if (message) {
                 std::cout << "  -> Complete message 3 received!" << std::endl;
                 print_message_details(message.get());
                 message_count++;
             } else {
                 auto error = parser.get_last_error();
                 if (error.type == AISParser::ParseError::ErrorType::NONE) {
                     std::cout << "  -> Fragment processed, message incomplete. "
                               << "Incomplete count: " << parser.get_incomplete_message_count() 
                               << std::endl;
                 } else {
                     std::cout << "  -> Error: " << error.message << std::endl;
                 }
             }
             
             std::cout << std::endl;
         }
     }
     
     std::cout << "Total messages received: " << message_count << std::endl;
     std::cout << "Remaining incomplete messages: " << parser.get_incomplete_message_count() << std::endl;
     
     std::cout << std::endl;
 }
 
 // Main function
 int main() {
     std::cout << "AISLIB Multi-part Message Example" << std::endl;
     std::cout << "=================================" << std::endl;
     std::cout << std::endl;
     
     // Run each example
     example_in_order();
     example_out_of_order();
     example_timeout();
     example_multiple_concurrent();
     
     return 0;
 }