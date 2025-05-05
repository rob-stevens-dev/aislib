# Add example applications
add_executable(nmea_parser nmea_parser.cpp)
target_link_libraries(nmea_parser aislib)

add_executable(binary_message_example binary_message_example.cpp)
target_link_libraries(binary_message_example aislib)

# Add multi-part message example (Phase 4)
add_executable(multipart_example multipart_example.cpp)
target_link_libraries(multipart_example aislib)