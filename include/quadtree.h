/**
 * @file quadtree.h
 * quadtree class definition.
 * @date Spring 2008
 * @date Modified Summer 2014
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <iostream>
#include "epng.h"

namespace cs225
{

/**
 * A tree structure that is used to compress epng::png images.
 */
class quadtree
{
  public:
	quadtree();
	quadtree(const epng::png& source, unsigned d);
	quadtree(const quadtree& other);
	quadtree(quadtree&& other);
	~quadtree() = default;
	
	void swap(quadtree& other);
	quadtree& operator=(quadtree other);
	void build_tree(const epng::png& source, unsigned d);
	const epng::rgba_pixel& operator() (unsigned x, unsigned y) const;
	epng::png decompress()const;

	void rotate_clockwise();

	void prune(unsigned tolerance);
	unsigned pruned_size(unsigned tolerance)const;
	unsigned ideal_prune(unsigned leaves)const;

  private:
    /**
     * A simple class representing a single node of a quadtree.
     * You may want to add to this class; in particular, it could
     * probably use a constructor or two...
     */
    class node
    {
      public:
	node() = default;
	node(node &other);
	node(unsigned x, unsigned y, unsigned length);
	node(const epng::png& source, unsigned x, unsigned y, unsigned length);
	
	void colorFiller(epng::png& output, unsigned x, unsigned y);
	auto nodeFinder(unsigned x, unsigned y)const ->node*;

	void rotate_node_clockwise();
	void node_prune(unsigned tolerance, int& pruned_size);//if pruned_size == -1, we do actually prune, and don't worry about the pruned_size value.
	
	bool check_tolerance(node*, unsigned);

        std::unique_ptr<node> northwest;
        std::unique_ptr<node> northeast;
        std::unique_ptr<node> southwest;
        std::unique_ptr<node> southeast;

        epng::rgba_pixel element; // the pixel stored as this node's "data"

	unsigned length_;

    };

    std::unique_ptr<node> root_; // the root of the tree

	unsigned res_;
/**** Do not remove this line or copy its contents here! ****/
#include "quadtree_given.h"
};
}
#endif
