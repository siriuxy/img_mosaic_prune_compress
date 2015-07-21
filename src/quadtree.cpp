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
	length_ = other.length_;
	element = other.element;
	if (other.northwest) northwest = std::unique_ptr<node>(new node(*other.northwest.get()));
	if (other.northeast) northeast = std::unique_ptr<node>(new node(*other.northeast.get()));
	if (other.southwest) southwest = std::unique_ptr<node>(new node(*other.southwest.get()));
	if (other.southeast) southeast = std::unique_ptr<node>(new node(*other.southeast.get()));
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
		element = *source(x, y);
		return;
	}
	else {
		//std::cout<<"current d is: "<<d<<endl;
		length_ = d;
		d = d/2;
		northwest = std::unique_ptr<node>(new node(source, x, y, d));
		northeast = std::unique_ptr<node>(new node(source, x+d, y, d));
		southwest = std::unique_ptr<node>(new node(source, x, y+d, d));
		southeast = std::unique_ptr<node>(new node(source, x+d, y+d, d));

		element.red = (northwest->element.red +
					northeast->element.red +
					southwest->element.red +
					southeast->element.red)/4;
		element.green = (northwest->element.green +
					northeast->element.green +
					southwest->element.green +
					southeast->element.green)/4;	
		element.blue =  (northwest->element.blue +
					northeast->element.blue +
					southwest->element.blue +
					southeast->element.blue)/4;
	}
}

const epng::rgba_pixel& quadtree::operator()(unsigned x, unsigned y)const{
	return root_.get()->nodeFinder(x,y)->element;
}

