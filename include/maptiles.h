/**
 * @file maptiles.h
 *
 * map_tiles() function.
 * Developed for CS225 PhotoMosaic MP
 *
 * @author Jack Toole
 * @date Fall 2011
 */

#ifndef MAPTILES_H_
#define MAPTILES_H_

#include <map>
#include <vector>

#include "epng.h"
#include "kdtree.h"
#include "mosaiccanvas.h"
#include "sourceimage.h"
#include "tileimage.h"

/**
 * Map the image tiles into a mosaic canvas which closely
 * matches the input image.
 *
 * @todo This function is required for mp_mosaic.2
 * @param source The input image to construct a photomosaic of
 * @param tiles The tiles image to use in the mosaic
 */
mosaic_canvas map_tiles(const source_image& source,
                        const std::vector<tile_image>& tiles);

#endif // MAPTILES_H_
