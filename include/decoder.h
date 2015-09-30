/**
 * @file decoder.h
 * Definitions for functions for the decoder program.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#ifndef DECODER_H_
#define DECODER_H_

#include <string>
#include <vector>

/**
 * decoder namespace: collection of functions used for running the decoder
 * program.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */
namespace decoder
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
 * @param program_name The name of the program.
 */
void print_usage(const std::string& program_name);

/**
 * Decodes a file using the given HuffmanTree.
 *
 * @param input_name Name of the file to be decompressed.
 * @param tree_name Name of the file from which to read the HuffmanTree.
 * @param output_name Name of the file to write decompressed output to.
 */
void decode_file(const std::string& input_name, const std::string& tree_name,
                const std::string& output_name);
}

#endif
