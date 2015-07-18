/**
 * @file quadtree.cpp
 * quadtree class implementation.
 * @date Spring 2008
 * @date Modified Summer 2014
 */

#include "quadtree.h"

namespace cs225
{

quadtree::quadtree(){
	root_ = nullptr;
	length_ = 0;
}

quadtree::quadtree(epng::png source, unsigned d){
	build_tree(source, d);
}

quadtree::quadtree(quadtree& other){
	//recursively copy down each unique_ptr's rgb elemnt, and build a similar tree structure
	node* otherRoot = other.root_.get();
	copy_tree(root_, otherRoot);
}

void quadtree::copy_tree(std::unique_ptr<node>& newNode, node* other){
	if (!other) return;
	else {
		newNode = std::unique_ptr<node>(new node(*other));//how do i scope to node(copy)
		copy_tree(newNode->northwest, other->northwest.get());
		copy_tree(newNode->northeast, other->northeast.get());
		copy_tree(newNode->southwest, other->southwest.get());
		copy_tree(newNode->southeast, other->southeast.get());
		}
}
quadtree::node::node(node& other){

	node_dis_ = other.node_dis_;
	node_start_x_ = other.node_start_x_;
	node_start_y_ = other.node_start_y_;
	element = other.element;

}

quadtree::quadtree(quadtree&& other):root_{nullptr},lengh_{0}{
	swap(other);
}

quadtree& quadtree::operator=(quadtree other){
	swap(other);
	return *this;
}

void quadtree::swap(quadtree& other){
	std::swap(root_, other.root_);
	std::swap(length_, other.length_);
}

void quadtree::build_tree(const epng::png & source, unsigned d){
	//if not empty, delete by swapping with empty tree.
	if (!empty()) {
		std::swap(root_, nullptr);
	}
	build_tree(std::move(root_), d);//do i really need to move?
}

std::unique_ptr<node> quadtree::build_tree(std::unique_ptr<node>& curr, unsigned side_length){
	if (length_ / (curr->node_dis_ + 1) == 1) {
		
		return;
	}
	

}



const epng::rgba_pixel& quadtree::operator()(unsigned x, unsigned y) const;

epng::png quadtree::decompress()const;

bool quadtree::empty(){
	return root_ == nullptr;
	}

}
