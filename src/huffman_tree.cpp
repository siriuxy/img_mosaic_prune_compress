/**
 * @file huffman_tree.cpp
 * Implementation of a Huffman Tree class.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <stdexcept>

#include "huffman_tree.h"

using namespace std;

huffman_tree::huffman_tree(vector<frequency> frequencies)
{
    std::stable_sort(frequencies.begin(), frequencies.end());
    build_tree(frequencies);
    vector<bool> path;
    build_map(root_.get(), path);
}

huffman_tree::huffman_tree(const huffman_tree& other)
{
    copy(other);
}

huffman_tree::huffman_tree(binary_file_reader& bfile)
{
    root_ = read_tree(bfile);
    vector<bool> path;
    build_map(root_.get(), path);
}

huffman_tree& huffman_tree::operator=(huffman_tree rhs)
{
    swap(rhs);
    return *this;
}

void huffman_tree::swap(huffman_tree& other)
{
    std::swap(root_, other.root_);
    std::swap(bits_map_, other.bits_map_);
}

void huffman_tree::copy(const huffman_tree& rhs)
{
    root_ = copy(rhs.root_.get());
}

auto huffman_tree::copy(const node* current) -> std::unique_ptr<node>
{
    if (!current)
        return nullptr;
    auto n = std::make_unique<node>(current->freq);
    n->left = copy(current->left.get());
    n->right = copy(current->right.get());
    return n;
}

void huffman_tree::build_tree(const vector<frequency>& frequencies)
{
    // queue containing leaf nodes
    std::queue<std::unique_ptr<node>> single_queue;
    // queue containing internal nodes
    std::queue<std::unique_ptr<node>> merge_queue;

    /**
     * @todo Your code here!
     *
     * First, place all of the leaf nodes into the single_queue in
     * increasing order of frequency. Note: frequencies is already sorted
     * for you.
     *
     * Next, until there is only one node on the two queues (that is, one
     * of the queues is empty and one has a single node), remove the two
     * smallest entries from the two queues. Then, create a new internal
     * node with these nodes as children, whose frequency is the sum of
     * these two children's frequencies. Place the new internal node onto
     * the back of the merge_queue.
     *
     * Finally, when there is a single node left, it is the root. Assign it
     * to the root and you're done!
     */

	//build single_queue
	for (auto p : frequencies) single_queue.push(std::unique_ptr<node>(new node(p)));	
	
	while( ! (single_queue.empty() && merge_queue.size() == 1)){
		unique_ptr<node> tmpLeft = remove_smallest(single_queue, merge_queue);
		unique_ptr<node> tmpRight = remove_smallest(single_queue, merge_queue);
		unique_ptr<node> tmpMerg(new node(tmpLeft->freq.count() + tmpRight->freq.count()));
		tmpMerg->left = std::move(tmpLeft);
		tmpMerg->right = std::move(tmpRight);
		//unique_ptr scope in while loop?
		merge_queue.push(std::move(tmpMerg));
	}
	root_ = std::move(merge_queue.front());
	merge_queue.pop();
}

void huffman_tree::build_map(const node* current, vector<bool>& path)
{
    // Base case: leaf node.
    if (!current->left && !current->right)
    {
        bits_map_[current->freq.character()] = path;
        return;
    }

    // Move left
    path.push_back(false);
    build_map(current->left.get(), path);
    path.pop_back();

    // Move right
    path.push_back(true);
    build_map(current->right.get(), path);
    path.pop_back();
}

void huffman_tree::print_in_order() const
{
    print_in_order(root_.get());
    cout << endl;
}

void huffman_tree::print_in_order(const node* current) const
{
    if (!current)
        return;
    print_in_order(current->left.get());
    cout << current->freq.character() << ":" << current->freq.count() << " ";
    print_in_order(current->right.get());
}

auto huffman_tree::remove_smallest(
    std::queue<std::unique_ptr<node>>& single_queue,
    std::queue<std::unique_ptr<node>>& merge_queue) -> std::unique_ptr<node>
{
    /**
     * @todo Your code here!
     *
     * Remove the smallest std::unique_ptr<node> from the two queues given
     * as parameters. The entries on the queues are in sorted order, so the
     * smaller of the two queues heads is the smallest item in either of
     * the queues. Return this item after removing it from its queue.
     */

    	std::unique_ptr<node> tmp;
	/*
	std::cout<<"values in remove_sm: "<<merge_queue.empty()<<", "
					<<single_queue.empty()<<", "
					<<merge_queue.front()<<", "
					<<single_queue.front()<<"."<<std::endl;
	*/
	if ((!single_queue.empty())&&(merge_queue.empty() || single_queue.front()->freq.count() <= merge_queue.front()->freq.count())){
		tmp = std::move(single_queue.front());
		single_queue.pop();
	}
	else if ((!merge_queue.empty())&&(single_queue.empty() || single_queue.front()->freq.count() > merge_queue.front()->freq.count())){
		tmp = std::move(merge_queue.front());
		merge_queue.pop();
	}
	else throw logic_error("error in if logic in remove smallest");
    return std::move(tmp);//got error (nullptr) when not using move... so i changed..
}

string huffman_tree::decode_file(binary_file_reader& bfile)
{
    stringstream ss;
    decode(ss, bfile);
    return ss.str();
}

