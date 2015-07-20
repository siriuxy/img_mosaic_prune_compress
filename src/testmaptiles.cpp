/**
 * @file testmaptiles.cpp
 * Runs the maptiles function to test it on some simple tiles.
 */

#include "maptiles.h"

/**
 * Test mapTiles function on a list of several valid tiles
 */
int main()
{
    epng::png src{"testsource.png"};
    source_image source{src, 8};

    std::vector<tile_image> tiles;
    epng::png a{1, 1};
    epng::png b{1, 1};
    epng::png c{1, 1};
    *a(0, 0) = {255, 0, 0};
    *b(0, 0) = {0, 255, 0};
    *c(0, 0) = {0, 0, 255};
    tiles.emplace_back(a);
    tiles.emplace_back(b);
    tiles.emplace_back(c);

    auto canvas = map_tiles(source, tiles);
    auto actual_image = canvas.draw(10);

    actual_image.save("testmaptiles.png");
}
