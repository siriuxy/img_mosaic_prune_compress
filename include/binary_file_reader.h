/**
 * @file binary_file_reader.h
 * Definition of a binary file class for input operations.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#ifndef BINARY_FILE_READER_H_
#define BINARY_FILE_READER_H_

#include <fstream>
#include <string>

/**
 * binary_file_reader: interface for reading binary files, bit by bit or byte
 * by byte. Wraps an ifstream in binary mode.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */
class binary_file_reader
{
  public:
    /**
     * Constructs a new binary_file_reader by opening the given file.
     *
     * @param fileName File to be opened.
     */
    binary_file_reader(const std::string& fileName);

    /**
     * Destroys a binary_file_reader, ensuring the file is correctly
     * closed. If the file is already closed, does nothing.
     */
    ~binary_file_reader();

    /**
     * Reads the next bit of the file. Should only be called when
     * has_bits() is true.
     *
     * @return The next bit of the file.
     */
    bool next_bit();

    /**
     * Reads the next byte of the file. Should only be called when
     * has_bytes() is true.
     *
     * @return The next byte of the file, as a char.
     */
    uint8_t next_byte();

    /**
     * Resets the file pointer to the beginning of the file.
     */
    void reset();

    /**
     * Closes the given file.
     */
    void close();

    /**
     * Determines if there are more **bits** to be read in the file.
     *
     * @return Whether or not there exists at least one more unread bit
     * in the file.
     */
    bool has_bits() const;

    /**
     * Determines if there are more **bytes** to be read in the file.
     *
     * @return Whether or not there exists at least one more unread
     * byte in the file.
     */
    bool has_bytes() const;

  private:
    /// Used to read in the file
    std::ifstream file;
    /// The current byte to read bits from.
    uint8_t current_byte_;
    /// The current bit within the current byte that is being read
    int8_t current_bit_;
    /// The total number of bytes in the file
    int max_bytes_;
    /// The number of bytes read from the file
    int num_read_;
    /// The number of padding bits there are in the final byte
    int8_t padding_bits_;

    /**
     * Determines whether or not another byte needs read from the file.
     *
     * @return Whether or not another byte must be read.
     */
    bool needs_next_byte() const;

    /**
     * Reads in a single byte from the file.
     */
    void read_next_byte();
};

#endif