auto quadtree::node::nodeFinder(unsigned x, unsigned y)const ->node*{
	bool north = false;
	bool west = false;
	if (x <  length_/2){
		west = true;
	}
	else if (length_/2 <= x && x < length_){
		west = false;
		x = x - length_/2;
	}
	else throw std::out_of_range("duh, access x out of range");
	
	if (y < length_/2){
		north = true;
	}
	else if (length_/2 <= y && y < length_){
		north = false;
		y = y - length_/2;
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
	root_.get()->colorFiller(ret, 0, 0);
//	cout<<root_.get()->element.red<<endl;
	return ret;
}

void quadtree::node::colorFiller(epng::png& output, unsigned x, unsigned y){
	//base case is when all of its child is nullptr
	if (!northwest){//others should be null too! maybe leave a test here?
	//	cout<<"current r, g are: "<<element.red<<", "<<int(element.blue)<<endl;
		for (unsigned i = 0; i < length_; i++){
			for (unsigned j = 0; j < length_; j++){
				*output(i+x, j+y) = element;
				}
			}
	}
	else {
		//cout<<"non operational x y: "<<x_<<", "<<y_<<endl;
		northwest->colorFiller(output, x, y);
		northeast->colorFiller(output, x+length_/2, y);
		southwest->colorFiller(output, x, y + length_/2);
		southeast->colorFiller(output, x+length_/2, y+length_/2);
	}
}

void quadtree::rotate_clockwise(){
	if (!root_) throw std::runtime_error("cannot rotate empty img");
	root_.get()->rotate_node_clockwise();
}

void quadtree::node::rotate_node_clockwise(){
	if (!northwest) {
	//	cout<<"reached the end"<<endl;
		return;//cannot use this as nullptr, it seems...
	}
	else {
	//	cout<<"reached parent nod"<<endl;

		std::unique_ptr<node> tmp = std::move(northwest);		
		northwest = std::move(southwest);
		southwest = std::move(southeast);
		southeast = std::move(northeast);
		northeast = std::move(tmp);

	northwest.get()->rotate_node_clockwise();
	northeast.get()->rotate_node_clockwise();
	southwest.get()->rotate_node_clockwise();
	southeast.get()->rotate_node_clockwise();
	}
}

void quadtree::prune(unsigned tolerance){
	int do_prune = -1;
	//cout<<root_.get()->element.red<<", "<<root_.get()->element.blue<<endl;
	root_.get()->node_prune(tolerance, do_prune);	
}

unsigned quadtree::pruned_size(unsigned tolerance) const{
	int pruned_size = 0;
	root_.get()->node_prune(tolerance, pruned_size);
	return pruned_size;
}

void quadtree::node::node_prune(unsigned tolerance, int& pruned_size){
	//cout<<element.red;
	
	if (!northwest) {
		if (pruned_size != -1) pruned_size += 1;
		return;
	}

//	cout<<int(element.blue)<<endl;

	if (all_child_check(this, tolerance, true)) {
			if (pruned_size == -1) {
			//cout<<"entered prune process"<<endl;
				northwest = nullptr;
				northeast = nullptr;
				southwest = nullptr;
				southeast = nullptr;
			}
			else pruned_size -= 3; //it would be at least 4 before minus 3
		}
	else  {
	if (pruned_size != -1) pruned_size += 1;
	northwest->node_prune(tolerance, pruned_size);
	northeast->node_prune(tolerance, pruned_size);
	southwest->node_prune(tolerance, pruned_size);
	southeast->node_prune(tolerance, pruned_size);
	}

}

bool quadtree::node::all_child_check(node* c, unsigned tolerance, bool prunable){

	if (this == c && !c->northwest) return false; 
	if (!c->northwest) return check_tolerance(c, tolerance);
	else return prunable 
		&& all_child_check(c->northwest.get(), tolerance, prunable) 
		&& all_child_check(c->northeast.get(), tolerance, prunable)
		&& all_child_check(c->southeast.get(), tolerance, prunable)
		&& all_child_check(c->southwest.get(), tolerance, prunable);
}

bool quadtree::node::check_tolerance(node* b, unsigned tolerance){
//	cout<<element.bluel;
	double sumSquareDiff = pow(abs(element.red - b->element.red), 2) 
			+ pow(abs(element.blue - b->element.blue), 2)
			+ pow(abs(element.green - b->element.green), 2);
//cout<<sumSquareDiff<<endl;
	return (sumSquareDiff - tolerance) <= 0;
}

/*	if (!northwest) {
		if (pruned_size != -1) pruned_size += 1;
		cout<<"reached unmodified pix"<<endl;
		return; //base case
	}

	int tr = int(element.red);
	int tb = int(element.blue);
	int tg = int(element.green);
	
	int ner = int(northeast->element.red);
	int neb = int(northeast->element.blue);
	int neg = int(northeast->element.green);

	int nwr = int(northwest->element.red);
	int nwb = int(northwest->element.blue);
	int nwg = int(northwest->element.green);

	int swr = int(southwest->element.red);
	int swb = int(southwest->element.blue);
	int swg = int(southwest->element.green);

	int ser = int(southeast->element.red);
	int seb = int(southeast->element.blue);
	int seg = int(southeast->element.green);

	double dne = pow((tr - ner), 2) + pow((tb - neb), 2) + pow((tg - neg), 2) - (double) tolerance;
	double dnw = pow((tr - nwr), 2) + pow((tb - nwb), 2) + pow((tg - nwg), 2) - (double) tolerance;
	double dse = pow((tr - ser), 2) + pow((tb - seb), 2) + pow((tg - seg), 2) - (double) tolerance;
	double dsw = pow((tr - swr), 2) + pow((tb - swb), 2) + pow((tg - swg), 2) - (double) tolerance;
	cout<<"tr, ner, nwr, swr"<<tr<<","<<ner<<","<<nwr<<","<<swr<<endl;
	cout<<element.red<<endl;
	cout<<int(element.red)<<endl;
	cout<<"dne, dnw .. "<<dne<<", "<<dnw<<", "<<dse<<endl;
	if (dne>0||dnw>0||dse>0||dsw>0){
		if (!(pruned_size == -1)) pruned_size += 1;
	northeast.get()->node_prune(tolerance, pruned_size);
	northwest.get()->node_prune(tolerance, pruned_size);
	southwest.get()->node_prune(tolerance, pruned_size);
	southeast.get()->node_prune(tolerance, pruned_size);
	}
	else if (dne<=0&&dnw<=0&&dse<=0&&dsw<=0&& !(northwest->northwest)){
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
}*/
}

