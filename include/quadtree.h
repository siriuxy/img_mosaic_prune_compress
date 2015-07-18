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
	quadtree(epng::png source, unsigned d);
	quadtree(quadtree& other);
	quadtree(quadtree&& other);
	~quadtree() = default;

	quadtree& operator=(quadtree other);

	void swap(quadtree& other);
	void build_tree(const epng::png & source, unsigned d);

	const epng::rgba_pixel& operator()(unsigned x, unsigned y) const;
	epng::png decompress()const;

	bool empty();



  private:
    /**
     * A simple class representing a single node of a quadtree.
     * You may want to add to this class; in particular, it could
     * probably use a constructor or two...
     */
    class node
    {
      public: //
        std::unique_ptr<node> northwest;
        std::unique_ptr<node> northeast;
        std::unique_ptr<node> southwest;
        std::unique_ptr<node> southeast;

        epng::rgba_pixel element; // the pixel stored as this node's "data"

	unsigned node_dis_;//distance from this node, to root_
	unsigned node_start_x_;
	unsigned node_start_y_;

	node(node& other);
    };

//private functions:
	copy_tree(std::unique_ptr<node> newNode, node* other);
	std::unique_ptr<node> build_tree(unique_ptr<node> curr, unsigned side_length);

//private vairaibles:
	unsigned length_; //side length of the image.
	std::unique_ptr<node> root_; // the root of the tree
/**** Do not remove this line or copy its contents here! ****/
#include "quadtree_given.h"
};
}
#endif
