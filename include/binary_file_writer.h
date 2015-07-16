/**
 * @file binary_file_writer.h
 * Definition of a binary file class for output operations.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#ifndef BINARY_FILE_WRITER_H_
#define BINARY_FILE_WRITER_H_

#include <fstream>
#include <string>

/**
 * binary_file_writer: interface for writing to binary files, bit by bit or
 * byte by byte. Wraps an ofstream in binary mode.
 *
 * @author Chase Geigle
 * @date Sumer 2012
 */
class binary_file_writer
{
  public:
    /**
     * Constructs a new binary_file_writer by opening the given filename.
     *
     * @param fileName File to be opened.
     */
    binary_file_writer(const std::string& fileName);

    /**
     * Destroys an binary_file_writer: the destructor here ensures that
     * all remaining bits are written to the file before closing the
     * file. If the file was already closed, nothing is done.
     */
    ~binary_file_writer();

    /**
     * Writes the next bit of the file.
     *
     * @param bit The bit to be written.
     */
    void write_bit(bool bit);

    /**
     * Writes the next byte to the file.
     *
     * @param byte The byte to be written.
     */
    void write_byte(uint8_t byte);

    /**
     * Closes the given file.
     */
    void close();

  private:
    /// Used to write to the file
    std::ofstream file;
    /// The current byte to write bits into
    uint8_t current_byte_;
    /// The current bit within the current byte that is being written
    int8_t current_bit_;

    /**
     * Writes the current byte to the file.
     */
    void write_current_byte_();
};
#endif
