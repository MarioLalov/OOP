#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PGM/pgm.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPGM.pgm", std::ios::binary);
    std::string format;
    file >> format;

    PGM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPGM.pgm");

    REQUIRE(pic.getWidth() == 24);
    REQUIRE(pic.getHeight() == 7);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
    REQUIRE(pic.getPixelGrayscale(1, 2) == 3);
}

TEST_CASE("files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPGM.pgm", std::ios::binary);
    std::string format;
    file >> format;

    PGM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPGM.pgm");

    REQUIRE(pic.getWidth() == 24);
    REQUIRE(pic.getHeight() == 7);
    REQUIRE(pic.getPixelGrayscale(1, 2) == 3);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
}