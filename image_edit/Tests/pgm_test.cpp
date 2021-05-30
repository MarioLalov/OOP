#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PGM/pgm.h"
#include "../Editor/image_editor.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPGM.pgm", std::ios::binary);
    std::string format;
    file >> format;

    PGM pic(format, file);

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

    PGM pic(format, file);

    REQUIRE(pic.getWidth() == 24);
    REQUIRE(pic.getHeight() == 7);
    REQUIRE(pic.getPixelGrayscale(1, 2) == 3);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
}

TEST_CASE("crop")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPGM.pgm", std::ios::binary);
    std::string format;
    file >> format;

    PGM *pic = new PGM(format, file);

    SECTION("angel of picture crop")
    {
        ImageEditor::crop(pic, 0, 0, 9, 4);

        REQUIRE(pic->getHeight() == 5);
        REQUIRE(pic->getWidth() == 10);
        REQUIRE(pic->getPixelGrayscale(1, 1) == 3);
    }

    SECTION("mid picture crop")
    {
        ImageEditor::crop(pic, 1, 1, 3, 3);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelGrayscale(1, 0) == 3);
    }

    SECTION("out of bound crop")
    {
        //pic.crop(1,1, 8, 15);
        ImageEditor::crop(pic, 1, 1, 30, 15);

        REQUIRE(pic->getHeight() == 6);
        REQUIRE(pic->getWidth() == 23);
        REQUIRE(pic->getPixelGrayscale(1, 0) == 3);
    }

    SECTION("out of bound crop with x out of range parameter")
    {
        //pic.crop(1, 1, 8, 5);
        ImageEditor::crop(pic, 1, 1, 30, 3);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 23);
        REQUIRE(pic->getPixelGrayscale(1, 0) == 3);
    }

    SECTION("out of bound crop with y out of range parameter")
    {
        //pic.crop(1, 1, 3, 15);
        ImageEditor::crop(pic, 1, 1, 3, 15);

        REQUIRE(pic->getHeight() == 6);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelGrayscale(1, 0) == 3);
    }

    delete pic;
}

TEST_CASE("resize")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPGM.pgm", std::ios::binary);
    std::string format;
    file >> format;

    PGM* pic = new PGM(format, file);

    SECTION("pixel resize")
    {
        //pic.resize(7, 8, false);
        ImageEditor::resize(pic, 7, 8, false);

        REQUIRE(pic->getHeight() == 8);
        REQUIRE(pic->getWidth() == 7);
    }

    SECTION("percentage resize")
    {
        //pic.resize(300, 300, true);
        ImageEditor::resize(pic, 200, 200, true);

        REQUIRE(pic->getHeight() == 14);
        REQUIRE(pic->getWidth() == 48);
    }

    delete pic;
}