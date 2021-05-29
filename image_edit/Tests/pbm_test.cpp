#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PBM/pbm.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm");

    REQUIRE(pic.getWidth() == 6);
    REQUIRE(pic.getHeight() == 10);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
    REQUIRE(!pic.getPixelGrayscale(2, 1) == 1);
}

TEST_CASE("files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm");

    REQUIRE(pic.getWidth() == 6);
    REQUIRE(pic.getHeight() == 10);
    REQUIRE(!pic.getPixelGrayscale(2, 1) == 1);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
}

TEST_CASE("crop")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm");

    SECTION("angel of picture crop")
    {
        pic.crop(0, 0, 2, 3);

        REQUIRE(pic.getHeight() == 4);
        REQUIRE(pic.getWidth() == 3);
        REQUIRE(!pic.getPixelGrayscale(1,0) == 1);
    }

    SECTION("mid picture crop")
    {
        pic.crop(1, 1, 3, 3);

        REQUIRE(pic.getHeight() == 3);
        REQUIRE(pic.getWidth() == 3);
        REQUIRE(!pic.getPixelGrayscale(1,0) == 1);
    }

    SECTION("out of bound crop")
    {
        pic.crop(1,1, 8, 15);

        REQUIRE(pic.getHeight() == 9);
        REQUIRE(pic.getWidth() == 5);
        REQUIRE(!pic.getPixelGrayscale(1,0) == 1);
    }

    SECTION("out of bound crop with x out of range parameter")
    {
        pic.crop(1,1, 8, 5);

        REQUIRE(pic.getHeight() == 5);
        REQUIRE(pic.getWidth() == 5);
        REQUIRE(!pic.getPixelGrayscale(1,0) == 1);
    }

    SECTION("out of bound crop with y out of range parameter")
    {
        pic.crop(1,1, 3, 15);

        REQUIRE(pic.getHeight() == 9);
        REQUIRE(pic.getWidth() == 3);
        REQUIRE(!pic.getPixelGrayscale(1,0) == 1);
    }
}

TEST_CASE("resize")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm");

    SECTION("pixel resize")
    {
        pic.resize(7, 8, false);

        REQUIRE(pic.getHeight() == 8);
        REQUIRE(pic.getWidth() == 7);
    }

    SECTION("percentage resize")
    {
        pic.resize(300, 300, true);

        REQUIRE(pic.getHeight() == 30);
        REQUIRE(pic.getWidth() == 18);
    }

}
