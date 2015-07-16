/**
 * @file encoder.cpp
 * Implementation of a namespace for encoding files using huffman_trees.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "binary_file_writer.h"
#include "encoder.h"
#include "huffman_tree.h"

using namespace std;

int encoder::main(const vector<string>& args)
{
    if (args.size() < 4)
    {
        print_usage(args[0]);
        return -1;
    }
    encode_file(args[1], args[2], args[3]);
    return 0;
}

void encoder::print_usage(const string& programName)
{
    cout << "Usage: " << endl;
    cout << "\t" << programName << " input output treefile" << endl;
    cout << "\t\tinput: file to be encoded" << endl;
    cout << "\t\toutput: encoded output" << endl;
    cout << "\t\ttreefile: compressed huffman tree for decoding" << endl;
}

void encoder::encode_file(const string& input_name, const string& output_name,
                          const string& tree_name)
{
    ifstream input(input_name);
    stringstream ss;
    ss << input.rdbuf();

    huffman_tree tree(get_frequencies(ss.str()));
    binary_file_writer output(output_name);
    binary_file_writer treeFile(tree_name);

    cout << "Printing generated huffman_tree..." << endl;
    tree.print(cout);

    cout << "Saving huffman_tree to file..." << endl;
    tree.write(ss.str(), output);
    tree.write_tree(treeFile);
}

vector<frequency> encoder::get_frequencies(const string& str)
{
    unordered_map<char, int> freqs;
    for (auto it = str.begin(); it != str.end(); ++it)
        freqs[*it]++;
    vector<frequency> result;
    for (auto it = freqs.begin(); it != freqs.end(); ++it)
        result.push_back(frequency(it->first, it->second));
    return result;
}
