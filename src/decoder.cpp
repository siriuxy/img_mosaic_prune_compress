/**
 * @file decoder.cpp
 * Implementation of a namespace for decoding files using huffman_trees.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include <fstream>
#include <iostream>

#include "binary_file_reader.h"
#include "decoder.h"
#include "huffman_tree.h"

using namespace std;

int decoder::main(const vector<string>& args)
{
    if (args.size() < 4)
    {
        print_usage(args[0]);
        return -1;
    }
    decode_file(args[1], args[2], args[3]);
    return 0;
}

void decoder::print_usage(const string& programName)
{
    cout << "Usage: " << endl;
    cout << "\t" << programName << " input treefile output" << endl;
    cout << "\t\tinput: file to be decoded" << endl;
    cout << "\t\ttreefile: compressed huffman tree to use for decoding" << endl;
    cout << "\t\toutput: decompressed file" << endl;
}

void decoder::decode_file(const string& input_name, const string& tree_name,
                          const string& output_name)
{
    binary_file_reader input(input_name);
    binary_file_reader treeIn(tree_name);
    huffman_tree tree(treeIn);

    ofstream output(output_name);
    output << tree.decode_file(input);
}
