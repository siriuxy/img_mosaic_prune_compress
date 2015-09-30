/**
 * @file binary_file_reader.cpp
 * Implementation of a binary file class for input operations.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include <iostream>
#include "binary_file_reader.h"

binary_file_reader::binary_file_reader(const std::string& fileName)
    : file{fileName, std::ios::binary},
      current_byte_{'\0'},
      current_bit_{-1},
      num_read_{0}
{
    file.seekg(-1, std::ios::end);
    max_bytes_ = file.tellg();
    padding_bits_ = static_cast<int8_t>(file.get());
    file.seekg(0, std::ios::beg);
}

binary_file_reader::~binary_file_reader()
{
    close();
}

void binary_file_reader::close()
{
    if (file.is_open())
        file.close();
}

bool binary_file_reader::has_bits() const
{
    return (num_read_ != max_bytes_) || (current_bit_ >= padding_bits_);
}

bool binary_file_reader::has_bytes() const
{
    return (num_read_ != max_bytes_) || current_bit_ == 7;
}

bool binary_file_reader::next_bit()
{
    if (needs_next_byte())
        read_next_byte();
    bool ret = ((current_byte_ >> current_bit_) & 1) == 1;
    --current_bit_;
    return ret;
}

uint8_t binary_file_reader::next_byte()
{
    uint8_t ret = 0;
    for (int currBit = 0; currBit < 8 && has_bits(); ++currBit)
        ret = ret | next_bit() << (7 - currBit);
    return ret;
}

bool binary_file_reader::needs_next_byte() const
{
    return current_bit_ == -1;
}

void binary_file_reader::read_next_byte()
{
    // read a single byte
    file.read(reinterpret_cast<char*>(&current_byte_), 1);
    current_bit_ = 7;
    ++num_read_;
}

void binary_file_reader::reset()
{
    file.seekg(0, std::ios::beg);
    current_bit_ = -1;
    current_byte_ = '\0';
}
