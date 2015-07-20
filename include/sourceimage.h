/**
 * @file sourceimage.h
 *
 * Definition of source_image class.
 *
 * @author Jack Toole
 * @author Matt Sachtler
 * @author Scott Wegner
 *
 * Developed for CS225 PhotoMosaic MP, Fall 2008
 * Rewritten Fall 2011 by Jack Toole
 */

#ifndef SOURCEIMAGE_H_
#define SOURCEIMAGE_H_

#include <cstdint>

#include "epng.h"

/**
 * source_image extends the Image class and provides some additional data and
 * functions suitable for the source image for the photomosaic.  The default
 * constructor will take a number of rows and columns to divide the image
 * into.  The image will then be processes to find the average color of each
 * region.
 */
class source_image
{
  public:
    /**
     * Default constructor
     *
     * @param image The image
     * @param resolution The resolution of the sub-regions. This will be
     * the number of tiles in the larger of the two dimensions of the
     * source_image. If the given resolution is greater than the largest
     * dimension of the image, it will  be automatically set to the pixel
     * dimensions
     */
    source_image(epng::png image, int resolution);

    /**
     * Get the average color of a particular region.  Note, the row and
     * column should be specified with a 0-based index. i.e., The top-left
     * corner is (row, column) (0,0).
     *
     * @param row The row of the particular region in the image
     * @param col The column of the particular region in the image
     *
     * @return The average color of the image
     */
    epng::rgba_pixel region_color(int row, int col) const;

    /**
     * Retrieve the number of row sub-regions the source image
     * is broken into.
     *
     * @return The number of rows
     */
    int rows() const;

    /**
     * Retrieve the number of column sub-regions the source
     * image is broken into.
     *
     * @return The number of columns
     */
    int columns() const;

  private:
    epng::png backing_image_;
    int resolution_;
};

#endif /* _SOURCEIMAGE_H */
