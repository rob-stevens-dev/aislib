/**
 * @file nmea_parser.cpp
 * @brief Example application for parsing NMEA sentences
 */

 #include "aislib/ais_parser.h"
 #include "aislib/nmea_utils.h"
 #include "aislib/bit_vector.h"
 #include <iostream>
 #include <string>
 #include <vector>
 #include <fstream>
 
 using namespace aislib;
 
 void print_usage(const char* program_name) {
     std::cout << "Usage: " << program_name << " [options] [NMEA sentences...]" << std::endl;
     std::cout << "Options:" << std::endl;
     std::cout << "  -f, --file <file>    Read NMEA sentences from file" << std::endl;
     std::cout << "  -h, --help           Display this help message" << std::endl;
     std::cout << std::endl;
     std::cout << "If no file is specified, NMEA sentences can be provided as command-line arguments" << std::endl;
     std::cout << "or read from standard input (one sentence per line)." << std::endl;
 }
 
 void process_sentence(AISParser& parser, const std::string& sentence) {
     std::cout << "Processing: " << sentence << std::endl;
     
     // Parse the sentence
     auto message = parser.parse(sentence);
     
     if (message) {
         // Message is complete
         std::cout << "Message type: " << static_cast<int>(message->get_message_type()) << std::endl;
         std::cout << "MMSI: " << message->get_mmsi() << std::endl;
         std::cout << "Repeat indicator: " << static_cast<int>(message->get_repeat_indicator()) << std::endl;
         std::cout << std::endl;
     } else {
         // Message is incomplete or invalid
         auto error = parser.get_last_error();
         
         if (error.type == AISParser::ParseError::ErrorType::NONE) {
             std::cout << "Message is part of a multi-sentence message (waiting for more fragments)" << std::endl;
         } else {
             std::cout << "Error parsing message: " << error.message << std::endl;
         }
         
         std::cout << std::endl;
     }
 }
 
 int main(int argc, char* argv[]) {
     // Parse command-line arguments
     std::string filename;
     std::vector<std::string> sentences;
     
     for (int i = 1; i < argc; ++i) {
         std::string arg = argv[i];
         
         if (arg == "-h" || arg == "--help") {
             print_usage(argv[0]);
             return 0;
         } else if (arg == "-f" || arg == "--file") {
             if (i + 1 < argc) {
                 filename = argv[++i];
             } else {
                 std::cerr << "Error: Missing filename after " << arg << std::endl;
                 print_usage(argv[0]);
                 return 1;
             }
         } else {
             sentences.push_back(arg);
         }
     }
     
     // Create parser
     AISParser parser;
     
     // Process sentences from file
     if (!filename.empty()) {
         std::ifstream file(filename);
         
         if (!file) {
             std::cerr << "Error: Could not open file " << filename << std::endl;
             return 1;
         }
         
         std::string line;
         
         while (std::getline(file, line)) {
             // Skip empty lines and comments
             if (line.empty() || line[0] == '#') {
                 continue;
             }
             
             process_sentence(parser, line);
         }
     }
     // Process sentences from command-line arguments
     else if (!sentences.empty()) {
         for (const auto& sentence : sentences) {
             process_sentence(parser, sentence);
         }
     }
     // Process sentences from standard input
     else {
         std::cout << "Enter NMEA sentences (one per line, Ctrl+D to end):" << std::endl;
         std::string line;
         
         while (std::getline(std::cin, line)) {
             // Skip empty lines and comments
             if (line.empty() || line[0] == '#') {
                 continue;
             }
             
             process_sentence(parser, line);
         }
     }
     
     return 0;
 }