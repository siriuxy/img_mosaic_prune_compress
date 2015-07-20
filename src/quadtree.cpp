/**
 * @file quadtree.cpp
 * quadtree class implementation.
 * @date Spring 2008
 * @date Modified Summer 2014
 */

#include "quadtree.h"
#include <iostream>
using std::cout;
using std::endl;
namespace cs225
{

quadtree::node::node(node &other){//deep copy of the quadtree:
	x_ = other.x_;
	y_ = other.y_;
	length_ = other.length_;
	element = other.element;
	if (other.northwest) northwest = std::unique_ptr<node>(new node(*other.northwest.get()));
	if (other.northeast) northeast = std::unique_ptr<node>(new node(*other.northeast.get()));
	if (other.southwest) southwest = std::unique_ptr<node>(new node(*other.southwest.get()));
	if (other.southeast) southeast = std::unique_ptr<node>(new node(*other.southeast.get()));
}

quadtree::node::node(unsigned x, unsigned y, unsigned length):x_{x},y_{y},length_{length}{
}

quadtree::quadtree():root_{nullptr}, res_{ 0}{
}

quadtree::quadtree(const epng::png& source, unsigned d){
	res_ = d;
	build_tree(source, d);
}

quadtree::quadtree(const quadtree &other){
	if (other.root_) root_ = std::unique_ptr<node>(new node(*other.root_));
	else if (!(other.root_)) {
		root_ = nullptr; 
		res_ = 0;
	}
}

quadtree::quadtree(quadtree &&other){
	root_ = nullptr;
	res_ = 0;
	swap(other);
}

void quadtree::swap(quadtree &other){
	std::swap(root_, other.root_);
	std::swap(res_, other.res_);
}

quadtree& quadtree::operator=(quadtree other){
	swap(other);
	return *this;
}

void quadtree::build_tree(const epng::png& source, unsigned d){
	//recursively define downwards, and fix element_ to its child averge on the way back
	if (d == 0) return;
	root_ = std::move(std::unique_ptr<node>(new node(source, 0, 0, d))); //move assignment... delete original
}

quadtree::node::node(const epng::png& source, unsigned x, unsigned y, unsigned d){
	if (d == 1) {
		length_ = 1;
		x_ = x;
		y_ = y;
		element = *source(x_, y_);
		return;
	}
	else {
		//std::cout<<"current d is: "<<d<<endl;
		length_ = d;
		x_ = x;
		y_ = y;
		d = d/2;
		northwest = std::unique_ptr<node>(new node(source, x_, y_, d));
		northeast = std::unique_ptr<node>(new node(source, x_+d, y_, d));
		southwest = std::unique_ptr<node>(new node(source, x_, y_+d, d));
		southeast = std::unique_ptr<node>(new node(source, x_+d, y_+d, d));

		element.red = 1/4 * (northwest->element.red +
					northeast->element.red +
					southwest->element.red +
					southeast->element.red);
		element.green = 1/4 * (northwest->element.green +
					northeast->element.green +
					southwest->element.green +
					southeast->element.green);	
		element.blue = 1/4 * (northwest->element.blue +
					northeast->element.blue +
					southwest->element.blue +
					southeast->element.blue);
	}
}

const epng::rgba_pixel& quadtree::operator()(unsigned x, unsigned y)const{
	return root_.get()->nodeFinder(x,y)->element;
}

auto quadtree::node::nodeFinder(unsigned x, unsigned y)const ->node*{
	bool north = false;
	bool west = false;
	if (x < x_ + length_/2){
		west = true;
	}
	else if (x_ + length_/2 <= x && x < x_ + length_){
		west = false;
	}
	else throw std::out_of_range("duh, access x out of range");
	
	if (y < y_ + length_/2){
		north = true;
	}
	else if (y_ + length_/2 <= y && y < y_ + length_){
		north = false;
	}
	else throw std::out_of_range("duh, access y out of range");
	
	if (north && west) return northwest->nodeFinder(x, y);
	else if (north && !west) return northeast->nodeFinder(x, y);
	else if (!north && west) return southwest->nodeFinder(x, y);
	else if (!north && !west) return southeast->nodeFinder(x, y);
	else throw std::runtime_error("out of control flow in nodeFinder()");

}

epng::png quadtree::decompress()const{
	if (!root_) throw std::runtime_error("tree empty, cannot decompress()");
	epng::png ret(res_, res_);
	root_.get()->colorFiller(ret);
	return ret;
}

void quadtree::node::colorFiller(epng::png& output){
	//base case is when all of its child is nullptr
	if (!northwest){//others should be null too! maybe leave a test here?
		*output(x_, y_) = element;
	}
	else {
		northwest->colorFiller(output);
		northeast->colorFiller(output);
		southwest->colorFiller(output);
		southeast->colorFiller(output);
	}
}
}

