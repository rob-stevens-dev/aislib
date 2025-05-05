/**
 * @file binary_application_ids.h
 * @brief AIS Binary Application IDs
 * 
 * This file defines the standard Designated Area Codes (DAC) and Function
 * Identifiers (FI) used in AIS binary messages. The combination of a DAC and
 * an FI forms an application ID, which determines how the binary data in
 * a message should be interpreted.
 * 
 * The DAC identifies the geographical region or authority responsible for
 * defining the FI, while the FI identifies the specific application or
 * message type within that authority's domain.
 * 
 * Common DACs include:
 * - DAC 1: International (IMO/ITU)
 * - DAC 200: European inland waterways
 * - DAC 366: United States
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

 #ifndef AISLIB_BINARY_APPLICATION_IDS_H
 #define AISLIB_BINARY_APPLICATION_IDS_H
 
 #include <cstdint>
 
 namespace aislib {
 namespace application {
 
 // Designated Area Codes (DAC)
 constexpr uint16_t BINARY_APP_ID_TEST = 0;     // Test/development
 constexpr uint16_t BINARY_APP_ID_IMO = 1;      // IMO/International
 constexpr uint16_t BINARY_APP_ID_EUROPE = 200; // European region
 constexpr uint16_t BINARY_APP_ID_USA = 366;    // United States
 constexpr uint16_t BINARY_APP_ID_CA = 316;     // Canada
 
 // IMO/International Function Identifiers (FI) for DAC = 1
 constexpr uint16_t BINARY_APP_FI_VESSEL_TRAFFIC_DATA = 1;
 constexpr uint16_t BINARY_APP_FI_DANGER_CARGO_INDICATION = 2;
 constexpr uint16_t BINARY_APP_FI_FAIRWAY_CLOSED = 3;
 constexpr uint16_t BINARY_APP_FI_TIDAL_WINDOW = 4;
 constexpr uint16_t BINARY_APP_FI_MARINE_TRAFFIC_SIGNAL = 5;
 constexpr uint16_t BINARY_APP_FI_BERTHING_DATA = 6;
 constexpr uint16_t BINARY_APP_FI_WEATHER_OBSERVATION = 11;
 constexpr uint16_t BINARY_APP_FI_AREA_NOTICE = 22;
 constexpr uint16_t BINARY_APP_FI_METEO_HYDRO_DATA = 31;
 constexpr uint16_t BINARY_APP_FI_ROUTE_INFO = 32;
 
 // European Region Function Identifiers (FI) for DAC = 200
 constexpr uint16_t BINARY_APP_FI_EUROPE_TEXT_DESCRIPTION = 10;
 
 // United States Function Identifiers (FI) for DAC = 366
 constexpr uint16_t BINARY_APP_FI_USA_TEXT_DATA = 1;
 constexpr uint16_t BINARY_APP_FI_USA_VESSEL_DATA = 2;
 constexpr uint16_t BINARY_APP_FI_USA_WEATHER_DATA = 3;
 
 } // namespace application
 } // namespace aislib
 
 #endif // AISLIB_BINARY_APPLICATION_IDS_H