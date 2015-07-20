/**
 * @file mosaiccanvas.h
 */

#include <iostream>

#include "mosaiccanvas.h"

bool mosaic_canvas::enable_output = false;

namespace
{
uint64_t divide(uint64_t a, uint64_t b)
{
    return (a + b / 2) / b;
}
}

tile_image& mosaic_canvas::images(int row, int col)
{
    return tile_images_[row * columns_ + col];
}

const tile_image& mosaic_canvas::images(int row, int col) const
{
    return tile_images_[row * columns_ + col];
}

mosaic_canvas::mosaic_canvas(int rows, int columns)
    : rows_(rows), columns_(columns)
{
    if (rows < 1 || columns < 1)
        throw std::invalid_argument{"cannot set non-positive rows or columns"};

    tile_images_.resize(rows_ * columns_);
}

int mosaic_canvas::rows() const
{
    return rows_;
}

int mosaic_canvas::columns() const
{
    return columns_;
}

void mosaic_canvas::set_tile(int row, int column, const tile_image& i)
{
    if (enable_output)
    {
        std::cerr << "\rPopulating Mosaic: setting tile (" << row << ", "
                  << column << ")" << std::string(20, ' ') << "\r";
        std::cerr.flush();
    }
    images(row, column) = i;
}

const tile_image& mosaic_canvas::tile(int row, int column) const
{
    return images(row, column);
}

epng::png mosaic_canvas::draw(int pixels_per_tile) const
{
    if (pixels_per_tile <= 0)
    {
        std::cerr << "ERROR: pixels_per_tile must be > 0" << std::endl;
        exit(-1);
    }

    int width = columns() * pixels_per_tile;
    int height = rows() * pixels_per_tile;

    // Create the image
    epng::png mosaic(width, height);

    // Create list of drawable tiles
    for (int row = 0; row < rows(); row++)
    {
        if (enable_output)
        {
            std::cerr << "\rDrawing Mosaic: resizing tiles ("
                      << (row * columns() + /*col*/ 0 + 1) << "/"
                      << (rows() * columns()) << ")" << std::string(20, ' ')
                      << "\r";
            std::cerr.flush();
        }
        for (int col = 0; col < columns(); col++)
        {
            int startX = divide(width * col, columns());
            int endX = divide(width * (col + 1), columns());
            int startY = divide(height * row, rows());
            int endY = divide(height * (row + 1), rows());

            if (endX - startX != endY - startY)
                std::cerr << "Error: resolution not constant: x: "
                          << (endX - startX) << " y: " << (endY - startY)
                          << std::endl;

            images(row, col).paste(mosaic, startX, startY, endX - startX);
        }
    }
    if (enable_output)
    {
        std::cerr << "\r" << std::string(60, ' ');
        std::cerr << "\rDrawing Mosaic: resizing tiles ("
                  << (rows() * columns()) << "/" << (rows() * columns()) << ")"
                  << std::endl;
        std::cerr.flush();
    }

    return mosaic;
}

