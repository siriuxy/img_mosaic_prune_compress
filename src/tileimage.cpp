#include <algorithm>
#include <cmath>

#include "tileimage.h"

namespace
{
uint64_t divide(uint64_t a, uint64_t b)
{
    return (a + b / 2) / b;
}

int fdivide(double a, double b)
{
    return a / b + 0.5;
}

double frac(double x)
{
    return x - floor(x);
}

epng::png crop_source_image(const epng::png& source)
{
    int height = source.height();
    int width = source.width();
    int resolution = std::min(width, height);

    int start_x = 0;
    int start_y = 0;

    if (width != height)
    {
        if (height > width)
            start_y = (height - width) / 2;
        else
            start_x = (width - width) / 2;
    }

    epng::png cropped(resolution, resolution);

    for (int y = 0; y < resolution; y++)
        for (int x = 0; x < resolution; x++)
            *cropped(x, y) = *source(start_x + x, start_y + y);

    return cropped;
}
}

tile_image::tile_image() : image_{1, 1}
{
    average_color_ = *image_(0, 0);
}

tile_image::tile_image(const epng::png& source)
    : image_{crop_source_image(source)}
{
    average_color_ = calculate_average_color();
}

epng::rgba_pixel tile_image::average_color() const
{
    return average_color_;
}

uint64_t tile_image::resolution() const
{
    return image_.width();
}

epng::rgba_pixel tile_image::calculate_average_color() const
{
    uint64_t r = 0;
    uint64_t g = 0;
    uint64_t b = 0;

    for (uint64_t y = 0; y < image_.height(); y++)
    {
        for (uint64_t x = 0; x < image_.width(); x++)
        {
            r += image_(x, y)->red;
            g += image_(x, y)->green;
            b += image_(x, y)->blue;
        }
    }

    epng::rgba_pixel color;
    uint64_t pixels = image_.width() * image_.height();
    color.red = divide(r, pixels);
    color.green = divide(g, pixels);
    color.blue = divide(b, pixels);
    return color;
}

void tile_image::paste(epng::png& canvas, int start_x, int start_y,
                       int res) const
{
    // If possible, avoid floating point comparisons. This helps ensure that
    // students'
    // photomosaic's are diff-able with solutions
    if (resolution() % res == 0)
    {
        int scaling_ratio = resolution() / res;

        for (int x = 0; x < res; x++)
        {
            for (int y = 0; y < res; y++)
            {
                int pix_start_x = (x) * scaling_ratio;
                int pix_end_x = (x + 1) * scaling_ratio;
                int pix_start_y = (y) * scaling_ratio;
                int pix_end_y = (y + 1) * scaling_ratio;

                *canvas(start_x + x, start_y + y) = scaled_pixel(
                    pix_start_x, pix_end_x, pix_start_y, pix_end_y);
            }
        }
    }
    else // scaling is necessary
    {
        double scaling_ratio = static_cast<double>(resolution()) / res;

        for (int x = 0; x < res; x++)
        {
            for (int y = 0; y < res; y++)
            {
                double pix_start_x = (double)(x) * scaling_ratio;
                double pix_end_x = (double)(x + 1) * scaling_ratio;
                double pix_start_y = (double)(y) * scaling_ratio;
                double pix_end_y = (double)(y + 1) * scaling_ratio;

                *canvas(start_x + x, start_y + y) = scaled_pixel(
                    pix_start_x, pix_end_x, pix_start_y, pix_end_y);
            }
        }
    }
}

epng::rgba_pixel tile_image::scaled_pixel(double start_x, double end_x,
                                          double start_y, double end_y) const
{
    double left_frac = 1.0 - frac(start_x);
    double right_frac = frac(end_x);
    double top_frac = 1.0 - frac(start_x);
    double bottom_frac = frac(end_x);

    int start_x_int = static_cast<int>(start_x);
    int end_x_int = static_cast<int>(ceil(end_x));
    int start_y_int = static_cast<int>(start_y);
    int end_y_int = static_cast<int>(ceil(end_y));

    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    double total_pixels = 0.0;

    for (int x = start_x_int; x < end_x_int; x++)
    {
        for (int y = start_y_int; y < end_y_int; y++)
        {
            double weight = 1.0;
            if (x == start_x_int)
                weight *= left_frac;
            if (x == end_x_int)
                weight *= right_frac;
            if (y == start_y_int)
                weight *= top_frac;
            if (y == end_y_int)
                weight *= bottom_frac;

            r += image_(x, y)->red * weight;
            g += image_(x, y)->green * weight;
            b += image_(x, y)->blue * weight;
            total_pixels += weight;
        }
    }

    epng::rgba_pixel avg;
    avg.red = fdivide(r, total_pixels);
    avg.green = fdivide(g, total_pixels);
    avg.blue = fdivide(b, total_pixels);
    return avg;
}

epng::rgba_pixel tile_image::scaled_pixel(int start_x_int, int end_x_int,
                                          int start_y_int, int end_y_int) const
{
    uint64_t r = 0;
    uint64_t g = 0;
    uint64_t b = 0;
    uint64_t total_pixels = 0;

    for (int x = start_x_int; x < end_x_int; x++)
    {
        for (int y = start_y_int; y < end_y_int; y++)
        {
            r += image_(x, y)->red;
            g += image_(x, y)->green;
            b += image_(x, y)->blue;
            total_pixels++;
        }
    }

    epng::rgba_pixel avg;
    avg.red = divide(r, total_pixels);
    avg.green = divide(g, total_pixels);
    avg.blue = divide(b, total_pixels);
    return avg;
}
