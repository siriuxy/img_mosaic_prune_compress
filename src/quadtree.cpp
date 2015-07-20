/**
 * @file quadtree.cpp
 * quadtree class implementation.
 * @date Spring 2008
 * @date Modified Summer 2014
 */

#include "quadtree.h"
#include <iostream>
#include <cmath>

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
	build_tree(source, d);
}

quadtree::quadtree(const quadtree &other){
	if (other.root_) {
	root_ = std::unique_ptr<node>(new node(*other.root_));
	res_ = other.res_;
	}
	else if (!(other.root_)) {
		root_ = nullptr; 
		res_ = 0;
	}
}//TODO: there is a bug in copy ctor. go fix it!

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
	res_ = d;
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
		//cout<<"current x, y are: "<<x_<<", "<<y_<<endl;
		*output(x_, y_) = element;
	}
	else {
		//cout<<"non operational x y: "<<x_<<", "<<y_<<endl;
		northwest->colorFiller(output);
		northeast->colorFiller(output);
		southwest->colorFiller(output);
		southeast->colorFiller(output);
	}
}

void quadtree::rotate_clockwise(){
	if (!root_) throw std::runtime_error("cannot rotate empty img");
	root_.get()->rotate_node_clockwise();//awkaward.. TODO: change signature later to no param
}

void quadtree::node::rotate_node_clockwise(){
	if (!northwest) return;//cannot use this as nullptr, it seems...
	else {
		northwest.get()->rotate_node_clockwise();
		northeast.get()->rotate_node_clockwise();
		southwest.get()->rotate_node_clockwise();
		southeast.get()->rotate_node_clockwise();

		std::unique_ptr<node> tmp = std::move(northwest);
		northwest = std::move(southwest);
		southwest = std::move(southeast);
		southeast = std::move(northeast);
		northeast = std::move(tmp);
	}
}

void quadtree::prune(unsigned tolerance){
	int do_prune = -1;
	root_.get()->node_prune(tolerance, do_prune);	
}

unsigned quadtree::pruned_size(unsigned tolerance) const{
	int pruned_size = 0;
	root_.get()->node_prune(tolerance, pruned_size);
	return pruned_size;
}

void quadtree::node::node_prune(unsigned tolerance, int& pruned_size){
	if (!northwest) {
		if (pruned_size != -1) pruned_size += 1;
		cout<<"reached unmodified pix"<<endl;
		return; //base case
	}

	int tr = element.red;
	int tb = element.blue;
	int tg = element.green;
	
	int ner = northeast->element.red;
	int neb = northeast->element.blue;
	int neg = northeast->element.green;

	int nwr = northwest->element.red;
	int nwb = northwest->element.blue;
	int nwg = northwest->element.green;

	int swr = southwest->element.red;
	int swb = southwest->element.blue;
	int swg = southwest->element.green;

	int ser = southeast->element.red;
	int seb = southeast->element.blue;
	int seg = southeast->element.green;

	double dne = pow((tr - ner), 2) + pow((tb - neb), 2) + pow((tg - neg), 2) - (double) tolerance;
	double dnw = pow((tr - nwr), 2) + pow((tb - nwb), 2) + pow((tg - nwg), 2) - (double) tolerance;
	double dse = pow((tr - ser), 2) + pow((tb - seb), 2) + pow((tg - seg), 2) - (double) tolerance;
	double dsw = pow((tr - swr), 2) + pow((tb - swb), 2) + pow((tg - swg), 2) - (double) tolerance;
	cout<<"tr, ner, nwr, swr"<<tr<<","<<ner<<","<<nwr<<","<<swr<<endl;
	cout<<element.red<<endl;
	cout<<"dne, dnw .. "<<dne<<", "<<dnw<<", "<<dse<<endl;
	if (dne>0||dnw>0||dse>0||dsw>0){
		if (!(pruned_size == -1)) pruned_size += 1;
	northeast.get()->node_prune(tolerance, pruned_size);
	northwest.get()->node_prune(tolerance, pruned_size);
	southwest.get()->node_prune(tolerance, pruned_size);
	southeast.get()->node_prune(tolerance, pruned_size);
	}
	else if (dne<=0&&dnw<=0&&dse<=0&&dsw<=0){
		if(pruned_size == -1){
			northeast = nullptr;
			northwest = nullptr;
			southwest = nullptr;
			southeast = nullptr;
		}
		else {
			pruned_size += 1;
		}
	}
	else throw std::runtime_error("node_prune logic error");
}
}

