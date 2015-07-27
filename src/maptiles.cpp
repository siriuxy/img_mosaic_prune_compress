/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"
mosaic_canvas map_tiles(const source_image& source,
                        const std::vector<tile_image>& tiles)
{
    /**
     * @todo Implement this function!
     */

//should i construct a reference array to convert idx of pts to those on the tiles.

	std::vector<point<3>> tile_pixel_vals;
	for (unsigned i = 0; i < tiles.size(); i++){
		double tmp[]{double(tiles[i].average_color().red), double(tiles[i].average_color().green), double(tiles[i].average_color().blue)};
		tile_pixel_vals.push_back(point<3> (tmp));//is this ok?
	}
	kd_tree<3> tree(tile_pixel_vals);

	mosaic_canvas ret(source.rows(), source.columns());
//	std::cout<<"total row col: "<<source.rows()<<", "<<source.columns()<<std::endl;
	for (int curRow = 0; curRow < ret.rows(); curRow ++){
		for (int curCol = 0; curCol < ret.columns(); curCol ++)
		{
			double tmp[]{double(source.region_color(curRow, curCol).red), double(source.region_color(curRow, curCol).green), double(source.region_color(curRow, curCol).blue)};
			//std::cout<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<std::endl;
			point<3> originalPt(tmp);
			point<3> foundPt = tree.find_nearest_neighbor(originalPt);
			//std::cout<<foundPt[0]<<foundPt[1]<<foundPt[2]<<std::endl;
			for ( auto p : tiles){
				if ( double(p.average_color().red) == foundPt[0] 
					&& double(p.average_color().green) == foundPt[1]
					&& double(p.average_color().blue) == foundPt[2]){
				ret.set_tile(curRow, curCol, p);
				//std::cout<<"set row, col at: "<<curRow<<", "<<curCol<<"| "<<p.resolution()<<"|"<<double(p.average_color().red)<<std::endl;
				break;
				}
			}
		}

	}
	return ret;
}
