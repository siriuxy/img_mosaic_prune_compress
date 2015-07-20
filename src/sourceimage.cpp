/**
 * sourceimage.h
 *
 * source_image extends the Image class and provides some additional data and
 * functions suitable for the source image for the photomosaic.  The default
 * constructor will take a number of rows and columns to divide the image
 * into.  The image will then be processes to find the average color of each
 * region.
 *
 * Authors:
 * Jack Toole
 * Matt Sachtler
 * Scott Wegner
 *
 * Developed for CS225 PhotoMosaic MP, Fall 2008
 * Rewritten Fall 2011 by Jack Toole
 */

#include <algorithm>
#include <cmath>

#include "sourceimage.h"

namespace
{
uint64_t divide(uint64_t a, uint64_t b)
{
    return (a + b / 2) / b;
}
}

source_image::source_image(epng::png image, int res)
    : backing_image_{std::move(image)}, resolution_{res}
{
    if (resolution_ < 1)
        throw std::runtime_error{"resolution set to < 1"};

    resolution_ = std::min(backing_image_.width(), backing_image_.height());
    resolution_ = std::min(resolution_, res);
}

epng::rgba_pixel source_image::region_color(int row, int col) const
{
    int width = backing_image_.width();
    int height = backing_image_.height();

    int startX = divide(width * col, columns());
    int endX = divide(width * (col + 1), columns());
    int startY = divide(height * row, rows());
    int endY = divide(height * (row + 1), rows());

    uint64_t r = 0;
    uint64_t g = 0;
    uint64_t b = 0;

    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            r += backing_image_(x, y)->red;
            g += backing_image_(x, y)->green;
            b += backing_image_(x, y)->blue;
        }
    }

    epng::rgba_pixel color;
    uint64_t numPixels = (endX - startX) * (endY - startY);
    color.red = divide(r, numPixels);
    color.green = divide(g, numPixels);
    color.blue = divide(b, numPixels);
    return color;
}

int source_image::rows() const
{
    if (backing_image_.height() <= backing_image_.width())
        return resolution_;
    else
        return divide(resolution_ * backing_image_.height(),
                      backing_image_.width());
}

int source_image::columns() const
{
    if (backing_image_.width() <= backing_image_.height())
        return resolution_;
    else
        return divide(resolution_ * backing_image_.width(),
                      backing_image_.height());
}

