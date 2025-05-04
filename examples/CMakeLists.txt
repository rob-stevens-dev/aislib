# Add example applications
add_executable(nmea_parser nmea_parser.cpp)
target_link_libraries(nmea_parser aislib)