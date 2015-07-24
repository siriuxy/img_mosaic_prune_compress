/**
 * @file kdtree.h
 * kd_tree implementation using points in k-dimensional space.
 *
 * @author Cinda Heeren
 * @author Jack Toole
 * @author Sean Massung
 * @author Chase Geigle
 * @date Spring 2009
 *
 * Revision history:
 * 3/31/2009     Created
 * 11/10/2009    Modified for MP6 Submission, Fall 2009
 * July 2012     Modified by Sean Massung to remove points_index, forbid
 *                 students from using std::sort, and generally clean up code
 * July 2014     Modified by Chase Geigle to port to C++14
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include <cstdint>
#include <vector>
#include <exception>
#include <cmath>
#include "coloredout.h"
#include "point.h"

/**
 * kd_tree class: implemented using points in Dim dimensional space (given
 * by the template parameter).
 */
template <int Dim>
class kd_tree
{
  public:
    /**
     * Determines if point a is smaller than point b in a given dimension d.
     * If there is a tie, break it with point::operator<().
     *
     * For example:
     *
     * \code
     * point<3> a(1, 2, 3);
     * point<3> b(3, 2, 1);
     * smaller_in_dimension(a, b, 0); // should print true
     * smaller_in_dimension(a, b, 2); // should print false
     * smaller_in_dimension(a, b, 1); // based on operator<, this should be true
     * \endcode
     *
     * @todo This function is required for mp_mosaic.1.
     * @param first point to compare.
     * @param second Second point to compare.
     * @param dim Dimension these points are being compared in.
     * @return A boolean value indicating whether the first point is smaller
     * than the second point in the `dim` dimension.
     */
    bool smaller_in_dimension(const point<Dim>& first, const point<Dim>& second,
                              int dim) const;

    /**
     * Determines if a point is closer to the target point than another
     * reference point. Takes three points: target, current_best, and
     * potential, and returns whether or not potential is closer to
     * target than current_best.
     *
     * We are using Euclidean distance to compare k-dimensional
     * points: \f$\sqrt{(p_1-q_1)^2+(p_2-q_2)^2+\ldots+(p_n-q_n)^2} =
       \sqrt{\sum_{i=1}^n (p_i-q_i)^2}\f$.
     * Note, however, that **minimizing distance is the same as minimizing
     * squared distance**, so you can **avoid invoking the square root**
     * and just compare squared distances in your code.
     *
     * For example:
     *
     * \code
     * point<3> target(1, 3, 5);
     * point<3> curr_best1(1, 3, 2);
     * point<3> poss_best1(2, 4, 4);
     * point<3> curr_best2(1, 3, 6);
     * point<3> poss_best2(2, 4, 4);
     * point<3> curr_best3(0, 2, 4);
     * point<3> poss_best3(2, 4, 6);
     * should_replace(target, curr_best1, poss_best1); // true
     * should_replace(target, curr_best2, poss_best2); // false
     * should_replace(target, curr_best1, poss_best3); // based on operator<,
     *                                                 // this should be false
     * \endcode
     *
     * @todo This function is required for mp_mosaic.1.
     * @param target The point we want to be close to.
     * @param current_best The point that is currently our closest point to
     * target.
     * @param potential Our point that is a candidate to replace
     * current_best (that is, it may be closer to target than current_best).
     * @return a boolean value indicating whether potential is closer to
     * target than current_best. Ties should be broken with
     * point::operator<().
     */
    bool should_replace(const point<Dim>& target,
                        const point<Dim>& current_best,
                        const point<Dim>& potential) const;

    /**
     * Constructs a kd_tree from a vector of points, each having dimension Dim.
     *
     * You may assume the vector has at least one point in it. It should
     * build the tree using recursive helper functions.
     *
     * Since we know the size of the kd_tree at construction, we can
     * represent the tree as a linear structure. For testing, we
     * **require** the following:
     *
     * 1. The root of a subtree in the kd_tree containing array indices
     *    \f$\{a, a+1, a+2, ..., b-1, b\}\f$ **must** be in cell
     *    \f$\left\lfloor\frac{a+b}{2}\right\rfloor\f$.
     *
     *    This means that, in particular, that the root of the kd_tree is
     *    positioned at index
     *    \f$\left\lfloor\frac{0+(n-1)}{2}\right\rfloor\f$.
     *
     *    As you build the tree, your recursive tree building function
     *    calls will refer to points on the left of this index, and to
     *    points on the right of this index.
     * 2. The median node of n nodes is calculated as the cell
     *    \f$\left\lfloor\frac{n-1}{2}\right\rfloor\f$. That is, the
     *    middle node is selected if there are an odd number, and the
     *    item before the middle if there are an even number. **If there
     *    are ties (two points have equal value along a dimension), they
     *    must be decided using point::operator<().** Although this is
     *    arbitrary and doesn't affect the functionality of the kd_tree,
     *    it is required to be able to grade your code.
     *
     * KD-trees are created recursively; at any stage of the construction,
     * the median value in the current dimension is selected. Then, all
     * the elements in the current subtree are divided up into elements
     * which are less than the median, or greater than the median, and
     * then the subtrees are created recursively. The children pick the
     * median in the next dimension, and repeat until the entire set of
     * inputs has been processed. Successive levels of the tree split on
     * increasing dimensions, modulo the total number: a 3D tree will
     * have levels split by dimension 0, 1, 2, 0, 1, 2, etc.
     *
     * You will probably want to write a helper function which performs the
     * median selection and partitioning. Maybe you can use a function you
     * already wrote...
     *
     * @see Here is a reference that should help you create concise,
     * efficient code: [Partition-based General Selection
     * Algorithm](http://en.wikipedia.org/wiki/Selection_algorithm). Note
     * that "select pivotIndex between left and right" means that you
     * should choose a midpoint between the left and right indices.
     *
     * @todo This function is required for mp_mosaic.1.
     * @param newpoints The vector of points to build your kd_tree off of.
     */
    kd_tree(const std::vector<point<Dim>>& newpoints);//TODO:can i randomize the vector first?


