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
    if (curDim < 0) throw std::out_of_range("curDim cannot be smaller than 0");
    else if (curDim >= Dim) {
    //	std::cout<<"dim and cur dim: "<<Dim<<", "<<curDim<<std::endl;
    	throw std::out_of_range("curDim too large for point dimension");
	}
//maybe unnecessary due to static assert?
//
	if (first[curDim] < second[curDim]) return true;
	else if (first[curDim] > second[curDim]) return false;
	else return first<second;
}

template <int Dim>
bool kd_tree<Dim>::should_replace(const point<Dim>& target,
                                 const point<Dim>& current_best,
                                 const point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
	double tarToCur = 0;
	double tarToPot = 0;
	for (int i = 0; i < Dim; i++){
		tarToCur += pow((target[i] - current_best[i]), 2);
		tarToPot += pow((target[i] - potential[i]), 2);
	}
	
	if (tarToPot < tarToCur) return true;
	else if (tarToPot > tarToCur) return false;
	else return potential < current_best;
	
}

template <int Dim>
int kd_tree<Dim>::partition(int start, int end, int pivotIdx, int curDim, std::vector<point<Dim>>& vec){
	//code derived from psudocode on https://en.wikipedia.org/wiki/Quickselect
	int storedIdx = start;
	point<Dim> pivotPt = vec[pivotIdx];
	std::swap(vec[pivotIdx], vec[end]);
	for (int i = start; i <= end; i++){
//		std::cout<<"current pts: "<<vec[i]<<", "<<vec[end]<<std::endl;
		if (smaller_in_dimension(vec[i], pivotPt, curDim)) {
/*			std::cout<<"partitioned, printing: "<<start<<end<<std::endl;
			for (int j = start; j <= end; j++){
				std::cout<<vec[j];
				if (j == i) std::cout<<"<-i";
				if (j == storedIdx) std::cout<<"<-storedIdx";
				std::cout<<std::endl;
			}
*/			
			std::swap(vec[storedIdx], vec[i]);
			storedIdx ++;
		}
	}
	std::swap(vec[storedIdx], vec[end]);
	return storedIdx;
}
template <int Dim>
auto kd_tree<Dim>::select(int start, int end, int curDim, std::vector<point<Dim>>& vec)->point<Dim>{
	int mid = (start + end) / 2 ;
	while (start != end){
		int pivotIdx = start;
		pivotIdx = partition(start, end, pivotIdx, curDim, vec);
	//	std::cout<<"curr pidx = "<<pivotIdx<<start<<end<<std::endl;
		if (mid == pivotIdx) return vec[pivotIdx];
		if (mid  < pivotIdx) end = pivotIdx -1;
		else start = pivotIdx + 1;
	}
	return vec[start];
}

template <int Dim>
void kd_tree<Dim>::kdtreefy(int start, int end, int curDim, std::vector<point<Dim>>& vec){
	if (start >= end) return;//TODO: understand why -1 and not -1 works when >= instead of ==
	else {
	//	std::swap(select(start, end, curDim, vec), vec[(start+end)/2]);
		select(start, end, curDim, vec);
		
		if ((start+end)/2-1) kdtreefy(start, (start+end)/2-1, (curDim+1)%Dim, vec);
		kdtreefy((start+end)/2+1, end, (curDim+1)%Dim, vec);
	}
}


template <int Dim>
kd_tree<Dim>::kd_tree(const std::vector<point<Dim>>& newpoints)
{
    /**
     * @todo Implement this function!
     */
	if (newpoints.size() == 0) return;
	points = newpoints;
	kdtreefy(0, (newpoints.size()-1), 0, points);
}

template <int Dim>
point<Dim> kd_tree<Dim>::find_nearest_neighbor(const point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */\
    return find_nearest_recursively(query, 0, points.size()-1, 0);
}

template <int Dim>
point<Dim> kd_tree<Dim>::find_nearest_recursively(const point<Dim>& query, int start, int end, int curDim) const{
//	std::cout<<start<<", "<<end<<std::endl;
	if (start >= end) return points[start];//TODO: understand why >= and why not ==. a modification suggested by Yi
	else {		
		point<Dim> best;
		if (smaller_in_dimension(query, points[(start+end)/2], curDim)) {
			best = find_nearest_recursively(query, start, (start+end)/2-1, (curDim+1)%Dim);
			if (should_replace(query, best, points[(start+end)/2])) best = points[(start+end)/2];
			double distance_in_dimension = pow(query[curDim] - points[(start+end)/2][curDim], 2);
			double distance_to_best = 0;
				for (int i = 0; i < Dim; i++)
					distance_to_best += pow((query[i] - best[i]), 2);
					
			if (distance_in_dimension <= distance_to_best) {
				point<Dim> otherSubtreeBest = find_nearest_recursively(query, (start+end)/2+1, end, (curDim+1)%Dim);
				if (should_replace(query, best, otherSubtreeBest)) best = otherSubtreeBest;
			}
		}
		else {
			//mirror version of the case in if(){}
			best = find_nearest_recursively(query, (start+end)/2 + 1, end, (curDim+1)%Dim);
			if (should_replace(query, best, points[(start+end)/2])) best = points[(start+end)/2];		
			double distance_in_dimension = pow(query[curDim] - points[(start+end)/2][curDim], 2);
			double distance_to_best = 0;
				for (int i = 0; i < Dim; i++)
					distance_to_best += pow((query[i] - best[i]), 2);
					
			if (distance_in_dimension <= distance_to_best) {
				point<Dim> otherSubtreeBest = find_nearest_recursively(query, start, (start+end)/2-1, (curDim+1)%Dim);
				if (should_replace(query, best, otherSubtreeBest)) best = otherSubtreeBest;
			}
		
		
		}
		return best;
	}
}





/*

rec{
	if base return ;
	if (smaller) best = rec(subtree, left);
	else if (larger) best = rec(subtree, right);
	operate on parent level.

}
*/
