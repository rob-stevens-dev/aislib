/**
 * @file binary_addressed_message.cpp
 * @brief Implementation of the BinaryAddressedMessage class
 * 
 * This file implements the BinaryAddressedMessage class (AIS Message Type 6),
 * which is used for point-to-point communication of binary data between
 * AIS stations.
 * 
 * The implementation handles the encoding and decoding of Binary Addressed
 * Messages, including the destination MMSI, sequence number, retransmit flag,
 * application ID, and binary data.
 * 
 * @author AISLIB Development Team
 * @date May 2025
 */

 #include "aislib/binary_addressed_message.h"
#include <sstream>
#include <iomanip>

namespace aislib {

BinaryAddressedMessage::BinaryAddressedMessage(
    uint32_t mmsi, 
    uint32_t dest_mmsi, 
    uint8_t sequence_number, 
    uint8_t repeat_indicator
) : BinaryMessage(mmsi, repeat_indicator),
    dest_mmsi_(dest_mmsi),
    sequence_number_(sequence_number),
    retransmit_flag_(false) {
}

BinaryAddressedMessage::BinaryAddressedMessage(const BitVector& bits)
    : BinaryMessage(0, 0),
      dest_mmsi_(0),
      sequence_number_(0),
      retransmit_flag_(false) {
    
    // Check message type
    if (bits.get_uint(0, 6) != 6) {
        throw std::invalid_argument("Invalid message type for Binary Addressed Message");
    }
    
    // Parse the message fields
    repeat_indicator_ = static_cast<uint8_t>(bits.get_uint(6, 2));
    mmsi_ = static_cast<uint32_t>(bits.get_uint(8, 30));
    sequence_number_ = static_cast<uint8_t>(bits.get_uint(38, 2));
    dest_mmsi_ = static_cast<uint32_t>(bits.get_uint(40, 30));
    retransmit_flag_ = bits.get_bit(70);
    
    // Spare bit (1 bit) - skipped
    
    // Parse application ID
    dac_ = static_cast<uint16_t>(bits.get_uint(72, 10));
    fi_ = static_cast<uint16_t>(bits.get_uint(82, 6));
    
    // Parse application data
    // The rest of the bits are application data
    size_t data_start = 88;
    size_t data_length = bits.size() - data_start;
    
    if (data_length > 0) {
        data_ = BitVector();
        for (size_t i = 0; i < data_length; ++i) {
            data_.append_bit(bits.get_bit(data_start + i));
        }
    }
}

uint8_t BinaryAddressedMessage::get_message_type() const {
    return 6;
}

uint32_t BinaryAddressedMessage::get_mmsi() const {
    return mmsi_;
}

uint8_t BinaryAddressedMessage::get_repeat_indicator() const {
    return repeat_indicator_;
}

uint32_t BinaryAddressedMessage::get_dest_mmsi() const {
    return dest_mmsi_;
}

uint8_t BinaryAddressedMessage::get_sequence_number() const {
    return sequence_number_;
}

bool BinaryAddressedMessage::get_retransmit_flag() const {
    return retransmit_flag_;
}

void BinaryAddressedMessage::set_dest_mmsi(uint32_t dest_mmsi) {
    dest_mmsi_ = dest_mmsi;
}

void BinaryAddressedMessage::set_sequence_number(uint8_t sequence_number) {
    if (sequence_number > 3) {
        sequence_number_ = 0; // Valid values are 0-3
    } else {
        sequence_number_ = sequence_number;
    }
}

void BinaryAddressedMessage::set_retransmit_flag(bool retransmit) {
    retransmit_flag_ = retransmit;
}

void BinaryAddressedMessage::to_bits(BitVector& bits) const {
    // Message Type (6 bits)
    bits.append_uint(get_message_type(), 6);
    
    // Repeat Indicator (2 bits)
    bits.append_uint(repeat_indicator_, 2);
    
    // MMSI (30 bits)
    bits.append_uint(mmsi_, 30);
    
    // Sequence Number (2 bits)
    bits.append_uint(sequence_number_, 2);
    
    // Destination MMSI (30 bits)
    bits.append_uint(dest_mmsi_, 30);
    
    // Retransmit Flag (1 bit)
    bits.append_bit(retransmit_flag_);
    
    // Spare (1 bit)
    bits.append_bit(false);
    
    // DAC (10 bits)
    bits.append_uint(dac_, 10);
    
    // FI (6 bits)
    bits.append_uint(fi_, 6);
    
    // Application Data (variable length)
    if (data_.size() > 0) {
        for (size_t i = 0; i < data_.size(); ++i) {
            bits.append_bit(data_.get_bit(i));
        }
    }
}

std::string BinaryAddressedMessage::to_string() const {
    std::stringstream ss;
    
    ss << "AIS Message Type: " << static_cast<int>(get_message_type()) << " (Binary Addressed Message)\n";
    ss << "MMSI: " << mmsi_ << "\n";
    ss << "Repeat Indicator: " << static_cast<int>(repeat_indicator_) << "\n";
    ss << "Sequence Number: " << static_cast<int>(sequence_number_) << "\n";
    ss << "Destination MMSI: " << dest_mmsi_ << "\n";
    ss << "Retransmit Flag: " << (retransmit_flag_ ? "Yes" : "No") << "\n";
    ss << "Application ID: DAC=" << dac_ << ", FI=" << fi_ << "\n";
    ss << "Data Length: " << data_.size() << " bits\n";
    
    if (data_.size() > 0) {
        ss << "Data (hex): " << data_.to_hex();
    }
    
    return ss.str();
}

} // namespace aislib