    /**
     * Finds the closest point to the parameter point in the kd_tree.
     *
     * This function takes a reference to a template parameter point and
     * returns the point closest to it in the tree. We are defining
     * closest here to be the minimum Euclidean distance between elements.
     * Again, **if there are ties (this time in distance), they must be
     * decided using point::operator<().** Recall that an rgba_pixel is
     * defined by three components: red, green, and blue.
     *
     * The find_nearest_neighbor() search is done in two steps: a search to
     * find the smallest hyperrectangle that contains the target element,
     * and then a back traversal to see if any other hyperrectangle could
     * contain a closer point, which may be a point with smaller distance
     * or a point with equal distance, but a "smaller" point (as defined
     * by operator< in the point class). In the first step, you must
     * recursively traverse down the tree, at each level choosing the
     * subtree which represents the region containing the search element
     * (another place to save some duplicate code?). When you reach the
     * lowest bounding hyperrectangle, then the corresponding node is
     * effectively the "current best" neighbor.
     *
     * However, it may be the case that a better match exists outside of
     * the containing hyperrectangle. At then end of first step of the
     * search, we start traversing back up the kdtree to the parent node.
     * The current best distance defines a radius which contains the
     * nearest neighbor. During the back-traversal (i.e., stepping out of
     * the recursive calls), you must first check if the distance to the
     * parent node is less than the current radius. If so, then that
     * distance now defines the radius, and we replace the "current best"
     * match. Next, it is necessary to check to see if the current
     * splitting plane's distance from search node is within the current
     * radius. If so, then the opposite subtree could contain a closer
     * node, and must also be searched recursively.
     *
     * During the back-traversal, it is important to only check the
     * subtrees that are within the current radius, or else the efficiency
     * of the kdtree is lost. If the distance from the search node to the
     * splitting plane is greater than the current radius, then there
     * cannot possibly be a better nearest neighbor in the subtree, so the
     * subtree can be skipped entirely.
     *
     * You can assume that find_nearest_neighbor will only be called on a
     * valid kd-tree.
     *
     * @see Here is a reference we found quite useful in writing our kd-tree:
     *  [Andrew Moore's KD-Tree Tutorial]
     * (http://www.autonlab.org/autonweb/14665/version/2/part/5/data/moore-tutorial.pdf?branch=main&language=en).
     *
     * @see There is [an example]
     * (https://wiki.engr.illinois.edu/display/cs225/MP+6.1) on the kd_tree
     * spec on the wiki.
     *
     * @todo This function is required for mp_mosaic.1.
     * @param query The point we wish to find the closest neighbor to in
     * the tree.
     * @return The closest point to a in the kd_tree.
     */
    point<Dim> find_nearest_neighbor(const point<Dim>& query) const;

    // functions used for grading:

    /**
     * You do not need to modify this function. Its implementation is in
     * kdtree_extras.tcc.
     * Prints the kd_tree to the terminal in a pretty way.
     */
    void print(std::ostream& out = std::cout,
               colored_out::enable_t enable_bold = colored_out::COUT,
               int modWidth = -1) const;

  private:
    /**
     * This is your kd_tree representation. Modify this vector to create a
     * kd_tree.
     */
    std::vector<point<Dim>> points;

    /**
     * Helper function for grading.
     */
    int getPrintData(int low, int high) const;

    /**
     * Helper function for grading.
     */
    void printTree(int low, int high, std::vector<std::string>& output,
                   int left, int top, int width, int currd) const;

    /**
     * @todo Add your helper functions here.
     */

//TODO: change them into private after testing
public:
	int partition(int start, int end, int pixotIdx, int curDim, std::vector<point<Dim>>& vec);
	auto select(int start, int end, int curDim, std::vector<point<Dim>>& vec)
			->point<Dim>;
	void kdtreefy(int start, int end, int curDim, std::vector<point<Dim>>& newpoints);

	point<Dim> find_nearest_recursively(const point<Dim>& query, int start, int end, int curDim) const;


};

#include "kdtree.tcc"
#include "kdtree_extras.tcc"
#endif
