#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PPM/ppm.h"
#include "../Editor/image_editor.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file);

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1, 1).red == 5);
    REQUIRE(pic.getPixelGrayscale(1, 1) == 5);
}

TEST_CASE("files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file);

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1, 1).red == 5);
}

TEST_CASE("invalid files")
{
    SECTION("invalid color values")
    {
        std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testColorPPM.ppm", std::ios::binary);
        std::string format;
        file >> format;

        REQUIRE_THROWS_AS(PPM(format, file), std::invalid_argument);
    }

    SECTION("invalid max value")
    {
        std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testMaxPPM.ppm", std::ios::binary);
        std::string format;
        file >> format;

        REQUIRE_THROWS_AS(PPM(format, file), std::invalid_argument);
    }

    SECTION("invalid width and height")
    {
        std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testDimPPM.ppm", std::ios::binary);
        std::string format;
        file >> format;

        REQUIRE_THROWS_AS(PPM(format, file), std::invalid_argument);
    }
}

TEST_CASE("crop")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testEditPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM* pic = new PPM(format, file);

    SECTION("angel of picture crop")
    {
        //pic.crop(0, 1, 1, 2);
        ImageEditor::crop(pic, 0, 1, 1, 2);

        REQUIRE(pic->getHeight() == 2);
        REQUIRE(pic->getWidth() == 2);
        REQUIRE(pic->getPixelRgb(0,0).red == 5);
    }

    SECTION("mid picture crop")
    {
        //pic.crop(1, 1, 2, 2);
        ImageEditor::crop(pic, 1, 1, 2, 2);

        REQUIRE(pic->getHeight() == 2);
        REQUIRE(pic->getWidth() == 2);
        REQUIRE(pic->getPixelRgb(0,1).red == 10);
    }

    SECTION("out of bound crop")
    {
        //pic.crop(1,1, 5, 6);
        ImageEditor::crop(pic, 1, 1, 5, 6);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0,1).red == 10);
    }

    SECTION("out of bound crop with x out of range parameter")
    {
        //pic.crop(1,1, 5, 3);
        ImageEditor::crop(pic, 1, 1, 5, 3);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0,1).red == 10);
    }

    SECTION("out of bound crop with y out of range parameter")
    {
        //pic.crop(1,1, 3, 6);
        ImageEditor::crop(pic, 1, 1, 3, 6);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0,1).red == 10);
    }

    delete pic;
}

TEST_CASE("resize")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testEditPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM* pic = new PPM(format, file);

    SECTION("pixel resize")
    {
        //pic.resize(13, 17, false);
        ImageEditor::resize(pic, 13, 17, false);

        REQUIRE(pic->getHeight() == 17);
        REQUIRE(pic->getWidth() == 13);
    }

    SECTION("percentage resize")
    {
        //pic.resize(200, 200, true);
        ImageEditor::resize(pic, 200, 200, true);

        REQUIRE(pic->getHeight() == 8);
        REQUIRE(pic->getWidth() == 8);
    }

    delete pic;
}