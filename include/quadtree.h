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







  private:
    /**
     * A simple class representing a single node of a quadtree.
     * You may want to add to this class; in particular, it could
     * probably use a constructor or two...
     */
    class node
    {
      public:
        std::unique_ptr<node> northwest;
        std::unique_ptr<node> northeast;
        std::unique_ptr<node> southwest;
        std::unique_ptr<node> southeast;

        epng::rgba_pixel element; // the pixel stored as this node's "data"
    };

    std::unique_ptr<node> root_; // the root of the tree

/**** Do not remove this line or copy its contents here! ****/
#include "quadtree_given.h"
};
}
#endif