void huffman_tree::decode(stringstream& ss, binary_file_reader& bfile)
{
    auto current = root_.get();
    bool hasBeenLooped = false;
    while (bfile.has_bits())
    {
      /**
       * @todo Your code here!
       *
       * This code is reading in all of the bits in the binary file
       * given. After reading a bit, we go left if the bit was a 0 (or
       * false), and we go right if the bit was a 1 (or true).
       *
       * Special case: if we are at a leaf node, we should "print" its
       * character to the stringstream (with operator<<, just like cout)
       * and start traversing from the root node again.
       */
	hasBeenLooped = true;
		 if (current->left&&current->right) {
			auto direction = bfile.next_bit();
			//std:cout<<direction<<std::endl;
			if (!direction){ // ==0
				current = current->left.get();
			}
			else if(direction){// ==1
				current = current->right.get();
			}
			else throw logic_error("err in inner logic of decode");
		}
		else if (!(current->left||current->right)){//actually, if no 1 child, then no child.... 
			//std::cout<<"print: "<<current->freq.character()<<std::endl;

			ss<<(current->freq.character());
			current = root_.get();
		}
		else throw std::logic_error("error in if logic in decode()");
    }
	if (hasBeenLooped){
		 if (current->left&&current->right) {
			throw logic_error("err in inner logic of decode");
		}

		else if (!(current->left||current->right)){

			ss<<(current->freq.character());
			current = root_.get();
		}
	}
}

void huffman_tree::write(const string& data, binary_file_writer& bfile)
{
    for (const auto& c : data)
        write(c, bfile);
}

void huffman_tree::write(char c, binary_file_writer& bfile)
{
    vector<bool> bits = bits_for_char(c);
    for (const auto& b : bits)
        bfile.write_bit(b);
}

vector<bool> huffman_tree::bits_for_char(char c)
{
    return bits_map_[c];
}

void huffman_tree::write_tree(binary_file_writer& bfile)
{
    write_tree(root_.get(), bfile);
}

void huffman_tree::write_tree(node* current, binary_file_writer& bfile)
{
    /**
     * @todo Your code here!
     *
     * This code is writing the current huffman_tree in a compressed format
     * to the given binary_file_writer. The strategy for doing so is as
     * follows:
     *     1. If we are a leaf node, write the bit "1" followed by the
     *        byte that is the character of this node.
     *     2. If we are an internal node, writ the bit "0", and then
     *        encode the left and right subtree, recursively.
     *
     * Note that we don't encode the frequencies in this compressed
     * version: this is fine, as the structure of the tree still reflects
     * what the relative frequencies were.
     */

	if (!(current->left||current->right)){//actually, if no 1 child, then no child.... 
		bfile.write_bit(1);
		bfile.write_byte((uint8_t) current->freq.character());
	}
	else if (current->left&&current->right) {
		bfile.write_bit(0);
		write_tree(current->left.get(), bfile);
		write_tree(current->right.get(), bfile);
	}
	else throw std::runtime_error("write_tree control error");

}

auto huffman_tree::read_tree(binary_file_reader& bfile) -> std::unique_ptr<node>
{
    /**
     * @todo Your code here!
     *
     * This code is reading a huffman_tree in from a file in the format that
     * we wrote it above. The strategy, then, is as follows:
     *
     *     1. If the file has no more bits, we're done.
     *     2. If we read a 1 bit, we are a leaf node: create a new
     *        node with the character that is the next byte in the
     *        file (its frequency should be 0, since we are ignoring
     *        frequency data now).
     *     3. If we read a 0 bit, create a new internal node (with
     *        frequency 0, since we are ignoring them now, and set its left
     *        child and right children to be the subtrees built recursively.
     *     4. Your function should return the node it creates, or nullptr
     *        if it did not create one.
     */

	std::unique_ptr<node> ret = unique_ptr<node>(new node(0));
	if (!bfile.has_bits()) return nullptr;
	else {
		bool nextBitVal = bfile.next_bit();
		if (nextBitVal) { 
			ret->freq = frequency(bfile.next_byte(), 0);
		}
		else if (!nextBitVal){
			ret->left = std::move(read_tree(bfile));
			ret->right = std::move(read_tree(bfile));
		}
		else throw std::runtime_error("inner control error");

	}
	return std::move(ret);
}

// class for generic printing

template <typename node>
class huffman_tree_node_descriptor
    : public GenericNodeDescriptor<huffman_tree_node_descriptor<node>>
{
  public:
    huffman_tree_node_descriptor(const node* root) : subroot(root)
    {/* nothing */
    }

    string key() const
    {
        std::stringstream ss;
        char ch = subroot->freq.character();
        int freq = subroot->freq.count();

        // print the sum of the two child frequencies
        if (ch == '\0')
            ss << freq;
        // print the leaf containing a character and its count
        else
        {
            if (ch == '\n')
                ss << "\\n";
            else
                ss << ch;
            ss << ":" << freq;
        }
        return ss.str();
    }

    bool isNull() const
    {
        return !subroot;
    }
    huffman_tree_node_descriptor left() const
    {
        return huffman_tree_node_descriptor(subroot->left.get());
    }
    huffman_tree_node_descriptor right() const
    {
        return huffman_tree_node_descriptor(subroot->right.get());
    }

  private:
    const node* subroot;
};

int huffman_tree::height(const node* subroot) const
{
    if (!subroot)
        return -1;
    return 1 +
           std::max(height(subroot->left.get()), height(subroot->right.get()));
}

void huffman_tree::print(std::ostream& out) const
{
    int h = height(root_.get());
    if (h > max_print_height_)
    {
        out << "Tree is too big to print. Try with a small file (e.g. "
               "data/small.txt)" << endl;
        return;
    }

    printTree(huffman_tree_node_descriptor<node>(root_.get()), out);
}
