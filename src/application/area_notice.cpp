/**
 * @file area_notice.cpp
 * @brief Implementation of Area Notice Message (DAC=1, FI=22)
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

 #include "aislib/application/area_notice.h"
 #include "aislib/binary_application_ids.h"
 #include <cmath>
 #include <ctime>
 #include <iomanip>
 #include <sstream>
 
 namespace aislib {
 namespace application {
 
 // Static SubArea constructors
 AreaNotice::SubArea AreaNotice::SubArea::Circle(double lon, double lat, uint16_t radius) {
     SubArea area;
     area.shape_type = AreaShape::CIRCLE;
     area.longitude = static_cast<int32_t>(std::round(lon * 600000.0)); // Convert to 1/10000 minutes
     area.latitude = static_cast<int32_t>(std::round(lat * 600000.0));  // Convert to 1/10000 minutes
     area.params.circle.radius = radius;
     area.params.circle.spare = 0;
     return area;
 }
 
 AreaNotice::SubArea AreaNotice::SubArea::Rectangle(double lon, double lat, uint16_t e_dim, uint16_t n_dim, uint16_t orient) {
     SubArea area;
     area.shape_type = AreaShape::RECTANGLE;
     area.longitude = static_cast<int32_t>(std::round(lon * 600000.0)); // Convert to 1/10000 minutes
     area.latitude = static_cast<int32_t>(std::round(lat * 600000.0));  // Convert to 1/10000 minutes
     area.params.rectangle.e_dimension = e_dim;
     area.params.rectangle.n_dimension = n_dim;
     area.params.rectangle.orientation = orient;
     area.params.rectangle.spare = 0;
     return area;
 }
 
 AreaNotice::SubArea AreaNotice::SubArea::Sector(double lon, double lat, uint16_t radius, uint16_t left_bound, uint16_t right_bound) {
     SubArea area;
     area.shape_type = AreaShape::SECTOR;
     area.longitude = static_cast<int32_t>(std::round(lon * 600000.0)); // Convert to 1/10000 minutes
     area.latitude = static_cast<int32_t>(std::round(lat * 600000.0));  // Convert to 1/10000 minutes
     area.params.sector.radius = radius;
     area.params.sector.left_bound = left_bound;
     area.params.sector.right_bound = right_bound;
     area.params.sector.spare = 0;
     return area;
 }
 
 AreaNotice::SubArea AreaNotice::SubArea::Text(double lon, double lat, const std::string& text) {
     SubArea area;
     area.shape_type = AreaShape::TEXT;
     area.longitude = static_cast<int32_t>(std::round(lon * 600000.0)); // Convert to 1/10000 minutes
     area.latitude = static_cast<int32_t>(std::round(lat * 600000.0));  // Convert to 1/10000 minutes
     
     // Copy text (with truncation if necessary)
     strncpy(area.params.text.text, text.c_str(), sizeof(area.params.text.text) - 1);
     area.params.text.text[sizeof(area.params.text.text) - 1] = '\0'; // Ensure null termination
     area.params.text.spare = 0;
     
     return area;
 }
 
 // AreaNotice Constructor
 AreaNotice::AreaNotice(
     uint8_t message_version,
     NoticeType notice_type,
     const std::chrono::system_clock::time_point& start_time,
     uint16_t duration,
     const std::vector<SubArea>& subareas
 ) : message_version_(message_version),
     notice_type_(notice_type),
     duration_(duration),
     subareas_(subareas)
 {
     // Extract time components from start_time
     std::time_t time = std::chrono::system_clock::to_time_t(start_time);
     std::tm* timeinfo = std::gmtime(&time);
     
     if (timeinfo) {
         start_month_ = timeinfo->tm_mon + 1;  // Month (1-12)
         start_day_ = timeinfo->tm_mday;       // Day (1-31)
         start_hour_ = timeinfo->tm_hour;      // Hour (0-23)
         start_minute_ = timeinfo->tm_min;     // Minute (0-59)
     } else {
         // Default to current time if conversion fails
         start_month_ = 0;
         start_day_ = 0;
         start_hour_ = 0;
         start_minute_ = 0;
     }
 }
 
 // Constructor from BitVector
 AreaNotice::AreaNotice(const BitVector& bits) 
     : message_version_(0),
       notice_type_(NoticeType::CAUTION),
       start_month_(0),
       start_day_(0),
       start_hour_(0),
       start_minute_(0),
       duration_(0)
 {
     // Parse message version (8 bits)
     message_version_ = static_cast<uint8_t>(bits.get_uint(0, 8));
     
     // Parse notice type (7 bits)
     notice_type_ = static_cast<NoticeType>(bits.get_uint(8, 7));
     
     // Parse start time (20 bits)
     start_month_ = static_cast<uint32_t>(bits.get_uint(15, 4));
     start_day_ = static_cast<uint32_t>(bits.get_uint(19, 5));
     start_hour_ = static_cast<uint32_t>(bits.get_uint(24, 5));
     start_minute_ = static_cast<uint32_t>(bits.get_uint(29, 6));
     
     // Parse duration (16 bits)
     duration_ = static_cast<uint16_t>(bits.get_uint(35, 16));
     
     // Parse subareas
     size_t bit_index = 51;
     
     while (bit_index + 3 <= bits.size()) { // Minimum 3 bits needed for shape type
         SubArea area;
         
         // Parse area shape
         area.shape_type = static_cast<AreaShape>(bits.get_uint(bit_index, 3));
         bit_index += 3;
         
         // Check if we have enough bits for coordinates (56 bits needed)
         if (bit_index + 56 > bits.size())
             break;
         
         // Parse longitude (28 bits)
         area.longitude = static_cast<int32_t>(bits.get_int(bit_index, 28));
         bit_index += 28;
         
         // Parse latitude (27 bits)
         area.latitude = static_cast<int32_t>(bits.get_int(bit_index, 27));
         bit_index += 27;
         
         // Parse shape-specific parameters
         switch (area.shape_type) {
             case AreaShape::CIRCLE: {
                 // Check if we have enough bits (14 bits needed)
                 if (bit_index + 14 > bits.size())
                     break;
                 
                 // Parse radius (12 bits)
                 area.params.circle.radius = static_cast<uint16_t>(bits.get_uint(bit_index, 12));
                 bit_index += 12;
                 area.params.circle.spare = static_cast<uint16_t>(bits.get_uint(bit_index, 2));
                 bit_index += 2;
                 break;
             }
                 
             case AreaShape::RECTANGLE: {
                 // Check if we have enough bits (27 bits needed)
                 if (bit_index + 27 > bits.size())
                     break;
                 
                 // Parse dimensions and orientation
                 area.params.rectangle.e_dimension = static_cast<uint16_t>(bits.get_uint(bit_index, 8));
                 bit_index += 8;
                 area.params.rectangle.n_dimension = static_cast<uint16_t>(bits.get_uint(bit_index, 8));
                 bit_index += 8;
                 area.params.rectangle.orientation = static_cast<uint16_t>(bits.get_uint(bit_index, 9));
                 bit_index += 9;
                 area.params.rectangle.spare = static_cast<uint16_t>(bits.get_uint(bit_index, 2));
                 bit_index += 2;
                 break;
             }
                 
             case AreaShape::SECTOR: {
                 // Check if we have enough bits (32 bits needed)
                 if (bit_index + 32 > bits.size())
                     break;
                 
                 // Parse sector parameters
                 area.params.sector.radius = static_cast<uint16_t>(bits.get_uint(bit_index, 12));
                 bit_index += 12;
                 area.params.sector.left_bound = static_cast<uint16_t>(bits.get_uint(bit_index, 9));
                 bit_index += 9;
                 area.params.sector.right_bound = static_cast<uint16_t>(bits.get_uint(bit_index, 9));
                 bit_index += 9;
                 area.params.sector.spare = static_cast<uint16_t>(bits.get_uint(bit_index, 2));
                 bit_index += 2;
                 break;
             }
                 
             case AreaShape::POLYLINE:
             case AreaShape::POLYGON: {
                 // For simplicity, we're only supporting one set of points in this implementation
                 // Real implementation should handle linked points
                 // Check if we have enough bits
                 if (bit_index + 2 > bits.size())
                     break;
                     
                 // Skip scale factor for now
                 bit_index += 2;
                 
                 // We only support up to 4 angles in this simplified implementation
                 for (int i = 0; i < 4; i += 2) {
                     if (bit_index + 28 > bits.size())
                         break;
                         
                     // Parse longitude offset
                     area.params.points.angles[i] = static_cast<int32_t>(bits.get_int(bit_index, 28));
                     bit_index += 28;
                     
                     if (bit_index + 27 > bits.size())
                         break;
                         
                     // Parse latitude offset
                     area.params.points.angles[i+1] = static_cast<int32_t>(bits.get_int(bit_index, 27));
                     bit_index += 27;
                 }
                 
                 break;
             }
                 
             case AreaShape::TEXT: {
                 // Check if we have enough bits for at least one character
                 if (bit_index + 6 > bits.size())
                     break;
                 
                 // Determine how many characters we can read
                 size_t max_chars = std::min(size_t(14), (bits.size() - bit_index) / 6);
                 
                 // Parse text
                 std::string text_content;
                 for (size_t i = 0; i < max_chars; i++) {
                     if (bit_index + 6 > bits.size())
                         break;
                         
                     uint8_t char_value = static_cast<uint8_t>(bits.get_uint(bit_index, 6));
                     bit_index += 6;
                     
                     if (char_value > 0)
                         text_content += static_cast<char>(char_value);
                     else
                         break; // End of string
                 }
                 
                 strncpy(area.params.text.text, text_content.c_str(), sizeof(area.params.text.text) - 1);
                 area.params.text.text[sizeof(area.params.text.text) - 1] = '\0'; // Ensure null termination
                 break;
             }
                 
             case AreaShape::RESERVED_6:
             case AreaShape::RESERVED_7:
                 // Skip reserved shapes
                 break;
         }
         
         // Add subarea to list
         subareas_.push_back(area);
     }
 }
 
 // Getters
 uint8_t AreaNotice::get_message_version() const {
     return message_version_;
 }
 
 AreaNotice::NoticeType AreaNotice::get_notice_type() const {
     return notice_type_;
 }
 
 std::chrono::system_clock::time_point AreaNotice::get_start_time() const {
     std::tm timeinfo = {};
     
     // Get current year (not stored in message)
     std::time_t now = std::time(nullptr);
     std::tm* now_tm = std::gmtime(&now);
     int year = now_tm ? now_tm->tm_year : 0;
     
     // Fill in time components
     timeinfo.tm_year = year;
     timeinfo.tm_mon = start_month_ - 1;
     timeinfo.tm_mday = start_day_;
     timeinfo.tm_hour = start_hour_;
     timeinfo.tm_min = start_minute_;
     
     // Convert to time_point
     std::time_t time = std::mktime(&timeinfo);
     return std::chrono::system_clock::from_time_t(time);
 }
 
 uint16_t AreaNotice::get_duration() const {
     return duration_;
 }
 
 const std::vector<AreaNotice::SubArea>& AreaNotice::get_subareas() const {
     return subareas_;
 }
 
 // Setters
 void AreaNotice::set_message_version(uint8_t version) {
     message_version_ = version;
 }
 
 void AreaNotice::set_notice_type(NoticeType type) {
     notice_type_ = type;
 }
 
 void AreaNotice::set_start_time(const std::chrono::system_clock::time_point& time) {
     // Extract time components
     std::time_t t = std::chrono::system_clock::to_time_t(time);
     std::tm* timeinfo = std::gmtime(&t);
     
     if (timeinfo) {
         start_month_ = timeinfo->tm_mon + 1;
         start_day_ = timeinfo->tm_mday;
         start_hour_ = timeinfo->tm_hour;
         start_minute_ = timeinfo->tm_min;
     }
 }
 
 void AreaNotice::set_duration(uint16_t duration) {
     duration_ = duration;
 }
 
 void AreaNotice::set_subareas(const std::vector<SubArea>& subareas) {
     subareas_ = subareas;
 }
 
 // Binary conversion
 void AreaNotice::to_bits(BitVector& bits) const {
     // Message Version (8 bits)
     bits.append_uint(message_version_, 8);
     
     // Notice Type (7 bits)
     bits.append_uint(static_cast<uint8_t>(notice_type_), 7);
     
     // Start Time (20 bits)
     bits.append_uint(start_month_, 4);
     bits.append_uint(start_day_, 5);
     bits.append_uint(start_hour_, 5);
     bits.append_uint(start_minute_, 6);
     
     // Duration (16 bits)
     bits.append_uint(duration_, 16);
     
     // Subareas
     for (const auto& area : subareas_) {
         // Shape Type (3 bits)
         bits.append_uint(static_cast<uint8_t>(area.shape_type), 3);
         
         // Coordinates (55 bits)
         bits.append_int(area.longitude, 28);
         bits.append_int(area.latitude, 27);
         
         // Shape-specific parameters
         switch (area.shape_type) {
             case AreaShape::CIRCLE: {
                 // Radius (12 bits)
                 bits.append_uint(area.params.circle.radius, 12);
                 // Spare (2 bits)
                 bits.append_uint(0, 2);
                 break;
             }
                 
             case AreaShape::RECTANGLE: {
                 // E Dimension (8 bits)
                 bits.append_uint(area.params.rectangle.e_dimension, 8);
                 // N Dimension (8 bits)
                 bits.append_uint(area.params.rectangle.n_dimension, 8);
                 // Orientation (9 bits)
                 bits.append_uint(area.params.rectangle.orientation, 9);
                 // Spare (2 bits)
                 bits.append_uint(0, 2);
                 break;
             }
                 
             case AreaShape::SECTOR: {
                 // Radius (12 bits)
                 bits.append_uint(area.params.sector.radius, 12);
                 // Left Bound (9 bits)
                 bits.append_uint(area.params.sector.left_bound, 9);
                 // Right Bound (9 bits)
                 bits.append_uint(area.params.sector.right_bound, 9);
                 // Spare (2 bits)
                 bits.append_uint(0, 2);
                 break;
             }
                 
             case AreaShape::POLYLINE:
             case AreaShape::POLYGON: {
                 // Scale Factor (2 bits)
                 bits.append_uint(0, 2); // Default scale factor 0
                 
                 // Points (each point is 55 bits)
                 for (int i = 0; i < 4; i++) {
                     if (i % 2 == 0) {
                         // Longitude angle
                         bits.append_int(area.params.points.angles[i], 28);
                     } else {
                         // Latitude angle
                         bits.append_int(area.params.points.angles[i], 27);
                     }
                 }
                 break;
             }
                 
             case AreaShape::TEXT: {
                 // Text (6 bits per character)
                 std::string text_str(area.params.text.text);
                 for (char c : text_str) {
                     bits.append_uint(static_cast<uint8_t>(c), 6);
                 }
                 // Null terminator if needed
                 if (text_str.length() < 14) {
                     bits.append_uint(0, 6);
                 }
                 break;
             }
                 
             case AreaShape::RESERVED_6:
             case AreaShape::RESERVED_7:
                 // No additional parameters for reserved shapes
                 break;
         }
     }
 }
 
 BitVector AreaNotice::to_bit_vector() const {
     BitVector bits;
     to_bits(bits);
     return bits;
 }
 
 BinaryAddressedMessage AreaNotice::to_addressed_message(
     uint32_t source_mmsi,
     uint32_t dest_mmsi,
     uint8_t sequence_number,
     uint8_t repeat_indicator
 ) const {
     // Create binary addressed message
     BinaryAddressedMessage message(source_mmsi, dest_mmsi, sequence_number, repeat_indicator);
     
     // Set application ID (IMO, Area Notice)
     message.set_application_id(1, 22);
     
     // Set binary data
     BitVector bits = to_bit_vector();
     message.set_data(bits);
     
     return message;
 }
 
 BinaryBroadcastMessage AreaNotice::to_broadcast_message(
     uint32_t source_mmsi,
     uint8_t repeat_indicator
 ) const {
     // Create binary broadcast message
     BinaryBroadcastMessage message(source_mmsi, repeat_indicator);
     
     // Set application ID (IMO, Area Notice)
     message.set_application_id(1, 22);
     
     // Set binary data
     BitVector bits = to_bit_vector();
     message.set_data(bits);
     
     return message;
 }
 
 std::string AreaNotice::to_string() const {
     std::stringstream ss;
     
     ss << "Area Notice Message:\n";
     ss << "  Message Version: " << static_cast<int>(message_version_) << "\n";
     
     // Notice Type
     ss << "  Notice Type: ";
     switch (notice_type_) {
         case NoticeType::CAUTION:
             ss << "Caution";
             break;
         case NoticeType::CAUTION_MARINE_MAMMALS:
             ss << "Caution: Marine mammals";
             break;
         case NoticeType::SECURITY_ZONE:
             ss << "Security zone";
             break;
         // Add other notice types as needed
         default:
             ss << "Unknown (" << static_cast<int>(notice_type_) << ")";
             break;
     }
     ss << "\n";
     
     // Start Time
     ss << "  Start Time: ";
     if (start_month_ > 0 && start_day_ > 0) {
         ss << "Month " << start_month_ 
            << ", Day " << start_day_
            << ", " << start_hour_ << ":" 
            << std::setw(2) << std::setfill('0') << start_minute_ << " UTC";
     } else {
         ss << "Not available";
     }
     ss << "\n";
     
     // Duration
     ss << "  Duration: ";
     if (duration_ == 0) {
         ss << "Unlimited";
     } else if (duration_ < 60) {
         ss << duration_ << " minutes";
     } else if (duration_ < 1440) {
         ss << (duration_ / 60) << " hours " << (duration_ % 60) << " minutes";
     } else {
         ss << (duration_ / 1440) << " days " << ((duration_ % 1440) / 60) << " hours";
     }
     ss << "\n";
     
     // Subareas
     ss << "  Subareas (" << subareas_.size() << "):\n";
     
     int area_index = 1;
     for (const auto& area : subareas_) {
         // Convert coordinates to degrees
         double longitude = area.longitude / 600000.0;
         double latitude = area.latitude / 600000.0;
         
         ss << "    Subarea " << area_index++ << ": "
            << "Pos(" << longitude << "°, " << latitude << "°), ";
         
         switch (area.shape_type) {
             case AreaShape::CIRCLE:
                 ss << "Circle, "
                    << "radius " << area.params.circle.radius << " m\n";
                 break;
                 
             case AreaShape::RECTANGLE:
                 ss << "Rectangle, "
                    << "E/W " << area.params.rectangle.e_dimension << " m, N/S " << area.params.rectangle.n_dimension << " m, "
                    << "orientation " << area.params.rectangle.orientation << "°\n";
                 break;
                 
             case AreaShape::SECTOR:
                 ss << "Sector, "
                    << "radius " << area.params.sector.radius << " m, "
                    << "from " << area.params.sector.left_bound << "° to " << area.params.sector.right_bound << "°\n";
                 break;
                 
             case AreaShape::POLYLINE:
                 ss << "Polyline with points:\n";
                 for (int i = 0; i < 4; i += 2) {
                     if (i + 1 < 4) {
                         double pt_lon = area.params.points.angles[i] / 600000.0;
                         double pt_lat = area.params.points.angles[i + 1] / 600000.0;
                         ss << "      Point: (" << pt_lon << "°, " << pt_lat << "°)\n";
                     }
                 }
                 break;
                 
             case AreaShape::POLYGON:
                 ss << "Polygon with points:\n";
                 for (int i = 0; i < 4; i += 2) {
                     if (i + 1 < 4) {
                         double pt_lon = area.params.points.angles[i] / 600000.0;
                         double pt_lat = area.params.points.angles[i + 1] / 600000.0;
                         ss << "      Point: (" << pt_lon << "°, " << pt_lat << "°)\n";
                     }
                 }
                 break;
                 
             case AreaShape::TEXT:
                 ss << "Text: \"" 
                    << area.params.text.text << "\"\n";
                 break;
                 
             case AreaShape::RESERVED_6:
             case AreaShape::RESERVED_7:
                 ss << "Reserved shape type\n";
                 break;
         }
     }
     
     return ss.str();
 }
 
 } // namespace application
 } // namespace aislib