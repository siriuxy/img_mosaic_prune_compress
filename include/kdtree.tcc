/**
 * @file kdtree.tcc
 * Implementation of kd_tree class.
 */

#include "kdtree.h"

template <int Dim>
bool kd_tree<Dim>::smaller_in_dimension(const point<Dim>& first,
                                       const point<Dim>& second,
                                       int curDim) const
{
    /**
     * @todo Implement this function!
     */
    return false;
}

template <int Dim>
bool kd_tree<Dim>::should_replace(const point<Dim>& target,
                                 const point<Dim>& current_best,
                                 const point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    return false;
}

template <int Dim>
kd_tree<Dim>::kd_tree(const std::vector<point<Dim>>& newpoints)
{
    /**
     * @todo Implement this function!
     */
}

template <int Dim>
point<Dim> kd_tree<Dim>::find_nearest_neighbor(const point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return {};
}
