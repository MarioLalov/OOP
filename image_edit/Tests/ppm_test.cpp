#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PPM/ppm.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPPM.ppm");

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1,1).red == 5);
    REQUIRE(pic.getPixelGrayscale(1,1) == 5);
}

TEST_CASE("files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPPM.ppm");

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1,1).red == 5);
}