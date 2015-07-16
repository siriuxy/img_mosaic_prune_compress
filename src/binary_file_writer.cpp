/**
 * @file binary_file_writer.cpp
 * Implementation of a binary file class for output operations.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include <iostream>
#include "binary_file_writer.h"

using namespace std;

binary_file_writer::binary_file_writer(const std::string& fileName)
    : file(fileName, ios::binary), current_byte_('\0'), current_bit_(7)
{
    /* nothing */
}

binary_file_writer::~binary_file_writer()
{
    close();
}

void binary_file_writer::close()
{
    if (!file.is_open())
        return;
    if (current_bit_ != 7)
    {
        auto padding_as_char = static_cast<uint8_t>(current_bit_ + 1);
        // write out the last partial byte
        write_current_byte_();
        current_byte_ = padding_as_char;
        write_current_byte_();
    }
    else
    {
        // write out number of padding bits (0 in this case)
        current_byte_ = 0;
        write_current_byte_();
    }
    file.close();
}

void binary_file_writer::write_bit(bool bit)
{
    current_byte_ = current_byte_ | (static_cast<uint8_t>(bit) << current_bit_);
    --current_bit_;
    if (current_bit_ == -1)
        write_current_byte_();
}

void binary_file_writer::write_byte(uint8_t byte)
{
    for (int i = 0; i < 8; ++i)
        write_bit(((byte >> (7 - i)) & 1) == 1);
}

void binary_file_writer::write_current_byte_()
{
    current_bit_ = 7;
    file.write(reinterpret_cast<char*>(&current_byte_), 1);
    current_byte_ = '\0';
}
