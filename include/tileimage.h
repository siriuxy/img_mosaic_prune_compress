/**
 * @file tileimage.h
 * Definition of the tile_image class.
 */

#ifndef TILEIMAGE_H_
#define TILEIMAGE_H_

#include <cstdint>

#include "epng.h"

/**
 * Represents a Tile in the Photomosaic.
 */
class tile_image
{
  private:
    /// The underlying image
    epng::png image_;
    /// The average color of the underlying image
    epng::rgba_pixel average_color_;

  public:
    /**
     * Creates a default tile_image.
     */
    tile_image();

    /**
     * Constructs a tile_image from a png.
     *
     * @param image The png to use to make this tile_image
     */
    explicit tile_image(const epng::png& image);

    /**
     * @return the average color of the underlying image
     */
    epng::rgba_pixel average_color() const;

    /**
     * @return the resolution of the tile (one side)
     */
    uint64_t resolution() const;

    /**
     * Copies this tile into the given coordinates in the provided image.
     *
     * @param canvas The image to copy the tile into
     * @param start_x The x-coordinate for the upper left corner of the tile
     * @param start_y The y-coordinate for the upper left corner of the
     * tile
     * @param resolution The desired resolution for the copy (this will
     * scale the tile_image appropriately when copying the pixels)
     */
    void paste(epng::png& canvas, int start_x, int start_y,
               int resolution) const;

  private:
    epng::rgba_pixel calculate_average_color() const;
    epng::rgba_pixel scaled_pixel(double start_x, double endX, double startY,
                                  double endY) const;
    epng::rgba_pixel scaled_pixel(int start_x, int endX, int startY,
                                  int endY) const;
};

#endif // TILEIMAGE_H
