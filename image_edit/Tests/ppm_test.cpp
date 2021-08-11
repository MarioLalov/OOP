#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PPM/ppm.h"
#include "../Editor/image_editor.h"
#include <fstream>
#include <string>

TEST_CASE("regular ASCII files")
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

TEST_CASE("ASCII files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file);

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1, 1).red == 5);
}

TEST_CASE("binary files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testBinaryPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    PPM pic(format, file);

    REQUIRE(pic.getWidth() == 3);
    REQUIRE(pic.getHeight() == 2);
    REQUIRE(pic.getPixelRgb(1, 1).red == 6);
    REQUIRE(pic.getPixelGrayscale(1, 1) == 6);
}

TEST_CASE("parametric constructor")
{
    SECTION("default behavior")
    {
        Rgb color(78, 30, 56);

        PPM pic("P3", 15, 18, color);

        REQUIRE(pic.getHeight() == 18);
        REQUIRE(pic.getWidth() == 15);
        REQUIRE(pic.getPixelRgb(5, 5).red == 78);
        REQUIRE(pic.getPixelRgb(5, 5).green == 30);
        REQUIRE(pic.getPixelRgb(5, 5).blue == 56);
    }

    SECTION("dimensions error")
    {
        Rgb color(78, 30, 56);

        REQUIRE_THROWS_AS(PPM("P3", 15, -18, color), std::invalid_argument);
    }

    SECTION("negative RGB values error")
    {
        Rgb color(78, -1, 56);

        REQUIRE_THROWS_AS(PPM("P3", 15, 18, color), std::invalid_argument);
    }

    SECTION("too high RGB vlaues error")
    {
        Rgb color(78, 30, 257);

        REQUIRE_THROWS_AS(PPM("P3", 15, 18, color), std::invalid_argument);
    }
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

    Image *pic = new PPM(format, file);

    SECTION("angel of picture crop")
    {
        ImageEditor::crop(pic, 0, 1, 1, 2);

        REQUIRE(pic->getHeight() == 2);
        REQUIRE(pic->getWidth() == 2);
        REQUIRE(pic->getPixelRgb(0, 0).red == 5);
    }

    SECTION("mid picture crop")
    {
        ImageEditor::crop(pic, 1, 1, 2, 2);

        REQUIRE(pic->getHeight() == 2);
        REQUIRE(pic->getWidth() == 2);
        REQUIRE(pic->getPixelRgb(0, 1).red == 10);
    }

    SECTION("out of bound crop")
    {
        ImageEditor::crop(pic, 1, 1, 5, 6);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0, 1).red == 10);
    }

    SECTION("out of bound crop with x out of range parameter")
    {
        ImageEditor::crop(pic, 1, 1, 5, 3);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0, 1).red == 10);
    }

    SECTION("out of bound crop with y out of range parameter")
    {
        ImageEditor::crop(pic, 1, 1, 3, 6);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(pic->getPixelRgb(0, 1).red == 10);
    }

    delete pic;
}

TEST_CASE("resize")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testEditPPM.ppm", std::ios::binary);
    std::string format;
    file >> format;

    Image *pic = new PPM(format, file);

    SECTION("pixel resize")
    {
        ImageEditor::resize(pic, 13, 17, false);

        REQUIRE(pic->getHeight() == 17);
        REQUIRE(pic->getWidth() == 13);
    }

    SECTION("percentage resize")
    {
        ImageEditor::resize(pic, 200, 200, true);

        REQUIRE(pic->getHeight() == 8);
        REQUIRE(pic->getWidth() == 8);
    }

    delete pic;
}