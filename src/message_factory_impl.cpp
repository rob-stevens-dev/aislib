/**
 * @file message_factory_impl.cpp
 * @brief Implementation of message factory initialization
 * 
 * This file contains the implementation for registering message types
 * with the message factory.
 */

 #include "aislib/message_factory.h"
 #include "aislib/position_report_class_a.h"
 #include "aislib/base_station_report.h"
 #include "aislib/position_report_class_b.h"
 
 namespace aislib {
 
 // Anonymous namespace for initialization function
 namespace {
 
 // Initialize the message factory with all supported message types
 void init_message_factory() {
     MessageFactory& factory = MessageFactory::instance();
     
     // Register Position Report Class A (Types 1, 2, 3)
     factory.register_message_type(1, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<PositionReportClassA>(bits);
     });
     
     factory.register_message_type(2, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<PositionReportClassA>(bits);
     });
     
     factory.register_message_type(3, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<PositionReportClassA>(bits);
     });
     
     // Register Base Station Report (Type 4)
     factory.register_message_type(4, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<BaseStationReport>(bits);
     });
     
     // Register Standard Class B Position Report (Type 18)
     factory.register_message_type(18, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<StandardPositionReportClassB>(bits);
     });
     
     // Register Extended Class B Position Report (Type 19)
     factory.register_message_type(19, [](const BitVector& bits) -> std::unique_ptr<AISMessage> {
         return std::make_unique<ExtendedPositionReportClassB>(bits);
     });
     
     // Additional message types will be registered in future phases
 }
 
 // Static variable to ensure initialization happens exactly once
 static struct Initializer {
     Initializer() {
         init_message_factory();
     }
 } initializer;
 
 } // anonymous namespace
 
 } // namespace aislib