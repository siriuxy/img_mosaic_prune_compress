/**
 * @file mosaiccanvas.h
 *
 * Implementation of the mosaic data structure.
 *
 * @author Jack Toole
 * @author Matt Sachtler
 * @author Scott Wegner
 * @date Fall 2008
 * Developed for CS225 PhotoMosaic MP
 */

#ifndef MOSAICCANVAS_H_
#define MOSAICCANVAS_H_

#include <vector>

#include "epng.h"
#include "tileimage.h"

/**
 * This is the actual mosaic data-structure which will hold the matrix
 * of sub-images to be written.  This is effectively just a 2-D array
 * of tile_image objects which can be accessed via convenience methods.
 */
class mosaic_canvas
{
  public:
    static bool enable_output;

    /**
     * Default constructor
     *
     * @param rows Number of rows to divide the canvas into
     * @param columns Number of columns to divide the canvas into
     */
    mosaic_canvas(int rows, int columns);

    /**
     * Retrieve the number of rows of images
     *
     * @return The number of rows in the mosaic, or -1 on error
     */
    int rows() const;

    /**
     * Retrieve the number of columns of images
     *
     * @return The number of columns in the mosaic, or -1 on error
     */
    int columns() const;

    /**
     * Set the tile_image for a particular region.  Note
     * that row and tile indices should be zero-based.
     *
     * @param row The row
     * @param column The column
     * @param img The tile_image to set
     *
     * @return 0 on success, or non-zero otherwise
     */
    void set_tile(int row, int column, const tile_image& img);

    /**
     * Retrieve the current tile_image for a particular
     * row and column.  If the row or column is out of
     * bounds, the default tile_image is returned.  Note
     * that row and tile indices should be zero-based.
     *
     * @param row The row
     * @param column The column
     *
     * @return The current tile_image for a particular,
     * or the default tile_image if none is set.
     */
    const tile_image& tile(int row, int column) const;

    /**
     * Save the current mosaic_canvas as a file with
     * the following pixels per tile.
     * @param pixels_per_tile pixels per Photomosaic tile
     * @return the Photomosaic as a epng::png object
     */
    epng::png draw(int pixels_per_tile) const;

  private:
    /**
     * Number of image rows in the mosaic
     */
    int rows_;

    /**
     * Number of image columns in the mosaic
     */
    int columns_;

    /**
     * The actual matrix of image data
     */
    std::vector<tile_image> tile_images_;

    tile_image& images(int x, int y);
    const tile_image& images(int x, int y) const;
};

#endif
