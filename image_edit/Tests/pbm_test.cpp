#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PBM/pbm.h"
#include "../Editor/image_editor.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file);

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

    PBM pic(format, file);

    REQUIRE(pic.getWidth() == 6);
    REQUIRE(pic.getHeight() == 10);
    REQUIRE(!pic.getPixelGrayscale(2, 1) == 1);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
}

TEST_CASE("parametric constructor")
{
    SECTION("default behavior")
    {
        PBM pic("P1", 15, 18, 1);

        REQUIRE(pic.getHeight() == 18);
        REQUIRE(pic.getWidth() == 15);
        REQUIRE(!pic.getPixelGrayscale(3, 3) == 1);
    }

    SECTION("dimensions error")
    {
        REQUIRE_THROWS_AS(PBM("P1", 15, -18, 1), std::invalid_argument);
    }

    SECTION("invalid monochrome value error")
    {
        REQUIRE_THROWS_AS(PBM("P1", 15, 18, 7), std::invalid_argument);
    }
}

TEST_CASE("invalid files")
{
    SECTION("invalid color values")
    {
        std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testInvalidColorPBM.pbm", std::ios::binary);
        std::string format;
        file >> format;

        REQUIRE_THROWS_AS(PBM(format, file), std::invalid_argument);
    }

    SECTION("invalid width and height")
    {
        std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testInvalidDimPBM.pbm", std::ios::binary);
        std::string format;
        file >> format;

        REQUIRE_THROWS_AS(PBM(format, file), std::invalid_argument);
    }
}

TEST_CASE("crop")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM *pic = new PBM(format, file);

    SECTION("angel of picture crop")
    {
        //pic.crop(0, 0, 2, 3);
        ImageEditor::crop(pic, 0, 0, 2, 3);

        REQUIRE(pic->getHeight() == 4);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(!pic->getPixelGrayscale(1, 0) == 1);
    }

    SECTION("mid picture crop")
    {
        ImageEditor::crop(pic, 1, 1, 3, 3);

        REQUIRE(pic->getHeight() == 3);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(!pic->getPixelGrayscale(1, 0) == 1);
    }

    SECTION("out of bound crop")
    {
        //pic.crop(1,1, 8, 15);
        ImageEditor::crop(pic, 1, 1, 8, 15);

        REQUIRE(pic->getHeight() == 9);
        REQUIRE(pic->getWidth() == 5);
        REQUIRE(!pic->getPixelGrayscale(1, 0) == 1);
    }

    SECTION("out of bound crop with x out of range parameter")
    {
        //pic.crop(1, 1, 8, 5);
        ImageEditor::crop(pic, 1, 1, 8, 5);

        REQUIRE(pic->getHeight() == 5);
        REQUIRE(pic->getWidth() == 5);
        REQUIRE(!pic->getPixelGrayscale(1, 0) == 1);
    }

    SECTION("out of bound crop with y out of range parameter")
    {
        //pic.crop(1, 1, 3, 15);
        ImageEditor::crop(pic, 1, 1, 3, 15);

        REQUIRE(pic->getHeight() == 9);
        REQUIRE(pic->getWidth() == 3);
        REQUIRE(!pic->getPixelGrayscale(1, 0) == 1);
    }

    delete pic;
}

TEST_CASE("resize")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM *pic = new PBM(format, file);

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
        ImageEditor::resize(pic, 300, 300, true);

        REQUIRE(pic->getHeight() == 30);
        REQUIRE(pic->getWidth() == 18);
    }

    delete pic;
}
