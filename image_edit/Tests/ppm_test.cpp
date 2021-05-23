#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PPM/ppm.h"
#include <fstream>
#include <string>

TEST_CASE("files with comments")
{
    std::ifstream file("../Files/testComment.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM image(format, file, "../Files/testComment.ppm");

    REQUIRE(image.getWidth() == 2);
    REQUIRE(image.getHeight() == 2);
    REQUIRE(image.getPixelRGB(1,1).red == 4);
}