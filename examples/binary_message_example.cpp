/**
 * @file binary_message_example.cpp
 * @brief Example for using binary message types in AISLIB
 * 
 * This example demonstrates how to create and use binary AIS messages.
 */

 #include "aislib/ais_message.h"
 #include "aislib/binary_addressed_message.h"
 #include "aislib/binary_broadcast_message.h"
 #include <iostream>
 #include <string>
 #include <memory>
 
 using namespace aislib;
 
 // Example function to demonstrate Binary Addressed Message (Type 6)
 void create_addressed_binary_message() {
     // Create a Binary Addressed Message
     uint32_t source_mmsi = 123456789;
     uint32_t dest_mmsi = 987654321;
     uint8_t sequence_number = 0;
     uint8_t repeat_indicator = 0;
     
     // Create a message using the constructor
     auto message = std::make_unique<BinaryAddressedMessage>(
         source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
     
     // Set the application ID (DAC and FI)
     uint16_t dac = 1;      // DAC 1 = IMO/International
     uint16_t fi = 31;      // FI 31 = Meteorological & Hydrological Data
     
     message->set_application_id(dac, fi);
     
     // For this example, we'll create a simple payload with some binary data
     BitVector payload;
     
     // Add some example data to the payload
     payload.append_uint(42, 8);            // Example 8-bit value
     payload.append_string("TEST", 4 * 6);  // 4 characters, 6 bits each
     payload.append_int(-123, 16);          // Example 16-bit signed value
     
     // Set the binary data payload
     message->set_data(payload);
     
     // Set optional retransmit flag
     message->set_retransmit_flag(false);
     
     // Now let's display the message details
     std::cout << "Binary Addressed Message (Type 6):" << std::endl;
     std::cout << "=================================" << std::endl;
     std::cout << "Source MMSI: " << message->get_mmsi() << std::endl;
     std::cout << "Destination MMSI: " << message->get_dest_mmsi() << std::endl;
     std::cout << "Sequence Number: " << static_cast<int>(message->get_sequence_number()) << std::endl;
     std::cout << "Application ID: DAC=" << message->get_dac() 
               << ", FI=" << message->get_fi() << std::endl;
     std::cout << "Retransmit Flag: " << (message->get_retransmit_flag() ? "Yes" : "No") << std::endl;
     std::cout << "Payload Length: " << message->get_data().size() << " bits" << std::endl;
     std::cout << "Payload Hex: " << message->get_data().to_hex() << std::endl;
     std::cout << std::endl;
     
     // Convert to NMEA sentences
     auto sentences = message->to_nmea();
     
     std::cout << "NMEA Sentence(s):" << std::endl;
     for (const auto& sentence : sentences) {
         std::cout << sentence << std::endl;
     }
     std::cout << std::endl;
 }
 
 // Example function to demonstrate Binary Broadcast Message (Type 8)
 void create_binary_broadcast_message() {
     // Create a Binary Broadcast Message
     uint32_t source_mmsi = 123456789;
     uint8_t repeat_indicator = 0;
     
     // Create a message using the constructor
     auto message = std::make_unique<BinaryBroadcastMessage>(source_mmsi, repeat_indicator);
     
     // Set the application ID (DAC and FI)
     uint16_t dac = 1;      // DAC 1 = IMO/International
     uint16_t fi = 22;      // FI 22 = Area Notice
     
     message->set_application_id(dac, fi);
     
     // For this example, we'll create a simple payload with some binary data
     BitVector payload;
     
     // Add some example data to the payload
     payload.append_uint(1, 8);             // Message version
     payload.append_uint(1, 8);             // Notice type (1 = Warning)
     payload.append_uint(0x12345, 16);      // Duration in minutes (example)
     payload.append_string("CAUTION", 7 * 6); // 7 characters, 6 bits each
     
     // Set the binary data payload
     message->set_data(payload);
     
     // Now let's display the message details
     std::cout << "Binary Broadcast Message (Type 8):" << std::endl;
     std::cout << "================================" << std::endl;
     std::cout << "Source MMSI: " << message->get_mmsi() << std::endl;
     std::cout << "Application ID: DAC=" << message->get_dac() 
               << ", FI=" << message->get_fi() << std::endl;
     std::cout << "Payload Length: " << message->get_data().size() << " bits" << std::endl;
     std::cout << "Payload Hex: " << message->get_data().to_hex() << std::endl;
     std::cout << std::endl;
     
     // Convert to NMEA sentences
     auto sentences = message->to_nmea();
     
     std::cout << "NMEA Sentence(s):" << std::endl;
     for (const auto& sentence : sentences) {
         std::cout << sentence << std::endl;
     }
     std::cout << std::endl;
 }
 
 int main() {
     std::cout << "AISLIB Binary Message Examples" << std::endl;
     std::cout << "=============================" << std::endl << std::endl;
     
     // Example: Create and display an Addressed Binary Message (Type 6)
     create_addressed_binary_message();
     
     // Example: Create and display a Binary Broadcast Message (Type 8)
     create_binary_broadcast_message();
     
     return 0;
 }