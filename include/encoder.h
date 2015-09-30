/**
 * @file encoder.h
 * Definitions for functions for the encoder program.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <string>
#include <vector>

#include "frequency.h"

/**
 * encoder namespace: collection of functions used for running the encoder
 * program.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */
namespace encoder
{
/**
 * Main method of the program, should you choose to run it as a command
 * line version.
 *
 * @param args The command line arguments, with args[0] being the
 * program name.
 */
int main(const std::vector<std::string>& args);

/**
 * Prints the usage of the command line version.
 *
 * @param programName The name of the program.
 */
void print_usage(const std::string& programName);

/**
 * Encodes a file using Huffman coding. Also creates the compressed
 * output of the HuffmanTree so it can be read in and used for
 * decompression.
 *
 * @param input_name Name of the file to be compressed.
 * @param output_name Name of the file to write compressed output.
 * @param tree_name Name of the file to write the compressed
 * HuffmanTree.
 */
void encode_file(const std::string& input_name, const std::string& output_name,
                 const std::string& tree_name);

/**
 * Determines the frequencies of characters in a string.
 *
 * @param str The string to be searched.
 * @return A vector of frequency objects representing the frequency
 * each character in the file has.
 */
std::vector<frequency> get_frequencies(const std::string& str);
}

#endif
